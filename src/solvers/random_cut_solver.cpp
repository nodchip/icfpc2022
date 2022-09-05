#include "stdafx.h"
#include "solver_registry.h"
#include "instruction.h"
#include "similarity_checker.h"
#include "solver_util.h"
#include <array>
#include <omp.h>
#include <fmt/core.h>

// 上限回数までランダムにcolor + cutする
class RandomCutSolver : public SolverBase {
public:
  struct Option : public OptionBase {
    int max_depth = 4;
    int num_trial = 1000;
    void setOptionParser(CLI::App* app) override {
      app->add_option("--random-cut-max-depth", max_depth);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  RandomCutSolver() { }
  SolverOutputs solve(const SolverArguments &args) override {
    const int max_depth = getOption<Option>()->max_depth;
    const int num_trial = getOption<Option>()->num_trial;
    LOG(INFO) << fmt::format("max_depth = {}, num_trial = {}", max_depth, num_trial);
    const auto canvas = args.previous_canvas;
    const auto top_level_id = std::to_string(canvas->calcTopLevelId());
    assert(canvas->blocks[top_level_id]->size == canvas->size());
    const int height = args.painting->height;
    const int width = args.painting->width;

    std::random_device device;
    std::mt19937_64 rng(args.random_seed.value_or(device()));
    GeometricMedianColorCache color_cache(*args.painting);
    std::vector<std::shared_ptr<Instruction>> best_inst;
    int64_t best_cost = std::numeric_limits<int64_t>::max();

    for (int itrial = 0; itrial < num_trial; ++itrial) {
      LOG(INFO) << fmt::format("start trial = {}/{} (best {})", itrial, num_trial, best_cost);

      std::vector<std::shared_ptr<Instruction>> work;
      const auto add_comment = [&](const std::string& comment) {
        work.push_back(std::make_shared<CommentInstruction>(comment));
      };
      add_comment(fmt::format("max_depth = {}", max_depth));
      std::vector<std::tuple<int, int, int, int, std::string, int>> stack;

      auto block = canvas->blocks[top_level_id];
      stack.emplace_back(block->bottomLeft.py, block->topRight.py, block->bottomLeft.px, block->topRight.px, block->id, 0);

      while (!stack.empty()) {
        const auto [b, t, l, r, name, depth] = stack.back();
        stack.pop_back();

        int division[2] = {
          std::uniform_int_distribution<int>(0, 1)(rng), // H, V
          std::uniform_int_distribution<int>(0, 2)(rng), // recursive .1, recursive .0, recursive both
        };

        if (division[0] == 0 && b + 1 < t && depth < max_depth) {
          const int y = std::uniform_int_distribution<int>(b + 1, t - 1)(rng);
          if (division[1] == 0) {
            work.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
            work.push_back(std::make_shared<HorizontalCutInstruction>(name, y));
            stack.emplace_back(y, t, l, r, name + ".1", depth + 1);
          } else if (division[1] == 1) {
            work.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
            work.push_back(std::make_shared<HorizontalCutInstruction>(name, y));
            stack.emplace_back(b, y, l, r, name + ".0", depth + 1);
          } else {
            work.push_back(std::make_shared<HorizontalCutInstruction>(name, y));
            stack.emplace_back(b, y, l, r, name + ".0", depth + 1);
            stack.emplace_back(y, t, l, r, name + ".1", depth + 1);
          }
        } else if (division[0] == 1 && l + 1 < r && depth < max_depth) {
          const int x = std::uniform_int_distribution<int>(l + 1, r - 1)(rng);
          if (division[1] == 0) {
            work.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
            work.push_back(std::make_shared<VerticalCutInstruction>(name, x));
            stack.emplace_back(b, t, x, r, name + ".1", depth + 1);
          } else if (division[1] == 1) {
            work.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
            work.push_back(std::make_shared<VerticalCutInstruction>(name, x));
            stack.emplace_back(b, t, l, x, name + ".0", depth + 1);
          } else {
            work.push_back(std::make_shared<VerticalCutInstruction>(name, x));
            stack.emplace_back(b, t, l, x, name + ".0", depth + 1);
            stack.emplace_back(b, t, x, r, name + ".1", depth + 1);
          }
        } else {
          work.push_back(std::make_shared<ColorInstruction>(name, RGBA()));
        }
      }
      work = replaceColorInstructionOptimal(color_cache, *args.painting, args.initial_canvas, work);

      std::optional<CostBreakdown> cost;
      try { 
        cost = computeCost(*args.painting, args.initial_canvas, work);
      } catch (const InvalidInstructionException& e) {
      }
      if (cost && 0 <= cost->total && cost->total < best_cost) {
        best_cost = cost->total;
        best_inst = work;
      }
    }

    // 操作列を構築する
    SolverOutputs ret;
    ret.solution = best_inst;

    // for (auto inst : ret.solution) { LOG(INFO) << inst->toString(); }
    return ret;
  }
};

REGISTER_SOLVER_WITH_OPTION("RandomCutSolver", RandomCutSolver, RandomCutSolver::Option);

// vim:ts=2 sw=2 sts=2 et ci
