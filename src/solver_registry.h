#pragma once
#include <string>
#include <functional>
#include <map>
#include <optional>
#include <cassert>
#include <CLI/CLI.hpp>

#include "instruction.h"
#include "interpreter.h"
#include "painter.h"


struct SolverArguments {
  SolverArguments(PaintingPtr painting, CanvasPtr canvas) : painting(painting), canvas(canvas) {}

  PaintingPtr painting;
  CanvasPtr canvas;
  std::vector<std::shared_ptr<Instruction>> optional_initial_solution;

  bool visualize = false;
  std::optional<double> timeout_s;
  std::optional<unsigned int> random_seed;
};

std::optional<CostBreakdown> executeInputSolution(const SolverArguments& args);

struct SolverOutputs {
  std::vector<std::shared_ptr<Instruction>> solution;
};

class OptionBase {
public:
  using Ptr = std::shared_ptr<OptionBase>;

  virtual ~OptionBase() {}
  virtual void setOptionParser(CLI::App* app) { assert(false); }
};

class SolverBase {
public:
  using Ptr = std::shared_ptr<SolverBase>;

  virtual ~SolverBase() {}
  virtual SolverOutputs solve(const SolverArguments &paramin) = 0;
  virtual OptionBase::Ptr createOption() { return std::make_shared<OptionBase>(); }
  void setOption(OptionBase::Ptr option_) { option = option_; }

  template <typename DerivedOption>
  std::shared_ptr<DerivedOption> getOption() {
    return std::dynamic_pointer_cast<DerivedOption>(option);
  }

private:
  OptionBase::Ptr option;
};

#define CONCAT_SUB(a, b) a##b
#define CONCAT(a, b) CONCAT_SUB(a, b)
#define REGISTER_SOLVER(name, cls) \
  static SolverRegistry CONCAT(_register_solver_, __LINE__) = {name, {__FILE__, [] { return std::make_shared<cls>(); }, nullptr }}
#define REGISTER_SOLVER_WITH_OPTION(name, cls, option_cls) \
  static SolverRegistry CONCAT(_register_solver_, __LINE__) = {name, {__FILE__, [] { return std::make_shared<cls>(); }, std::make_shared<option_cls>() }}

struct SolverRegistry {
  struct SolverEntry {
    std::string file_name;
    std::function<SolverBase::Ptr(void)> factory;
    OptionBase::Ptr option;
  };
  static std::map<std::string, SolverEntry>& getRegistry() {
    static std::map<std::string, SolverEntry> s_solver_registry;
    return s_solver_registry;
  }
  static SolverBase::Ptr getSolver(std::string name);
  static void setOptionParser(CLI::App* app);
  static std::string getCanonicalSolverName(std::string name);

  SolverRegistry(std::string name, SolverEntry entry) {
    getRegistry()[name] = entry;
  }

  static void displaySolvers();
};

SolverOutputs solve_with(const std::string& solver_name, PaintingPtr problem, CanvasPtr initial_canvas, const std::vector<std::shared_ptr<Instruction>>& initial_solution);

// vim:ts=2 sw=2 sts=2 et ci
