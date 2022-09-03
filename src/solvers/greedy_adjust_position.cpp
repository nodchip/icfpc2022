#include "stdafx.h"
#include <thread>
#include <fmt/format.h>
#include "solver_registry.h"
#include "instruction.h"
#include "interpreter.h"
#include "painter.h"

class GreedyAdjustPosition : public SolverBase {
public:
  struct Option : public OptionBase {
    int loop = 20;
    int delta = 10;
    void setOptionParser(CLI::App* app) override {
      app->add_option("--loop", loop);
      app->add_option("--greedy-adjust-position-delta", delta);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  GreedyAdjustPosition() { }
  SolverOutputs solve(const SolverArguments &args) override {
    const int delta = getOption<Option>()->delta;
    const int loop = getOption<Option>()->loop;
    LOG(INFO) << "delta = " << delta << " loop = " << loop;
    SolverOutputs ret;
    ret.solution = args.optional_initial_solution;

    // 入力のcutについて、±deltaの範囲でスコアを最高にする物を選ぶ
    std::vector<std::shared_ptr<Instruction>> work = ret.solution;
    int best_cost = std::numeric_limits<int>::max();
    std::vector<std::shared_ptr<Instruction>> best_inst = ret.solution;
    for (size_t iloop = 0; iloop < loop; ++iloop) {
      LOG(INFO) << fmt::format("loop = {}/{}", iloop, loop);
      for (size_t i = 0; i < work.size(); ++i) {
        //LOG(INFO) << fmt::format("i={}/{}({}%)", i, work.size(), 100.0 * i / work.size());
        if (auto vcut = std::dynamic_pointer_cast<VerticalCutInstruction>(best_inst[i])) {
          for (int d = -delta; d <= delta; ++d) {
            auto new_vcut = std::make_shared<VerticalCutInstruction>(vcut->block_id, vcut->lineNumber + d);
            work[i] = new_vcut;
            auto cost = computeCost(*args.painting, work);
            if (cost && 0 < cost->total && cost->total < best_cost) {
              LOG(INFO) << fmt::format("[V] update cost {} -> {}", best_cost, cost->total);
              best_cost = cost->total;
              best_inst = work;
            }
          }
          work[i] = vcut;
        } else if (auto hcut = std::dynamic_pointer_cast<HorizontalCutInstruction>(best_inst[i])) {
          for (int d = -delta; d <= delta; ++d) {
            auto new_hcut = std::make_shared<HorizontalCutInstruction>(hcut->block_id, hcut->lineNumber + d);
            work[i] = new_hcut;
            auto cost = computeCost(*args.painting, work);
            if (cost && 0 < cost->total && cost->total < best_cost) {
              LOG(INFO) << fmt::format("[H] update cost {} -> {}", best_cost, cost->total);
              best_cost = cost->total;
              best_inst = work;
            }
          }
          work[i] = hcut;
        }
      }
    }

    ret.solution = best_inst;
    return ret;
  }
};

REGISTER_SOLVER_WITH_OPTION("GreedyAdjustPosition", GreedyAdjustPosition, GreedyAdjustPosition::Option);
// vim:ts=2 sw=2 sts=2 et ci

