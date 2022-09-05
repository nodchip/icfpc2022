#include "stdafx.h"
#include "solver_registry.h"
#include "instruction.h"
#include "similarity_checker.h"
#include <array>
#include <omp.h>
#include <fmt/core.h>

// 上限回数までランダムにcolor + cutする
class RandomCutSolver : public SolverBase {
public:
  struct Option : public OptionBase {
    int max_depth = 4;
    void setOptionParser(CLI::App* app) override {
      app->add_option("--random-cut-max-depth", max_depth);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  RandomCutSolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    const int max_depth = getOption<Option>()->max_depth;
    LOG(INFO) << fmt::format("max_depth = {}", max_depth);
    const auto canvas = args.previous_canvas;
    const auto top_level_id = std::to_string(canvas->calcTopLevelId());
    assert(canvas->blocks[top_level_id]->size == canvas->size());
    const int height = args.painting->height;
    const int width = args.painting->width;

    // 操作列を構築する
    SolverOutputs ret;
    ret.solution = args.optional_initial_solution;
    const auto add_comment = [&](const std::string& comment) {
      ret.solution.push_back(std::make_shared<CommentInstruction>(comment));
    };
    add_comment(fmt::format("max_depth = {}", max_depth));
    std::vector<std::tuple<int, int, int, int, std::string, int>> stack;

    auto block = canvas->blocks[top_level_id];
    stack.emplace_back(block->bottomLeft.py, block->topRight.py, block->bottomLeft.px, block->topRight.px, block->id, 0);

    std::random_device device;
    std::mt19937_64 rng(args.random_seed.value_or(device()));
    while (!stack.empty()) {
      const auto [b, t, l, r, name, depth] = stack.back();
      stack.pop_back();

      int division[2] = {
        std::uniform_int_distribution<int>(0, 1)(rng), // H, V
        std::uniform_int_distribution<int>(0, 2)(rng), // recursive .1, recursive .0, recursive both
      };

      if (division[0] == 0 && b + 1 < t && depth < max_depth) {
        const int y = std::uniform_int_distribution<int>(b + 1, t)(rng);
        if (division[1] == 0) {
          ret.solution.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
          ret.solution.push_back(std::make_shared<HorizontalCutInstruction>(name, y));
          stack.emplace_back(y, t, l, r, name + ".1", depth + 1);
        } else if (division[1] == 1) {
          ret.solution.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
          ret.solution.push_back(std::make_shared<HorizontalCutInstruction>(name, y));
          stack.emplace_back(b, y, l, r, name + ".0", depth + 1);
        } else {
          ret.solution.push_back(std::make_shared<HorizontalCutInstruction>(name, y));
          stack.emplace_back(b, y, l, r, name + ".0", depth + 1);
          stack.emplace_back(y, t, l, r, name + ".1", depth + 1);
        }
      } else if (division[0] == 1 && l + 1 < r && depth < max_depth) {
        const int x = std::uniform_int_distribution<int>(l + 1, r)(rng);
        if (division[1] == 0) {
          ret.solution.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
          ret.solution.push_back(std::make_shared<VerticalCutInstruction>(name, x));
          stack.emplace_back(b, t, x, r, name + ".1", depth + 1);
        } else if (division[1] == 1) {
          ret.solution.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
          ret.solution.push_back(std::make_shared<VerticalCutInstruction>(name, x));
          stack.emplace_back(b, t, l, x, name + ".0", depth + 1);
        } else {
          ret.solution.push_back(std::make_shared<VerticalCutInstruction>(name, x));
          stack.emplace_back(b, t, l, x, name + ".0", depth + 1);
          stack.emplace_back(b, t, x, r, name + ".1", depth + 1);
        }
      } else {
        ret.solution.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
      }
    }

    // for (auto inst : ret.solution) { LOG(INFO) << inst->toString(); }
    return ret;
  }
};

REGISTER_SOLVER_WITH_OPTION("RandomCutSolver", RandomCutSolver, RandomCutSolver::Option);

// vim:ts=2 sw=2 sts=2 et ci
