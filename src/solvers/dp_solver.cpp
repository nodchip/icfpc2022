#include <map>
#include <string>
#include <thread>
#include <vector>

#include <fmt/format.h>

#include "instruction.h"
#include "similarity_checker.h"
#include "solver_registry.h"
#include "stdafx.h"

class DpSolver : public SolverBase {
  static constexpr int kPixelPerGrid = 10;

  class State {
   public:
    State() = default;

    void UpdateOutput(const std::string& block_id,
                      SolverOutputs& output) const {
      assert(instruction);
      switch (instruction->getBaseCost()) {
        case ColorInstruction::kBaseCost:
          std::dynamic_pointer_cast<ColorInstruction>(instruction)->block_id =
              block_id;
          break;
        case VerticalCutInstruction::kBaseCost:
          if (auto p = std::dynamic_pointer_cast<VerticalCutInstruction>(
                  instruction)) {
            p->block_id = block_id;
          } else {
            std::dynamic_pointer_cast<HorizontalCutInstruction>(instruction)
                ->block_id = block_id;
          }
          break;
      }
      output.solution.push_back(instruction);
      for (int i = 0; i < children.size(); ++i) {
        std::string block(fmt::format("{}.{}", block_id, i));
        children[i]->UpdateOutput(block, output);
      }
    }

    double cost = -1;
    std::shared_ptr<Instruction> instruction;
    std::vector<const State*> children;
    std::map<RGBA, int> color_count;
    double similarity = 1e+20;
  };

  using VState = std::vector<State>;
  using V2State = std::vector<VState>;
  using V3State = std::vector<V2State>;
  using V4State = std::vector<V3State>;

 public:
  DpSolver() = default;
  SolverOutputs solve(const SolverArguments& args) override {
    const auto& target = *args.painting;
    assert(target.width % kPixelPerGrid == 0);
    assert(target.height % kPixelPerGrid == 0);

    const int width = target.width / kPixelPerGrid;
    const int height = target.height / kPixelPerGrid;
    dp_.resize(height, V3State(width, V2State(height + 1, VState(width + 1))));
    for (int gy = 0; gy < height; ++gy) {
      for (int gx = 0; gx < width; ++gx) {
        auto& state = dp_[gy][gx][gy + 1][gx + 1];
        for (int y = gy * kPixelPerGrid; y < (gy + 1) * kPixelPerGrid; ++y) {
          for (int x = gx * kPixelPerGrid; x < (gx + 1) * kPixelPerGrid; ++x) {
            auto&& p = target(x, y);
            state.color_count[p] = state.color_count[p] + 1;
          }
        }

        double sum_r = 0;
        double sum_g = 0;
        double sum_b = 0;
        double sum_a = 0;
        for (auto&& cc : state.color_count) {
          auto&& color = cc.first;
          const int n = cc.second;
          sum_r += color[0] * n;
          sum_g += color[1] * n;
          sum_b += color[2] * n;
          sum_a += color[3] * n;
        }
        int avg_r = sum_r / (kPixelPerGrid * kPixelPerGrid) + 0.5;
        int avg_g = sum_g / (kPixelPerGrid * kPixelPerGrid) + 0.5;
        int avg_b = sum_b / (kPixelPerGrid * kPixelPerGrid) + 0.5;
        int avg_a = sum_a / (kPixelPerGrid * kPixelPerGrid) + 0.5;
        RGBA avg(avg_r, avg_g, avg_b, avg_a);
        state.instruction = std::make_shared<ColorInstruction>("", avg);
        state.similarity = 0;
        for (auto&& cc : state.color_count) {
          auto&& color = cc.first;
          const int n = cc.second;
          state.similarity += SimilarityChecker::pixelDiff(avg, color) * n;
        }
        state.similarity *= SimilarityChecker::alpha;
        state.cost = state.instruction->getBaseCost() * width * height +
                     state.similarity;
      }
    }
    SolverOutputs ret;
    auto&& best = SolveDp(0, 0, height, width);
    const auto comment_instruction = std::make_shared<CommentInstruction>("");
    comment_instruction->comment = fmt::format("cost = {0}", best.cost);
    LOG(INFO) << comment_instruction->comment;
    ret.solution.push_back(comment_instruction);
    best.UpdateOutput("0", ret);
    return ret;
  }

 private:
  const State& SolveDp(const int y0, const int x0, const int y1, const int x1) {
    auto&& state = dp_[y0][x0][y1][x1];
    if (state.cost > 0) {
      return state;
    }
    if (y0 == y1 || x0 == x1) {
      state.cost = 1e+10;
      state.instruction = std::make_shared<Instruction>();
      return state;
    }
    const double base_ratio =
        dp_.size() * dp_[0].size() / ((y1 - y0) * (x1 - x0));

    // Try X-line cut
    for (int x = x0 + 1; x < x1; ++x) {
      auto& state0 = SolveDp(y0, x0, y1, x);
      auto& state1 = SolveDp(y0, x, y1, x1);
      if (IsSameColorPaint(state0, state1)) {
        state.instruction = std::make_shared<ColorInstruction>(
            "", std::dynamic_pointer_cast<ColorInstruction>(state0.instruction)
                    ->color);
        state.similarity = state0.similarity + state1.similarity;
        state.cost = state.similarity +
                     state.instruction->getBaseCost() * base_ratio;
        return state;
      }

      double cost = VerticalCutInstruction::kBaseCost * base_ratio +
                    state0.cost + state1.cost;
      if (state.cost < 0 || cost < state.cost) {
        state.cost = cost;
        state.instruction =
            std::make_shared<VerticalCutInstruction>("", x * kPixelPerGrid);
        state.children.clear();
        state.children.push_back(&state0);
        state.children.push_back(&state1);
      }
    }
    // Try Y-line cut
    for (int y = y0 + 1; y < y1; ++y) {
      auto& state0 = SolveDp(y0, x0, y, x1);
      auto& state1 = SolveDp(y, x0, y1, x1);
      if (IsSameColorPaint(state0, state1)) {
        state.instruction = std::make_shared<ColorInstruction>(
            "", std::dynamic_pointer_cast<ColorInstruction>(state0.instruction)
                    ->color);
        state.similarity = state0.similarity + state1.similarity;
        state.cost = state.similarity +
                     state.instruction->getBaseCost() * base_ratio;
        return state;
      }

      double cost = HorizontalCutInstruction::kBaseCost * base_ratio +
                    state0.cost + state1.cost;
      if (state.cost < 0 || cost < state.cost) {
        state.cost = cost;
        state.instruction =
            std::make_shared<HorizontalCutInstruction>("", y * kPixelPerGrid);
        state.children.clear();
        state.children.push_back(&state0);
        state.children.push_back(&state1);
      }
    }

    return state;
  }

  bool IsSameColorPaint(const State& state0, const State& state1) {
    if (state0.instruction->getBaseCost() != ColorInstruction::kBaseCost ||
        state1.instruction->getBaseCost() != ColorInstruction::kBaseCost) {
      return false;
    }
    auto c0 = std::dynamic_pointer_cast<ColorInstruction>(state0.instruction);
    auto c1 = std::dynamic_pointer_cast<ColorInstruction>(state1.instruction);
    return c0->color == c1->color;
  }

  V4State dp_;
};

REGISTER_SOLVER("DpSolver", DpSolver);
// vim:ts=2 sw=2 sts=2 et ci
