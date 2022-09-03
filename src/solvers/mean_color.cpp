#include "stdafx.h"
#include <thread>
#include "solver_registry.h"
#include "instruction.h"
#include "interpreter.h"
#include "painter.h"

class JustSplitSolver : public SolverBase {
public:
  JustSplitSolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;
    ret.solution = args.optional_initial_solution;
    ret.solution.push_back(std::make_shared<PointCutInstruction>("0", Point(args.painting->width / 2, args.painting->height / 2)));
    return ret;
  }
};
REGISTER_SOLVER("JustSplitSolver", JustSplitSolver);

class MeanColorSolver : public SolverBase {
public:
  MeanColorSolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;
    ret.solution = args.optional_initial_solution;

    // 入力の盤面で区切られたブロックの配置自体は変更せず、各Simpleブロックに対して対応するpaintingの平均値となるcolorだけ発行する
    // TODO: 実際にはcolorを発行しない方がスコアが良い可能性があるので、その場合は発行しないオプションを追加する

    auto executed = executeInputSolution(args);
    assert(executed);

    for (const auto& [block_id, block] : executed->canvas->blocks) {
      if (block->typ == SimpleBlockType) {
        auto color = meanColor(*args.painting, block->bottomLeft, block->topRight);
        assert(color);
        ret.solution.push_back(std::make_shared<ColorInstruction>(block->id, *color));
        LOG(INFO) << "emit " << std::make_shared<ColorInstruction>(block->id, *color)->toString();
      }
    }

    return ret;
  }
};

REGISTER_SOLVER("MeanColorSolver", MeanColorSolver);
// vim:ts=2 sw=2 sts=2 et ci

