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
    SolverOutputs outputs;
    outputs.solution = args.optional_initial_solution;
    auto canvas = args.previous_canvas;

    std::map<std::string, BlockPtr> colored_blocks;
    for (auto&& instruction : outputs.solution) {
      auto color_instruction =
          std::dynamic_pointer_cast<ColorInstruction>(instruction);
      if (!color_instruction) {
        continue;
      }
      auto&& id = color_instruction->block_id;
      colored_blocks[id] = canvas->blocks[color_instruction->block_id];
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
      for (auto iter = outputs.solution.begin(); iter != outputs.solution.end();
           ++iter) {
        auto instruction = std::dynamic_pointer_cast<ColorInstruction>(*iter);
        if (!instruction || instruction->block_id != id) {
          continue;
        }
        color_instruction = instruction;
        outputs.solution.erase(iter);
      }
      for (auto iter = outputs.solution.begin(); iter != outputs.solution.end();
           ++iter) {
        auto instruction = *iter;
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
        outputs.solution.insert(iter, color_instruction);
        colored_blocks[parent_id] = canvas->blocks[parent_id];
      }
    }

    return outputs;
  }

 private:
  static BlockPtr FindSmallestBlock(
      const std::map<std::string, BlockPtr>& colored_blocks) {
    BlockPtr best = colored_blocks.begin()->second;
    for (auto&& kv : colored_blocks) {
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
