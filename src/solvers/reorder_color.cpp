#include "stdafx.h"

#include <array>
#include <memory>

#include <fmt/core.h>
#include <omp.h>

#include "instruction.h"
#include "solver_registry.h"

class ReorderColor : public SolverBase {
  using BlockPtr = std::shared_ptr<Block>;

  static bool CompareBlockSize(const BlockPtr a, const BlockPtr b) {
    return a->size.px * a->size.py < b->size.px * b->size.py;
  }

 public:
  ReorderColor() = default;

  SolverOutputs solve(const SolverArguments& args) override {
    SolverOutputs outputs;
    outputs.solution = args.optional_initial_solution;
    auto canvas = args.previous_canvas;

    std::vector<BlockPtr> colored_blocks;
    for (auto&& instruction : outputs.solution) {
      auto color_instruction =
          std::dynamic_pointer_cast<ColorInstruction>(instruction);
      if (!color_instruction) {
        continue;
      }
      colored_blocks.push_back(canvas->blocks[color_instruction->block_id]);
    }
    std::sort(colored_blocks.begin(), colored_blocks.end(), CompareBlockSize);

    return outputs;
  }
};

REGISTER_SOLVER("ReorderColor", ReorderColor);
// vim:ts=2 sw=2 sts=2 et ci
