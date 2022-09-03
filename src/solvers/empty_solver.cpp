#include "stdafx.h"
#include <thread>
#include "solver_registry.h"
#include "instruction.h"

class EmptySolver : public SolverBase {
public:
  EmptySolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;

    // 平均色を吐く
    auto mean_color = *meanColor(*args.painting, Point(0, 0), Point(args.painting->width, args.painting->height));
    auto color = *geometricMedianColor(*args.painting, Point(0, 0), Point(args.painting->width, args.painting->height));
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
// vim:ts=2 sw=2 sts=2 et ci

