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
    int64_t accum_rgba[4] = {0, 0, 0, 0};
    for (auto rgba : args.painting->frame) {
      accum_rgba[0] += rgba.r;
      accum_rgba[1] += rgba.g;
      accum_rgba[2] += rgba.b;
      accum_rgba[3] += rgba.a;
    }
    const Color mean_color(
      double(accum_rgba[0]) / args.painting->frame.size(),
      double(accum_rgba[1]) / args.painting->frame.size(),
      double(accum_rgba[2]) / args.painting->frame.size(),
      double(accum_rgba[3]) / args.painting->frame.size());
    ret.solution.push_back(std::make_shared<ColorInstruction>("0", mean_color));

    // dummy.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    return ret;
  }
};

REGISTER_SOLVER("EmptySolver", EmptySolver);
// vim:ts=2 sw=2 sts=2 et ci

