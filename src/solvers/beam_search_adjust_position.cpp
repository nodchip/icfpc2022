#include "stdafx.h"
#include <queue>
#include <thread>
#include <fmt/format.h>
#include "solver_registry.h"
#include "instruction.h"
#include "interpreter.h"
#include "painter.h"
#include "solver_util.h"

class BeamSearchAdjustPosition : public SolverBase {
public:
  struct Option : public OptionBase {
    int loop = 20;
    int delta = 10;
    bool verbose = false;
    bool adjust_color = false; // あまり効かない(少なくともIntervalDPSolver3に対して)
    bool optimal_color = false; // かなり重い. cutを動かす際の評価値として、cutを動かした上で最終的なスコアの意味で全colorを最適値に設定した値を用いる. これを用いる場合は多分adjust_colorは不要になる。
    int beam_width = 10;
    void setOptionParser(CLI::App* app) override {
      app->add_option("--beam-search-adjust-position-loop", loop);
      app->add_option("--beam-search-adjust-position-delta", delta);
      app->add_option("--beam-search-adjust-position-beam-width", beam_width);
      app->add_flag("--beam-search-adjust-position-verbose", verbose);
      app->add_flag("--beam-search-adjust-position-color", adjust_color);
      app->add_flag("--beam-search-adjust-position-optimal-color", optimal_color);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  BeamSearchAdjustPosition() { }
  SolverOutputs solve(const SolverArguments &args) override {
    const int delta = getOption<Option>()->delta;
    const int loop = getOption<Option>()->loop;
    const bool verbose = getOption<Option>()->verbose;
    const bool adjust_color = getOption<Option>()->adjust_color;
    const bool optimal_color = getOption<Option>()->optimal_color;
    const int beam_width = getOption<Option>()->beam_width;
    LOG(INFO) << "delta = " << delta << " loop = " << loop << " color = " << adjust_color << " optimal_clor = " << optimal_color << " beam_width=" << beam_width;
    SolverOutputs ret;
    auto initial_canvas = args.initial_canvas;
    ret.solution = args.optional_initial_solution;

    GeometricMedianColorCache geometric_median(*args.painting);
    // 入力のcutについて、±deltaの範囲でスコアを最高にする物を選ぶ
    // score -> instructions
    std::priority_queue<std::pair<int, std::vector<std::shared_ptr<Instruction>>>> current_candidates;
    current_candidates.push(std::make_pair(std::numeric_limits<int>::max(), ret.solution));
    int best_cost = std::numeric_limits<int>::max();
    std::vector<std::shared_ptr<Instruction>> best_inst;
    bool best_cost_updated = true;
    auto try_update = [&](std::priority_queue<std::pair<int, std::vector<std::shared_ptr<Instruction>>>>& next_candidates, const std::vector<std::shared_ptr<Instruction>>& work, bool is_cut, int i, const std::string& tag) {
      std::optional<CostBreakdown> cost;
      try { 
        cost = computeCost(*args.painting, initial_canvas, work);
      } catch (const InvalidInstructionException& e) {
        return false;
      }
      if (!cost || 0 >= cost->total) return false;
      int final_cost = cost->total;
      std::vector<std::shared_ptr<Instruction>> work_opt = work;
      if (optimal_color && is_cut) {
        // 全てのcolorコマンドを最適に設定したと仮定した際のコストに更新
        work_opt = replaceColorInstructionOptimal(geometric_median, *args.painting, initial_canvas, work);
        std::optional<CostBreakdown> cost_opt;
        try { 
          cost_opt = computeCost(*args.painting, initial_canvas, work_opt);
        } catch (const InvalidInstructionException& e) {
          return false;
        }
        if (cost_opt && 0 < cost_opt->total) {
          assert(cost_opt->total <= final_cost);
          if (cost_opt->total < final_cost ) {
            final_cost = cost_opt->total;
          }
        }
      }
      if (next_candidates.size() < beam_width || final_cost < next_candidates.top().first) {
        if (verbose) LOG(INFO) << fmt::format("[{}] update cost {} -> {}", tag, best_cost, final_cost);
        next_candidates.push(std::make_pair(final_cost, work_opt));
        if (next_candidates.size() > beam_width) {
          next_candidates.pop();
        }
        if (best_cost > final_cost) {
          best_cost = final_cost;
          best_inst = work_opt;
          best_cost_updated = true;
        }
      }
      return true;
    };
    for (size_t iloop = 0; best_cost_updated && iloop < loop; ++iloop) {
      std::priority_queue<std::pair<int, std::vector<std::shared_ptr<Instruction>>>> next_candidates;
      if (verbose) LOG(INFO) << fmt::format("loop = {}/{}", iloop, loop);
      best_cost_updated = false;
      while (!current_candidates.empty()) {
        auto work = current_candidates.top().second;
        current_candidates.pop();

        for (size_t i = 0; i < work.size(); ++i) {
          //LOG(INFO) << fmt::format("i={}/{}({}%)", i, work.size(), 100.0 * i / work.size());
          if (auto vcut = std::dynamic_pointer_cast<VerticalCutInstruction>(work[i])) {
            for (int d = -delta; d <= delta; ++d) {
              auto new_vcut = std::make_shared<VerticalCutInstruction>(vcut->block_id, vcut->lineNumber + d);
              work[i] = new_vcut;
              if (!try_update(next_candidates, work, true, i, "V")) continue;
            }
            work[i] = vcut;
          }
          else if (auto hcut = std::dynamic_pointer_cast<HorizontalCutInstruction>(work[i])) {
            for (int d = -delta; d <= delta; ++d) {
              auto new_hcut = std::make_shared<HorizontalCutInstruction>(hcut->block_id, hcut->lineNumber + d);
              work[i] = new_hcut;
              if (!try_update(next_candidates, work, true, i, "H")) continue;
            }
            work[i] = hcut;
          }
          else if (auto col = std::dynamic_pointer_cast<ColorInstruction>(work[i])) {
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
                  try_update(next_candidates, work, false, i, "C");
                  work[i] = col;
                }
              }
            }
          }
        }
      }

      std::swap(current_candidates, next_candidates);
    }

    ret.solution = best_inst;
    return ret;
  }
};

REGISTER_SOLVER_WITH_OPTION("BeamSearchAdjustPosition", BeamSearchAdjustPosition, BeamSearchAdjustPosition::Option);
// vim:ts=2 sw=2 sts=2 et ci

