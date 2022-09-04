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

      auto color =
          geometricMedianColor(painting, Point(x0, y0), Point(x1, y1), 10)
              .value();
      instruction = std::make_shared<ColorInstruction>("", color);

      similarity_cost = 0;
      for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
          similarity_cost +=
              SimilarityChecker::pixelDiff(color, painting(x, y));
        }
      }
      similarity_cost *= SimilarityChecker::alpha;

      const double area = (y1 - y0) * (x1 - x0);
      instruction_multiplier = 1.0 * painting.width * painting.height / area;

      double action_cost = ColorInstruction::kBaseCost * instruction_multiplier;
      cost = similarity_cost + action_cost;
    }

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

    double cost = 1e+20;
    double similarity_cost = 1e+20;
    int x0 = -1;
    int y0 = -1;
    int x1 = -1;
    int y1 = -1;
    double instruction_multiplier;
    // TODO: Use enum or other imm values.
    std::shared_ptr<Instruction> instruction;
    std::vector<const State*> children;
  };

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

    assert(xs.size() == num_grids + 1);
    assert(ys.size() == num_grids + 1);
    auto states = CreateMultiDimVec<State>(num_grids, num_grids + 1, num_grids,
                                           num_grids + 1, State());

    // Initialize with Color instruction.
    for (int iy0 = 0; iy0 < ys.size(); ++iy0) {
      const int y0 = ys[iy0];
      for (int iy1 = iy0 + 1; iy1 < ys.size(); ++iy1) {
        const int y1 = ys[iy1];
        for (int ix0 = 0; ix0 < xs.size(); ++ix0) {
          const int x0 = xs[ix0];
          for (int ix1 = ix0 + 1; ix1 < xs.size(); ++ix1) {
            const int x1 = xs[ix1];
            auto& state = states[iy0][iy1][ix0][ix1];
            state.InitWithColor(painting, x0, y0, x1, y1);
          }
        }
      }
    }

    // DP search
    for (int diy = 1; diy < ys.size(); ++diy) {
      for (int dix = 1; dix < xs.size(); ++dix) {
        for (int iy0 = 0; iy0 + diy < ys.size(); ++iy0) {
          for (int ix0 = 0; ix0 + dix < xs.size(); ++ix0) {
            const int iy1 = iy0 + diy;
            const int ix1 = ix0 + dix;
            auto& state = states[iy0][iy1][ix0][ix1];
            const double cut_cost = VerticalCutInstruction::kBaseCost *
                                    state.instruction_multiplier;
            const double color_cost =
                ColorInstruction::kBaseCost * state.instruction_multiplier;

            // Try horizontal cut
            for (int iy = iy0 + 1; iy < iy1; ++iy) {
              auto&& state0 = states[iy0][iy][ix0][ix1];
              auto&& state1 = states[iy][iy1][ix0][ix1];
              double min_cost = state.cost;
              min_cost =
                  std::min(min_cost, cut_cost + state0.cost + state1.cost);
              min_cost =
                  std::min(min_cost, cut_cost + color_cost +
                                         state0.similarity_cost + state1.cost);
              min_cost =
                  std::min(min_cost, cut_cost + color_cost + state0.cost +
                                         state1.similarity_cost);
              if (min_cost < state.cost) {
                state.cost = min_cost;
                state.instruction =
                    std::make_shared<HorizontalCutInstruction>("", ys[iy]);
                state.children.clear();
                state.children.push_back(&state0);
                state.children.push_back(&state1);
              }
            }
            // Try vertical cut
            for (int ix = ix0 + 1; ix < ix1; ++ix) {
              auto&& state0 = states[iy0][iy1][ix0][ix];
              auto&& state1 = states[iy0][iy1][ix][ix1];
              double min_cost = state.cost;
              min_cost =
                  std::min(min_cost, cut_cost + state0.cost + state1.cost);
              min_cost =
                  std::min(min_cost, cut_cost + color_cost +
                                         state0.similarity_cost + state1.cost);
              min_cost =
                  std::min(min_cost, cut_cost + color_cost + state0.cost +
                                         state1.similarity_cost);
              if (min_cost < state.cost) {
                state.cost = min_cost;
                state.instruction =
                    std::make_shared<VerticalCutInstruction>("", xs[ix]);
                state.children.clear();
                state.children.push_back(&state0);
                state.children.push_back(&state1);
              }
            }
            // TODO: Try point cut
          }
        }
      }
    }

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
};

REGISTER_SOLVER_WITH_OPTION("DpSolver", DpSolver, DpSolver::Option);
// vim:ts=2 sw=2 sts=2 et ci
