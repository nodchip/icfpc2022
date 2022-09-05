#include "stdafx.h"
#include <thread>
#include "solver_registry.h"
#include "instruction.h"
#include "interpreter.h"
#include "painter.h"
#include "solver_util.h"

// オプティマイザ.
// 入力命令列を処理して新命令列を返す:
//   入力color命令の位置やblock idは変えずに、colorだけを変えて良い場合に最適な命令列を生成しようとする
//   誤差の影響で悪化する場合は何もしない
class ColorOptimizer : public SolverBase {
public:
  struct Option : public OptionBase {
    void setOptionParser(CLI::App* app) override {
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  ColorOptimizer() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;

    GeometricMedianColorCache color_cache(*args.painting);
    ret.solution = replaceColorInstructionOptimal(color_cache, *args.painting, args.initial_canvas, args.optional_initial_solution);

    return ret;
  }
};
REGISTER_SOLVER_WITH_OPTION("ColorOptimizer", ColorOptimizer, ColorOptimizer::Option);

class RemoveAdverseInstructionOptimizer : public SolverBase {
public:
  struct Option : public OptionBase {
    void setOptionParser(CLI::App* app) override {
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  RemoveAdverseInstructionOptimizer() { }
  SolverOutputs solve(const SolverArguments &args) override {
    SolverOutputs ret;

    GeometricMedianColorCache color_cache(*args.painting);
    ret.solution = removeAdverseInstruction(color_cache, *args.painting, args.initial_canvas, args.optional_initial_solution);

    return ret;
  }
};
REGISTER_SOLVER_WITH_OPTION("RemoveAdverseInstructionOptimizer", RemoveAdverseInstructionOptimizer, RemoveAdverseInstructionOptimizer::Option);

// vim:ts=2 sw=2 sts=2 et ci

