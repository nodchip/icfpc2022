#include "stdafx.h"
#include <thread>
#include <fmt/format.h>
#include "solver_registry.h"
#include "instruction.h"

class EmptySolver : public SolverBase {
public:
  EmptySolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;

    // 平均色を吐く
    auto mean_color = *meanColor(*args.painting, Point(0, 0), Point(args.painting->width, args.painting->height));
    auto color = *geometricMedianColor(*args.painting, Point(0, 0), Point(args.painting->width, args.painting->height), true);
    LOG(INFO) << "mean color = " << mean_color;
    LOG(INFO) << "median color = " << color;
    ret.solution.push_back(std::make_shared<ColorInstruction>("0", color));

    // dummy.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    return ret;
  }
};
REGISTER_SOLVER("EmptySolver", EmptySolver);

class NullSolver : public SolverBase {
public:
  NullSolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;
    ret.solution = args.optional_initial_solution;
    return ret;
  }
};
REGISTER_SOLVER("NullSolver", NullSolver);

class RemoveConflictingColorSolver : public SolverBase {
public:
  RemoveConflictingColorSolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;
    std::set<std::string> found;
    int removed = 0;
    for (auto it = args.optional_initial_solution.rbegin(); it != args.optional_initial_solution.rend(); ++it) {
      if (auto color = std::dynamic_pointer_cast<ColorInstruction>(*it)) {
        if (found.find(color->block_id) == found.end()) {
          ret.solution.push_back(*it);
          found.insert(color->block_id);
        } else {
          ++removed;
        }
      } else {
        ret.solution.push_back(*it);
      }
    }
    std::reverse(ret.solution.begin(), ret.solution.end());
    LOG(INFO) << fmt::format("removed {} color instructions", removed);
    return ret;
  }
};
REGISTER_SOLVER("RemoveConflictingColorSolver", RemoveConflictingColorSolver);
// vim:ts=2 sw=2 sts=2 et ci

