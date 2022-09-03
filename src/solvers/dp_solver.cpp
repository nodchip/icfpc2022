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
  static constexpr int kDefaultPixelPerGrid = 50;

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
    double similarity = 1e+20;
    int x0 = -1;
    int y0 = -1;
    int x1 = -1;
    int y1 = -1;
    std::shared_ptr<Instruction> instruction;
    std::vector<const State*> children;
  };

  using VState = std::vector<State>;
  using V2State = std::vector<VState>;
  using V3State = std::vector<V2State>;
  using V4State = std::vector<V3State>;

 public:
  struct Option : public OptionBase {
    int pixel_per_grid = kDefaultPixelPerGrid;
    void setOptionParser(CLI::App* app) override {
      app->add_option("--dp-pixel-per-grid", pixel_per_grid);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  DpSolver() = default;
  SolverOutputs solve(const SolverArguments& args) override {
    const int ppg = getOption<Option>()->pixel_per_grid;
    const auto& target = *args.painting;
    assert(target.width % ppg == 0);
    assert(target.height % ppg == 0);

    const int width = target.width / ppg;
    const int height = target.height / ppg;
    dp_.resize(height, V3State(width, V2State(height + 1, VState(width + 1))));
    for (int gy = 0; gy < height; ++gy) {
      for (int gx = 0; gx < width; ++gx) {
        const int x0 = gx * ppg;
        const int y0 = gy * ppg;
        const int x1 = (gx + 1) * ppg;
        const int y1 = (gy + 1) * ppg;

        const auto [avg, similarity] = GetBestRGBA(target, x0, y0, x1, y1);
        auto& state = dp_[gy][gx][gy + 1][gx + 1];
        state.x0 = x0;
        state.y0 = y0;
        state.x1 = x1;
        state.y1 = y1;
        state.instruction = std::make_shared<ColorInstruction>("", avg);
        state.similarity = similarity;
        state.cost = state.instruction->getBaseCost() * width * height +
                     state.similarity;
      }
    }
    SolverOutputs ret;
    auto&& best = SolveDp(0, 0, width, height);
    const auto comment_instruction = std::make_shared<CommentInstruction>("");
    comment_instruction->comment = fmt::format("cost = {0}", best.cost);
    LOG(INFO) << comment_instruction->comment;
    ret.solution.push_back(comment_instruction);
    best.UpdateOutput("0", ret);
    LOG(INFO) << "Total cost: " << best.cost;
    return ret;
  }

 private:
  const State& SolveDp(const int x0, const int y0, const int x1, const int y1) {
    auto&& state = dp_[y0][x0][y1][x1];
    if (state.cost > 0) {
      return state;
    }
    if (y0 == y1 || x0 == x1) {
      assert(false);
      state.cost = 1e+10;
      state.instruction = std::make_shared<Instruction>();
      return state;
    }
    const double base_ratio =
        dp_.size() * dp_[0].size() / ((y1 - y0) * (x1 - x0));

    // Try X-line cut
    for (int x = x0 + 1; x < x1; ++x) {
      auto& state0 = SolveDp(x0, y0, x, y1);
      auto& state1 = SolveDp(x, y0, x1, y1);
      if (IsSameColorPaint(state0, state1)) {
        state.instruction = std::make_shared<ColorInstruction>(
            "", std::dynamic_pointer_cast<ColorInstruction>(state0.instruction)
                    ->color);
        state.similarity = state0.similarity + state1.similarity;
        state.cost =
            state.similarity + state.instruction->getBaseCost() * base_ratio;
        return state;
      }

      double cost = VerticalCutInstruction::kBaseCost * base_ratio +
                    state0.cost + state1.cost;
      if (state.cost < 0 || cost < state.cost) {
        state.cost = cost;
        state.instruction = std::make_shared<VerticalCutInstruction>(
            "", x * getOption<Option>()->pixel_per_grid);
        state.children.clear();
        state.children.push_back(&state0);
        state.children.push_back(&state1);
      }
    }
    // Try Y-line cut
    for (int y = y0 + 1; y < y1; ++y) {
      auto& state0 = SolveDp(x0, y0, x1, y);
      auto& state1 = SolveDp(x0, y, x1, y1);
      if (IsSameColorPaint(state0, state1)) {
        state.instruction = std::make_shared<ColorInstruction>(
            "", std::dynamic_pointer_cast<ColorInstruction>(state0.instruction)
                    ->color);
        state.similarity = state0.similarity + state1.similarity;
        state.cost =
            state.similarity + state.instruction->getBaseCost() * base_ratio;
        return state;
      }

      double cost = HorizontalCutInstruction::kBaseCost * base_ratio +
                    state0.cost + state1.cost;
      if (state.cost < 0 || cost < state.cost) {
        state.cost = cost;
        state.instruction = std::make_shared<HorizontalCutInstruction>(
            "", y * getOption<Option>()->pixel_per_grid);
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

  static std::tuple<RGBA, double> GetBestRGBA(const Painting& painting,
                                              const int x0,
                                              const int y0,
                                              const int x1,
                                              const int y1) {
    std::map<RGBA, int> color_count;
    for (int y = y0; y < y1; ++y) {
      for (int x = x0; x < x1; ++x) {
        auto&& p = painting(x, y);
        color_count[p] = color_count[p] + 1;
      }
    }

    double sum_r = 0;
    double sum_g = 0;
    double sum_b = 0;
    double sum_a = 0;
    int count = 0;
    for (auto&& cc : color_count) {
      auto&& color = cc.first;
      const int n = cc.second;
      sum_r += color[0] * n;
      sum_g += color[1] * n;
      sum_b += color[2] * n;
      sum_a += color[3] * n;
      count += n;
    }

    int avg_r = std::round(sum_r / count);
    int avg_g = std::round(sum_g / count);
    int avg_b = std::round(sum_b / count);
    int avg_a = std::round(sum_a / count);
    RGBA avg(avg_r, avg_g, avg_b, avg_a);

    double similarity = 0;
    for (auto&& cc : color_count) {
      auto&& color = cc.first;
      const int n = cc.second;
      similarity += SimilarityChecker::pixelDiff(avg, color) * n;
    }

    return {avg, similarity * SimilarityChecker::alpha};
  }

  V4State dp_;
};

REGISTER_SOLVER_WITH_OPTION("DpSolver", DpSolver, DpSolver::Option);
// vim:ts=2 sw=2 sts=2 et ci
