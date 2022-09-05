#include "stdafx.h"

#include <array>
#include <memory>

#include <fmt/core.h>
#include <omp.h>

#include "instruction.h"
#include "solver_registry.h"

class ReorderColor : public SolverBase {
  using BlockPtr = std::shared_ptr<Block>;
  using Instructions = std::vector<std::shared_ptr<Instruction>>;

  struct BlockInfo {
    BlockInfo(BlockPtr b) : block(b), area(b->size.px * b->size.py) {}

    BlockPtr block;
    int area;
    std::shared_ptr<ColorInstruction> color;
    std::shared_ptr<Instruction> instruction;
    std::vector<std::shared_ptr<BlockInfo>> children;
    std::shared_ptr<BlockInfo> parent;

    bool isFullyColored() const { return color || isIndirectlyColored(); }
    bool isIndirectlyColored() const {
      if (children.size() == 0)
        return false;
      for (auto&& child : children) {
        if (!child->isFullyColored())
          return false;
      }
      return true;
    }
  };
  using IdBlockInfoMap = std::map<std::string, std::shared_ptr<BlockInfo>>;
  using BlockInfos = std::vector<std::shared_ptr<BlockInfo>>;

 public:
  ReorderColor() = default;

  SolverOutputs solve(const SolverArguments& args) override {
    auto instructions = args.optional_initial_solution;
    auto infos = BuildBlockInfoTree(args);

    while (MoveColorInstruction(infos)) {
      /* Repeat while any updates happen */
    }

    SolverOutputs outputs;
    outputs.solution = BuildInstructions(infos, instructions);
    return outputs;
  }

 private:
  static BlockInfos BuildBlockInfoTree(const SolverArguments& args) {
    auto operands = std::make_shared<OperandMap>();
    auto&& instructions = args.optional_initial_solution;
    auto cost = computeCost(*args.painting, args.initial_canvas, instructions,
                            operands);
    if (!cost) {
      return {};
    }

    std::map<std::string, std::shared_ptr<BlockInfo>> info_map;
    auto& canvas = cost->canvas;
    for (auto&& instruction : instructions) {
      auto&& operand = (*operands)[instruction];
      if (operand.input_blocks.size() != 1) {
        // Do not process comments, merges, and swaps.
        continue;
      }
      for (auto&& block : operand.input_blocks) {
        if (info_map.find(block->id) == info_map.end()) {
          info_map[block->id] = std::make_shared<BlockInfo>(block);
        }
      }
      for (auto&& block : operand.output_blocks) {
        if (info_map.find(block->id) == info_map.end()) {
          info_map[block->id] = std::make_shared<BlockInfo>(block);
        }
      }
    }

    // Build relationships
    for (auto&& instruction : instructions) {
      auto&& operand = (*operands)[instruction];
      if (operand.input_blocks.size() != 1) {
        // Do not process comments, merges, and swaps.
        continue;
      }

      auto&& input = operand.input_blocks[0];
      auto&& info = info_map[input->id];
      auto&& outputs = operand.output_blocks;
      switch (outputs.size()) {
        case 1:  // ColorInstruction
          info->color =
              std::dynamic_pointer_cast<ColorInstruction>(instruction);
          assert(info->color);
          break;
        case 2:
        case 4:
          info->instruction = instruction;
          for (auto&& output : outputs) {
            auto&& child_info = info_map[output->id];
            info->children.push_back(child_info);
            child_info->parent = info;
          }
          break;
        default:
          LOG(INFO) << outputs.size() << " " << instruction->toString();
          assert(false);
          break;
      }
    }

    BlockInfos infos;
    for (auto&& kv : info_map) {
      infos.push_back(kv.second);
    }
    std::sort(infos.begin(), infos.end(),
              [](auto&& a, auto&& b) -> bool { return a->area < b->area; });
    return infos;
  }

  static bool MoveColorInstruction(BlockInfos& infos) {
    bool updated = false;
    for (auto&& info : infos) {
      if (!info->color) {
        continue;
      }
      auto parent = info->parent;
      if (!parent) {
        continue;
      }
      if (parent->color) {
        if (info->color == parent->color) {
          info->color.reset();
          updated = true;
        }
        continue;
      }

      auto siblings = parent->children;
      bool is_others_fullfilled = true;
      for (auto&& sibling : siblings) {
        if (sibling == info) {
          continue;
        }
        if (!sibling->isFullyColored()) {
          is_others_fullfilled = false;
          break;
        }
      }
      if (is_others_fullfilled) {
        parent->color = info->color;
        info->color.reset();
        parent->color->block_id = parent->block->id;
        updated = true;
      }
    }
    return updated;
  }

  static Instructions BuildInstructions(
      const BlockInfos& infos,
      const Instructions& initial_instructions) {
    Instructions instructions;
    for (auto&& inst : initial_instructions) {
      if (std::dynamic_pointer_cast<ColorInstruction>(inst) ||
          std::dynamic_pointer_cast<VerticalCutInstruction>(inst) ||
          std::dynamic_pointer_cast<HorizontalCutInstruction>(inst) ||
          std::dynamic_pointer_cast<PointCutInstruction>(inst)) {
        break;
      }
      instructions.push_back(inst);
    }

    BlockInfos stack{infos.back()};
    assert(!stack[0]->parent);
    while (!stack.empty()) {
      auto&& info = stack.back();
      stack.pop_back();
      if (info->color) {
        instructions.push_back(info->color);
      }
      if (info->instruction) {
        instructions.push_back(info->instruction);
      }
      for (auto&& child : info->children) {
        stack.push_back(child);
      }
    }

    Instructions tail_instructions;
    for (int i = initial_instructions.size() - 1; i >= 0; --i) {
      auto&& inst = initial_instructions[i];
      if (std::dynamic_pointer_cast<ColorInstruction>(inst) ||
          std::dynamic_pointer_cast<VerticalCutInstruction>(inst) ||
          std::dynamic_pointer_cast<HorizontalCutInstruction>(inst) ||
          std::dynamic_pointer_cast<PointCutInstruction>(inst)) {
        break;
      }
      tail_instructions.push_back(inst);
    }
    for (int i = tail_instructions.size() - 1; i >= 0; --i) {
      instructions.push_back(tail_instructions[i]);
    }

    return instructions;
  }
};

REGISTER_SOLVER("ReorderColor", ReorderColor);
// vim:ts=2 sw=2 sts=2 et ci
