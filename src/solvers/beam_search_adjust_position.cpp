#include "stdafx.h"
#include <queue>
#include <thread>
#include <fmt/format.h>
#include "solver_registry.h"
#include "instruction.h"
#include "interpreter.h"
#include "painter.h"

class BeamSearchAdjustPosition : public SolverBase {
public:
  struct Option : public OptionBase {
    int loop = 20;
    int delta = 10;
    bool verbose = false;
    bool adjust_color = false; // あまり効かない(少なくともIntervalDPSolver3に対して)
    int beam_width = 10;
    void setOptionParser(CLI::App* app) override {
      app->add_option("--beam-search-adjust-position-loop", loop);
      app->add_option("--beam-search-adjust-position-delta", delta);
      app->add_option("--beam-search-adjust-position-beam-width", beam_width);
      app->add_flag("--beam-search-adjust-position-verbose", verbose);
      app->add_flag("--beam-search-adjust-position-color", adjust_color);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  BeamSearchAdjustPosition() { }
  SolverOutputs solve(const SolverArguments &args) override {
    const int delta = getOption<Option>()->delta;
    const int loop = getOption<Option>()->loop;
    const bool verbose = getOption<Option>()->verbose;
    const bool adjust_color = getOption<Option>()->adjust_color;
    const int beam_width = getOption<Option>()->beam_width;
    LOG(INFO) << "delta = " << delta << " loop = " << loop << " color = " << adjust_color << " beam_width=" << beam_width;
    SolverOutputs ret;
    auto initial_canvas = args.initial_canvas;
    ret.solution = args.optional_initial_solution;

    // 入力のcutについて、±deltaの範囲でスコアを最高にする物を選ぶ
    // score -> instructions
    std::priority_queue<std::pair<int, std::vector<std::shared_ptr<Instruction>>>> current_candidates;
    current_candidates.push(std::make_pair(std::numeric_limits<int>::max(), ret.solution));
    int best_cost = std::numeric_limits<int>::max();
    std::vector<std::shared_ptr<Instruction>> best_inst;
    bool best_cost_updated = true;
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
              std::optional<CostBreakdown> cost;
              try {
                cost = computeCost(*args.painting, initial_canvas, work);
              }
              catch (const InvalidInstructionException& e) {
                continue;
              }
              if (cost && 0 < cost->total && (next_candidates.size() < beam_width || cost->total < next_candidates.top().first)) {
                if (verbose) LOG(INFO) << fmt::format("[V] update cost {} -> {}", best_cost, cost->total);
                next_candidates.push(std::make_pair(cost->total, work));
                if (next_candidates.size() > beam_width) {
                  next_candidates.pop();
                }
                if (best_cost > cost->total) {
                  best_cost = cost->total;
                  best_inst = work;
                  best_cost_updated = true;
                }
              }
            }
            work[i] = vcut;
          }
          else if (auto hcut = std::dynamic_pointer_cast<HorizontalCutInstruction>(work[i])) {
            for (int d = -delta; d <= delta; ++d) {
              auto new_hcut = std::make_shared<HorizontalCutInstruction>(hcut->block_id, hcut->lineNumber + d);
              work[i] = new_hcut;
              std::optional<CostBreakdown> cost;
              try {
                cost = computeCost(*args.painting, initial_canvas, work);
              }
              catch (const InvalidInstructionException& e) {
                continue;
              }
              if (cost && 0 < cost->total && (next_candidates.size() < beam_width || cost->total < next_candidates.top().first)) {
                if (verbose) LOG(INFO) << fmt::format("[H] update cost {} -> {}", best_cost, cost->total);
                next_candidates.push(std::make_pair(cost->total, work));
                if (next_candidates.size() > beam_width) {
                  next_candidates.pop();
                }
                if (best_cost > cost->total) {
                  best_cost = cost->total;
                  best_inst = work;
                  best_cost_updated = true;
                }
              }
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
                  std::optional<CostBreakdown> cost;
                  try { 
                    cost = computeCost(*args.painting, initial_canvas, work);
                  } catch (const InvalidInstructionException& e) {
                    // ignore
                  }
                  if (cost && 0 < cost->total && (next_candidates.size() < beam_width || cost->total < next_candidates.top().first)) {
                    if (verbose) LOG(INFO) << fmt::format("[C] update cost {} -> {}", best_cost, cost->total);
                    next_candidates.push(std::make_pair(cost->total, work));
                    if (next_candidates.size() > beam_width) {
                      next_candidates.pop();
                    }
                    if (best_cost > cost->total) {
                      best_cost = cost->total;
                      best_inst = work;
                      best_cost_updated = true;
                    }
                  }
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

