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
    bool verbose = false;
    bool adjust_color = false; // あまり効かない(少なくともIntervalDPSolver3に対して)
    void setOptionParser(CLI::App* app) override {
      app->add_option("--greedy-adjust-position-loop", loop);
      app->add_option("--greedy-adjust-position-delta", delta);
      app->add_flag("--greedy-adjust-position-verbose", verbose);
      app->add_flag("--greedy-adjust-position-color", adjust_color);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  GreedyAdjustPosition() { }
  SolverOutputs solve(const SolverArguments &args) override {
    const int delta = getOption<Option>()->delta;
    const int loop = getOption<Option>()->loop;
    const bool verbose = getOption<Option>()->verbose;
    const bool adjust_color = getOption<Option>()->adjust_color;
    LOG(INFO) << "delta = " << delta << " loop = " << loop << " color = " << adjust_color;
    SolverOutputs ret;
    auto initial_canvas = args.initial_canvas;
    ret.solution = args.optional_initial_solution;

    // 入力のcutについて、±deltaの範囲でスコアを最高にする物を選ぶ
    int best_cost = std::numeric_limits<int>::max();
    std::vector<std::shared_ptr<Instruction>> best_inst = ret.solution;
    auto try_update = [&](const std::vector<std::shared_ptr<Instruction>>& work, const std::string& tag) {
      std::optional<CostBreakdown> cost;
      try { 
        cost = computeCost(*args.painting, initial_canvas, work);
      } catch (const InvalidInstructionException& e) {
        return false;
      }
      if (cost && 0 < cost->total && cost->total < best_cost) {
        if (verbose) LOG(INFO) << fmt::format("[{}] update cost {} -> {}", tag, best_cost, cost->total);
        best_cost = cost->total;
        best_inst = work;
      }
      return true;
    };
    for (size_t iloop = 0; iloop < loop; ++iloop) {
      LOG(INFO) << fmt::format("start loop = {}/{} (best {})", iloop, loop, best_cost);
      const int best_cost_at_the_beginning_of_loop = best_cost;
      std::vector<std::shared_ptr<Instruction>> work = best_inst;
      for (size_t i = 0; i < work.size(); ++i) {
        //LOG(INFO) << fmt::format("i={}/{}({}%)", i, work.size(), 100.0 * i / work.size());
        if (auto vcut = std::dynamic_pointer_cast<VerticalCutInstruction>(work[i])) {
          for (int d = -delta; d <= delta; ++d) {
            auto new_vcut = std::make_shared<VerticalCutInstruction>(vcut->block_id, vcut->lineNumber + d);
            work[i] = new_vcut;
            if (!try_update(work, "V")) continue;
          }
          work[i] = vcut;
        } else if (auto hcut = std::dynamic_pointer_cast<HorizontalCutInstruction>(work[i])) {
          for (int d = -delta; d <= delta; ++d) {
            auto new_hcut = std::make_shared<HorizontalCutInstruction>(hcut->block_id, hcut->lineNumber + d);
            work[i] = new_hcut;
            if (!try_update(work, "H")) continue;
          }
          work[i] = hcut;
        } else if (auto col = std::dynamic_pointer_cast<ColorInstruction>(work[i])) {
          if (adjust_color) {
            // 存在しないはずの色を設定して全体を描画することで、最後にどの領域に対応する命令かが分かる(ynasu案)
            Point bottomLeft(0, 0), topRight(0, 0);
            {
              const RGBA marker_color(0, 255, 0, 128); // 自然には存在しないはずと思いたい
              auto temp = work;
              temp[i] = std::make_shared<ColorInstruction>(col->block_id, marker_color);
              auto canvas = computeCost(*args.painting, initial_canvas, temp)->canvas;
              assert(canvas);
              for (const auto& [_, block] : canvas->blocks) {
                if (auto simple_block = std::dynamic_pointer_cast<SimpleBlock>(block)) {
                  if (simple_block->color == marker_color) {
                    bottomLeft = simple_block->bottomLeft;
                    topRight = simple_block->topRight;
                    break;
                  }
                }
              }
            }
            if (bottomLeft != topRight) {
              if (verbose) LOG(INFO) << fmt::format("found region to replace. ({},{})-({},{})", bottomLeft.px, bottomLeft.py, topRight.px, topRight.py);
              auto color = geometricMedianColor(*args.painting, bottomLeft, topRight, true);
              if (color) {
                auto new_col = std::make_shared<ColorInstruction>(col->block_id, *color);
                work[i] = new_col;
                try_update(work, "C");
                work[i] = col;
              }
            }
          }
        }
      }
      if (best_cost == best_cost_at_the_beginning_of_loop) {
        break;
      }
    }

    ret.solution = best_inst;
    return ret;
  }
};

REGISTER_SOLVER_WITH_OPTION("GreedyAdjustPosition", GreedyAdjustPosition, GreedyAdjustPosition::Option);
// vim:ts=2 sw=2 sts=2 et ci

