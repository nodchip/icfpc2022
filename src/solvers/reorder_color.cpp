#include "stdafx.h"

#include <array>
#include <memory>

#include <fmt/core.h>
#include <omp.h>

#include "instruction.h"
#include "solver_registry.h"

class ReorderColor : public SolverBase {
  using BlockPtr = std::shared_ptr<Block>;

  static bool CompareBlockSize(const BlockPtr& a, const BlockPtr& b) {
    return a->size.px * a->size.py < b->size.px * b->size.py;
  }

 public:
  ReorderColor() = default;

  SolverOutputs solve(const SolverArguments& args) override {
    LOG(INFO) << "Reorder";
    auto instructions = args.optional_initial_solution;

    auto operands_ptr = std::make_shared<OperandMap>();
    auto&& operands = *operands_ptr;
    auto cost = computeCost(*args.painting, args.initial_canvas, instructions,
                            operands_ptr);
    if (!cost) {
      LOG(ERROR) << fmt::format("failed to run the solution! terminating.");
      return SolverOutputs();
    }
    auto canvas = cost->canvas;
    // Make a list of all blocks
    for (auto&& instruction : instructions) {
      for (auto&& block : operands[instruction].input_blocks) {
        canvas->blocks[block->id] = block;
      }
      for (auto&& block : operands[instruction].output_blocks) {
        canvas->blocks[block->id] = block;
      }
    }

    std::map<std::string, BlockPtr> colored_blocks;
    for (auto&& instruction : instructions) {
      auto color_instruction =
          std::dynamic_pointer_cast<ColorInstruction>(instruction);
      if (!color_instruction) {
        continue;
      }
      auto&& id = color_instruction->block_id;
      auto block = canvas->blocks[color_instruction->block_id];
      assert(block);
      colored_blocks[id] = block;
    }

    while (colored_blocks.size() > 0) {
      auto child = FindSmallestBlock(colored_blocks);
      auto&& id = child->id;
      colored_blocks.erase(id);
      if (id.find('.') == std::string::npos) {  // Root blocks
        continue;
      }
      auto parent_id = id.substr(0, id.size() - 2);
      if (colored_blocks.find(parent_id) != colored_blocks.end()) {
        // If the parent block is colored, we can't pass the ColorInstruction to
        // it.
        continue;
      }
      std::shared_ptr<ColorInstruction> color_instruction;
      // Erase a color instruction that colors `chlid` block.
      for (int i = 0; i < instructions.size(); ++i) {
        auto instruction =
            std::dynamic_pointer_cast<ColorInstruction>(instructions[i]);
        if (!instruction || instruction->block_id != id) {
          continue;
        }
        color_instruction = instruction;
        instructions.erase(instructions.begin() + i);
      }
      // Find a cut instruction that cuts the parent block, and inserts a color
      // instruction before it.
      for (int i = 0; i < instructions.size(); ++i) {
        auto instruction = instructions[i];
        if (auto vcut = std::dynamic_pointer_cast<VerticalCutInstruction>(
                instruction)) {
          if (vcut->block_id != parent_id) {
            continue;
          }
        } else if (auto hcut =
                       std::dynamic_pointer_cast<HorizontalCutInstruction>(
                           instruction)) {
          if (hcut->block_id != parent_id) {
            continue;
          }
        } else if (auto pcut = std::dynamic_pointer_cast<PointCutInstruction>(
                       instruction)) {
          if (pcut->block_id != parent_id) {
            continue;
          }
        } else {
          // Ignore other instruction
          continue;
        }

        color_instruction->block_id = parent_id;
        instructions.insert(instructions.begin() + i, color_instruction);
        colored_blocks[parent_id] = canvas->blocks[parent_id];
        break;
      }
    }

    // TODO: Remove the first color [255,255,255,255] if it was not in input.

    for (auto&& inst : instructions) {
      LOG(INFO) << inst->toString();
    }

    SolverOutputs outputs;
    outputs.solution = instructions;
    return outputs;
  }

 private:
  static BlockPtr FindSmallestBlock(
      const std::map<std::string, BlockPtr>& colored_blocks) {
    BlockPtr best = colored_blocks.begin()->second;
    for (auto kv : colored_blocks) {
      auto&& block = kv.second;
      if (block->size.px * block->size.py < best->size.px * best->size.py) {
        best = block;
      }
    }
    return best;
  }
};

REGISTER_SOLVER("ReorderColor", ReorderColor);
// vim:ts=2 sw=2 sts=2 et ci
