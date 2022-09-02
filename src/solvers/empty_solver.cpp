#include "stdafx.h"
#include <thread>
#include "solver_registry.h"
#include "instruction.h"

class EmptySolver : public SolverBase {
public:
  EmptySolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;

    // 適当な色を吐くだけ
    ret.solution.push_back(std::make_shared<ColorInstruction>("0", Color(255, 0, 0, 255)));

    // dummy.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    return ret;
  }
};

REGISTER_SOLVER("EmptySolver", EmptySolver);
// vim:ts=2 sw=2 sts=2 et ci

