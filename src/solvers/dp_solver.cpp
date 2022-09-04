#include <map>
#include <string>
#include <thread>
#include <vector>

#include <fmt/format.h>

#include "instruction.h"
#include "similarity_checker.h"
#include "solver_registry.h"
#include "stdafx.h"

namespace {

template <typename T, typename... Args>
auto CreateMultiDimVec(std::size_t count, Args&&... args) {
  if constexpr (sizeof...(args) > 1) {
    return std::vector(count, CreateMultiDimVec<T>(args...));
  } else {
    return std::vector<T>(count, args...);
  }
}

}  // namespace

class DpSolver final : public SolverBase {
  static constexpr int kDefaultNumGrids = 10;

  class State {
   public:
    State() = default;

    void InitWithColor(const Painting& painting,
                       const int x0,
                       const int y0,
                       const int x1,
                       const int y1) {
      assert(x0 != x1);
      assert(y0 != y1);
      this->x0 = x0;
      this->y0 = y0;
      this->x1 = x1;
      this->y1 = y1;
      const int area = (y1 - y0) * (x1 - x0);
      instruction_multiplier = 1.0 * painting.width * painting.height / area;

      double action_cost = 0;
      if (area <= 10000) {
        const auto [color, distance] = GetBestColor(painting, x0, y0, x1, y1);
        color_instruction = std::make_shared<ColorInstruction>("", color);
        similarity_cost = distance * SimilarityChecker::alpha;
        base_color = color;
        action_cost =
            std::round(ColorInstruction::kBaseCost * instruction_multiplier);
      } else {
        similarity_cost = 1e+20;
      }

      cost = similarity_cost + action_cost;
    }

    void UpdateOutput(const std::string& block_id,
                      SolverOutputs& output) const {
      assert(instruction || color_instruction);
      if (color_instruction) {
        color_instruction->block_id = block_id;
        output.solution.push_back(color_instruction);
      }

      if (instruction) {
        if (auto p = std::dynamic_pointer_cast<VerticalCutInstruction>(
                instruction)) {
          p->block_id = block_id;
        } else {
          std::dynamic_pointer_cast<HorizontalCutInstruction>(instruction)
              ->block_id = block_id;
        }
        output.solution.push_back(instruction);
      }

      for (int i = 0; i < children.size(); ++i) {
        if (!children[i]) {
          continue;
        }
        std::string block(fmt::format("{}.{}", block_id, i));
        children[i]->UpdateOutput(block, output);
      }
    }

    static std::tuple<RGBA, double> GetBestColor(const Painting& painting,
                                                 const int x0,
                                                 const int y0,
                                                 const int x1,
                                                 const int y1,
                                                 const int num_iteration = 10) {
      const size_t n = (y1 - y0) * (x1 - x0);

      std::vector<RGBA> colors;
      colors.reserve(n);
      for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
          colors.push_back(painting(x, y));
        }
      }

      std::array<double, 4> estimate{0, 0, 0, 0};
      for (auto&& c : colors) {
        estimate[0] += c[0];
        estimate[1] += c[1];
        estimate[2] += c[2];
        estimate[3] += c[3];
      }
      for (int i = 0; i < 4; ++i) {
        estimate[i] = std::round(estimate[i] / n);
      }

      std::vector<double> weights(n);
      for (int iter = 0; iter < num_iteration; ++iter) {
        for (size_t i = 0; i < n; ++i) {
          double d0 = colors[i][0] - estimate[0];
          double d1 = colors[i][1] - estimate[1];
          double d2 = colors[i][2] - estimate[2];
          double d3 = colors[i][3] - estimate[3];
          weights[i] =
              1.0 / (std::sqrt(d0 * d0 + d1 * d1 + d2 * d2 + d3 * d3) + 1e-6);
        }

        estimate = {0.0, 0.0, 0.0, 0.0};
        double sum = 0;
        for (size_t i = 0; i < n; ++i) {
          estimate[0] += colors[i][0] * weights[i];
          estimate[1] += colors[i][1] * weights[i];
          estimate[2] += colors[i][2] * weights[i];
          estimate[3] += colors[i][3] * weights[i];
          sum += weights[i];
        }
        estimate[0] /= sum;
        estimate[1] /= sum;
        estimate[2] /= sum;
        estimate[3] /= sum;
      }

      for (int i = 0; i < 4; ++i) {
        estimate[i] = std::round(estimate[i]);
      }

      // Compute sum of distance
      double diff = 0;
      for (auto&& color : colors) {
        double d0 = color[0] - estimate[0];
        double d1 = color[1] - estimate[1];
        double d2 = color[2] - estimate[2];
        double d3 = color[3] - estimate[3];
        diff += std::sqrt(d0 * d0 + d1 * d1 + d2 * d2 + d3 * d3);
      }

      return {RGBA(estimate[0], estimate[1], estimate[2], estimate[3]), diff};
    }

    double cost = 1e+20;
    double similarity_cost = 1e+20;
    int x0 = -1;
    int y0 = -1;
    int x1 = -1;
    int y1 = -1;
    double instruction_multiplier;
    std::shared_ptr<ColorInstruction> color_instruction;
    std::optional<RGBA> base_color;
    std::shared_ptr<Instruction> instruction;
    std::vector<const State*> children;
  };

  using States = std::vector<std::vector<std::vector<std::vector<State>>>>;

 public:
  struct Option : public OptionBase {
    int num_grids = kDefaultNumGrids;
    void setOptionParser(CLI::App* app) override {
      app->add_option("--dp-num-grids", num_grids);
    }
  };

  OptionBase::Ptr createOption() override { return std::make_shared<Option>(); }

  DpSolver() = default;

  SolverOutputs solve(const SolverArguments& args) override {
    const size_t num_grids = getOption<Option>()->num_grids;
    const Painting& painting = *args.painting;
    assert(painting.width % num_grids == 0);
    assert(painting.height % num_grids == 0);
    LOG(INFO) << "# of grids: " << num_grids;

    // Make a list of Xs and Ys for grids.
    // TODO: Make this list dynamically.
    std::vector<int> xs;
    std::vector<int> ys;
    {
      const int dy = painting.height / num_grids;
      const int dx = painting.width / num_grids;
      for (int i = 0; i <= num_grids; ++i) {
        xs.push_back(dx * i);
        ys.push_back(dy * i);
      }
    }

    const int num_xs = xs.size() - 1;
    const int num_ys = ys.size() - 1;
    auto states = CreateMultiDimVec<State>(num_ys, num_ys + 1, num_xs,
                                           num_xs + 1, State());
    InitializeStates(painting, xs, ys, states);

    LOG(INFO) << "DP search starts";
    for (int diy = 1; diy <= num_ys; ++diy) {
      for (int dix = 1; dix <= num_xs; ++dix) {
        for (int iy0 = 0; iy0 + diy <= num_ys; ++iy0) {
          for (int ix0 = 0; ix0 + dix <= num_xs; ++ix0) {
            const int iy1 = iy0 + diy;
            const int ix1 = ix0 + dix;
            auto& state = states[iy0][iy1][ix0][ix1];
            const double cut_cost =
                std::round(VerticalCutInstruction::kBaseCost *
                           state.instruction_multiplier);
            const double color_cost = std::round(ColorInstruction::kBaseCost *
                                                 state.instruction_multiplier);
            TryHorizontalCut(states, ix0, ix1, iy0, iy1, state);
            TryVerticalCut(states, ix0, ix1, iy0, iy1, state);
            // TODO: Try point cut
          }
        }
      }
    }
    LOG(INFO) << "DP finishes";

    SolverOutputs ret;
    auto&& state = states[0][ys.size() - 1][0][xs.size() - 1];
    const auto comment_instruction = std::make_shared<CommentInstruction>("");
    comment_instruction->comment = fmt::format("Total cost = {0}", state.cost);
    LOG(INFO) << comment_instruction->comment;
    ret.solution.push_back(comment_instruction);

    const auto top_level_id =
        std::to_string(args.previous_canvas->calcTopLevelId());
    state.UpdateOutput(top_level_id, ret);
    return ret;
  }

 private:
  void InitializeStates(const Painting& painting,
                        const std::vector<int>& xs,
                        const std::vector<int>& ys,
                        States& states) {
    LOG(INFO) << "Initialization starts";
    for (int iy0 = 0; iy0 < ys.size() - 1; ++iy0) {
      const int y0 = ys[iy0];
      for (int iy1 = iy0 + 1; iy1 < ys.size(); ++iy1) {
        const int y1 = ys[iy1];
        for (int ix0 = 0; ix0 < xs.size() - 1; ++ix0) {
          const int x0 = xs[ix0];
          for (int ix1 = ix0 + 1; ix1 < xs.size(); ++ix1) {
            const int x1 = xs[ix1];
            auto& state = states[iy0][iy1][ix0][ix1];
            state.InitWithColor(painting, x0, y0, x1, y1);
          }
        }
      }
    }
    LOG(INFO) << "Initialization is done";
  }

  void TryHorizontalCut(const States& states,
                        int ix0,
                        int ix1,
                        int iy0,
                        int iy1,
                        State& state) {
    const double cut_cost = std::round(VerticalCutInstruction::kBaseCost *
                                       state.instruction_multiplier);
    const double color_cost =
        std::round(ColorInstruction::kBaseCost * state.instruction_multiplier);

    for (int iy = iy0 + 1; iy < iy1; ++iy) {
      auto&& state0 = states[iy0][iy][ix0][ix1];
      auto&& state1 = states[iy][iy1][ix0][ix1];
      bool is_cut = false;
      {
        double cost = cut_cost + state0.cost + state1.cost;
        if (cost < state.cost) {
          state.cost = cost;
          state.color_instruction.reset();
          state.children = {&state0, &state1};
          is_cut = true;
        }
      }
      if (state0.base_color) {
        double cost =
            cut_cost + color_cost + state0.similarity_cost + state1.cost;
        if (cost < state.cost) {
          state.cost = cost;
          state.color_instruction =
              std::make_shared<ColorInstruction>("", *state0.base_color);
          state.children = {nullptr, &state1};
          is_cut = true;
        }
      }
      if (state1.base_color) {
        double cost =
            cut_cost + color_cost + state0.cost + state1.similarity_cost;
        if (cost < state.cost) {
          state.cost = cost;
          state.color_instruction =
              std::make_shared<ColorInstruction>("", *state1.base_color);
          state.children = {&state0};
          is_cut = true;
        }
      }
      if (is_cut) {
        state.instruction =
            std::make_shared<HorizontalCutInstruction>("", state0.y1);
      }
    }
  }

  void TryVerticalCut(const States& states,
                      int ix0,
                      int ix1,
                      int iy0,
                      int iy1,
                      State& state) {
    const double cut_cost = std::round(VerticalCutInstruction::kBaseCost *
                                       state.instruction_multiplier);
    const double color_cost =
        std::round(ColorInstruction::kBaseCost * state.instruction_multiplier);

    for (int ix = ix0 + 1; ix < ix1; ++ix) {
      auto&& state0 = states[iy0][iy1][ix0][ix];
      auto&& state1 = states[iy0][iy1][ix][ix1];
      bool is_cut = false;
      {
        double cost = cut_cost + state0.cost + state1.cost;
        if (cost < state.cost) {
          state.cost = cost;
          state.color_instruction.reset();
          state.children = {&state0, &state1};
          is_cut = true;
        }
      }
      if (state0.base_color) {
        double cost =
            cut_cost + color_cost + state0.similarity_cost + state1.cost;
        if (cost < state.cost) {
          state.cost = cost;
          state.color_instruction =
              std::make_shared<ColorInstruction>("", *state0.base_color);
          state.children = {nullptr, &state1};
          is_cut = true;
        }
      }
      if (state1.base_color) {
        double cost =
            cut_cost + color_cost + state0.cost + state1.similarity_cost;
        if (cost < state.cost) {
          state.cost = cost;
          state.color_instruction =
              std::make_shared<ColorInstruction>("", *state1.base_color);
          state.children = {&state0};
          is_cut = true;
        }
      }
      if (is_cut) {
        state.instruction =
            std::make_shared<VerticalCutInstruction>("", state0.x1);
      }
    }
  }
};

REGISTER_SOLVER_WITH_OPTION("DpSolver", DpSolver, DpSolver::Option);
// vim:ts=2 sw=2 sts=2 et ci
