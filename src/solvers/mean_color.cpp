#include "stdafx.h"
#include <thread>
#include "solver_registry.h"
#include "instruction.h"
#include "interpreter.h"
#include "painter.h"

class MeanColorSolver : public SolverBase {
public:
  MeanColorSolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;
    ret.solution = args.optional_initial_solution;

    // 入力の盤面で区切られたブロックの配置自体は変更せず、全ブロックに対して対応するpaintingの平均値となるcolorだけ発行する
    // TODO: 実際にはcolorを発行しない方がスコアが良い可能性があるので、その場合は発行しないオプションを追加する

    // Interpreter::Evaluate(args.optional_initial_solution); // Run()は文字列なので、Instruction列を入れるタイプの入力
    std::shared_ptr<InterpreterResult> interpreter_result;
    LOG(ERROR) << "Not Implemented!!";
    assert(interpreter_result);

    for (const auto& [block_id, block] : interpreter_result->canvas->blocks) {
      auto color = meanColor(*args.painting, block->bottomLeft, block->topRight);
      assert(color);
      ret.solution.push_back(std::make_shared<ColorInstruction>(block->id, *color));
    }

    return ret;
  }
};

REGISTER_SOLVER("MeanColorSolver", MeanColorSolver);
// vim:ts=2 sw=2 sts=2 et ci

