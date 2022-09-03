#include "stdafx.h"
#include "solver_registry.h"
#include "instruction.h"
#include "similarity_checker.h"
#include <fmt/core.h>

class ColorOnlySolver : public SolverBase {
public:
  ColorOnlySolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    double sum_delta_cost = 0.0;
    SolverOutputs ret;
    for (const auto& [id, block] : args.canvas->blocks) {
      const Color current_color = std::dynamic_pointer_cast<SimpleBlock>(block)->color;
      const Color best_color = geometricMedianColor(*args.painting, block->bottomLeft, block->topRight).value();
      const ColorInstruction instruction(id, best_color);
      double delta_cost = getCost(instruction, block->size.getScalarSize(), args.canvas->size().getScalarSize());
      for (int y = block->bottomLeft.py; y < block->topRight.py; ++y) {
        for (int x = block->bottomLeft.px; x < block->topRight.px; ++x) {
          delta_cost -= SimilarityChecker::pixelDiff((*args.painting)(x, y), current_color);
          delta_cost += SimilarityChecker::pixelDiff((*args.painting)(x, y), best_color);
        }
      }
      delta_cost *= SimilarityChecker::alpha;
      delta_cost += getCost(instruction, block->size.getScalarSize(), args.canvas->size().getScalarSize());
      LOG(INFO) << fmt::format("id = {0}: delta = {1}", id, delta_cost);
      if (delta_cost < 0.0) {
        sum_delta_cost += delta_cost;
        ret.solution.push_back(std::make_shared<ColorInstruction>(id, best_color));
      }
    }

    return ret;
  }
};

REGISTER_SOLVER("ColorOnlySolver", ColorOnlySolver);
// vim:ts=2 sw=2 sts=2 et ci
