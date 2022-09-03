#include "stdafx.h"
#include "solver_registry.h"
#include "instruction.h"
#include <array>
#include <fmt/core.h>

template <typename T, typename... Args>
auto CreateVector(std::size_t count, Args&&... args) {
  if constexpr (sizeof...(args) > 1) {
    return std::vector(count, CreateVector<T>(args...));
  } else {
    return std::vector<T>(count, args...);
  }
}

class IntervalDPSolver2 : public SolverBase {
public:
  struct Option : public OptionBase {
    int num_intervals = 10;
    void setOptionParser(CLI::App* app) override {
      app->add_option("--interval-dp-2-num-intervals", num_intervals);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  IntervalDPSolver2() { }
  SolverOutputs solve(const SolverArguments &args) override {
    const int num_intervals = getOption<Option>()->num_intervals;
    LOG(INFO) << "num_intervals = " << num_intervals;
    const int height = args.painting->height;
    const int width = args.painting->width;
    const auto get_value = [&, frame=args.painting->frame](int y, int x, int c) {
      return frame[x + y * width][c];
    };

    // グリッドに分割する
    // とりあえず縦横とも等分する。境界を探して設定すればより良くなるはず
    std::vector<int> yticks;
    for (int y = 0; y < height; y += height / num_intervals) {
      yticks.push_back(y);
    }
    yticks.push_back(height);
    std::vector<int> xticks;
    for (int x = 0; x < width; x += width / num_intervals) {
      xticks.push_back(x);
    }
    xticks.push_back(width);
    const int H = yticks.size() - 1;
    const int W = xticks.size() - 1;

    // グリッドに沿った長方形を color move で平均色に塗ったときのコストを求める
    // move のコストと similarity のコストを両方足しておく
    // O(height * width * (HW)^2)
    constexpr double kAlpha = 0.005;
    auto colors = CreateVector<Color>(H, H + 1, W, W + 1, Color());
    auto color_costs = CreateVector<double>(H, H + 1, W, W + 1, std::numeric_limits<double>::infinity());

    for (int b = 0; b < H; ++b) {
      for (int t = b + 1; t <= H; ++t) {
        for (int l = 0; l < W; ++l) {
          for (int r = l + 1; r <= W; ++r) {
            colors[b][t][l][r] = geometricMedianColor(*args.painting, Point(xticks[l], yticks[b]), Point(xticks[r], yticks[t]), 10).value();
            double cost = 0.0;
            for (int y = yticks[b]; y < yticks[t]; ++y) {
              for (int x = xticks[l]; x < xticks[r]; ++x) {
                double distance2 = 0.0;
                for (int c = 0; c < 4; ++c) {
                  const int diff = get_value(y, x, c) - colors[b][t][l][r][c];
                  distance2 += diff * diff;
                }
                cost += std::sqrt(distance2);
              }
            }
            color_costs[b][t][l][r] = kAlpha * cost;
            color_costs[b][t][l][r] += std::round(5.0 * height * width / ((yticks[t] - yticks[b]) * (xticks[r] - xticks[l])));
          }
        }
      }
    }

    // Line cut move で分割する手順を区間 DP で求める
    // O((H+W) * (HW)^2)
    auto best_divisions = CreateVector<int>(H, H + 1, W, W + 1, 2, -1);
    auto best_costs = CreateVector<double>(H, H + 1, W, W + 1, std::numeric_limits<double>::infinity());
    for (int h = 1; h <= H; ++h) {
      for (int w = 1; w <= W; ++w) {
        for (int b = 0; b + h <= H; ++b) {
          const int t = b + h;
          for (int l = 0; l + w <= W; ++l) {
            const int r = l + w;
            auto& best_cost = best_costs[b][t][l][r];
            best_cost = color_costs[b][t][l][r];
            const int cut_cost = std::round(7.0 * height * width / ((yticks[t] - yticks[b]) * (xticks[r] - xticks[l])));
            for (int m = b + 1; m < t; ++m) {
              const double cost = cut_cost + best_costs[b][m][l][r] + best_costs[m][t][l][r];
              if (cost < best_cost) {
                best_cost = cost;
                best_divisions[b][t][l][r] = {m, -1};
              }
            }
            for (int m = l + 1; m < r; ++m) {
              const double cost = cut_cost + best_costs[b][t][l][m] + best_costs[b][t][m][r];
              if (cost < best_cost) {
                best_cost = cost;
                best_divisions[b][t][l][r] = {-1, m};
              }
            }
          }
        }
      }
    }
    LOG(INFO) << fmt::format("best cost = {0}", best_costs[0][H][0][W]);

    // 操作列を構築する
    SolverOutputs ret;
    const auto add_comment = [&](const std::string& comment) {
      ret.solution.push_back(std::make_shared<CommentInstruction>(comment));
    };
    add_comment(fmt::format("cost = {0}", static_cast<int>(std::round(best_costs[0][H][0][W]))));
    add_comment(fmt::format("num_intervals = {0}", num_intervals));
    std::vector<std::tuple<int, int, int, int, std::string>> stack;
    stack.emplace_back(0, H, 0, W, "0");
    while (!stack.empty()) {
      const auto [b, t, l, r, name] = stack.back();
      stack.pop_back();
      const auto& division = best_divisions[b][t][l][r];
      if (division[0] >= 0) {
        ret.solution.push_back(std::make_shared<HorizontalCutInstruction>(name, yticks[division[0]]));
        stack.emplace_back(b, division[0], l, r, name + ".0");
        stack.emplace_back(division[0], t, l, r, name + ".1");
      } else if (division[1] >= 0) {
        ret.solution.push_back(std::make_shared<VerticalCutInstruction>(name, yticks[division[1]]));
        stack.emplace_back(b, t, l, division[1], name + ".0");
        stack.emplace_back(b, t, division[1], r, name + ".1");
      } else {
        ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[b][t][l][r]));
      }
    }

    return ret;
  }
};

REGISTER_SOLVER_WITH_OPTION("IntervalDPSolver2", IntervalDPSolver2, IntervalDPSolver2::Option);
// vim:ts=2 sw=2 sts=2 et ci
