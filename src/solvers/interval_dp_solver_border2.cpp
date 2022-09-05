#include "stdafx.h"
#include <thread>
#include <queue>
#include "solver_registry.h"
#include <fmt/format.h>
#include "instruction.h"
#include "instruction_cost_calculator.h"
#include "similarity_checker.h";
#include <opencv2/opencv.hpp>
#include <omp.h>
#include "edge_detect.h"




#if 1

template <typename T, typename... Args>
auto CreateVector(std::size_t count, Args&&... args) {
  if constexpr (sizeof...(args) > 1) {
    return std::vector(count, CreateVector<T>(args...));
  }
  else {
    return std::vector<T>(count, args...);
  }
}

void AddPrefix(std::vector<std::shared_ptr<Instruction>>& solution, const std::string& prefix) {
  for (auto& e : solution) {
    assert(e);
    assert(e->GetBlockID().size());
    auto previous_name = e->GetBlockID();
    auto new_name = prefix + previous_name;
    LOG(INFO) << fmt::format("prefix is {}, previous_name is {}, new_name is {}", prefix, previous_name, new_name);
    e->SetBlockID(new_name);
  }
}



using DfsRet = std::pair<double, std::vector<std::shared_ptr<Instruction>>>;
using DfsRetPtr = std::shared_ptr<DfsRet>;
DfsRetPtr SolveDfs(const Point& bottomLeft, const Point& topRight, const EdgeDetect& edge, PaintingPtr painting, const std::string& id, bool allow_point_cut = false) {
  std::vector<int> yticks_tmp, xticks_tmp;
  const int height = painting->height;
  const int width = painting->width;
  int height_tmp = topRight.py - bottomLeft.py;
  int width_tmp = topRight.px - bottomLeft.px;
  int max_tick = 15;
  yticks_tmp = edge.YTicks(bottomLeft, topRight, height_tmp * 0.75 / (max_tick - 1), max_tick);
  xticks_tmp = edge.XTicks(bottomLeft, topRight, width_tmp * 0.75 / (max_tick - 1), max_tick);
  LOG(INFO) << fmt::format("height_tmp = {},  width_tmp = {}. yticks size is {}, xticks size is {}", height_tmp, width_tmp, yticks_tmp.size(), xticks_tmp.size());
  const auto get_value = [&, frame = painting->frame](int y, int x, int c) {
    return frame[x + y * width][c];
  };
  constexpr double kAlpha = 0.005;

  bool last = (height_tmp <= max_tick * 2) || (width_tmp <= max_tick * 2);
  

  //for (int y = yticks[bb]; y < yticks[bb + 1]; ++y) yticks_tmp.push_back(y);
  //for (int x = xticks[ll]; x < xticks[ll + 1]; ++x) xticks_tmp.push_back(x);
  const int H_tmp = yticks_tmp.size() - 1;
  const int W_tmp = xticks_tmp.size() - 1;
  auto colors_tmp = CreateVector<Color>(H_tmp, H_tmp + 1, W_tmp, W_tmp + 1, Color());
  auto color_costs_tmp = CreateVector<double>(H_tmp, H_tmp + 1, W_tmp, W_tmp + 1, std::numeric_limits<double>::infinity());
  auto similarity_costs_tmp = CreateVector<double>(H_tmp, H_tmp + 1, W_tmp, W_tmp + 1, std::numeric_limits<double>::infinity());

  for (int btlr = 0; btlr < H_tmp * (H_tmp + 1) * W_tmp * (W_tmp + 1); ++btlr) {
    const int b = btlr / (W_tmp + 1) / W_tmp / (H_tmp + 1);
    const int t = btlr / (W_tmp + 1) / W_tmp % (H_tmp + 1);
    const int l = btlr / (W_tmp + 1) % W_tmp;
    const int r = btlr % (W_tmp + 1);
    if (b + 1 <= t && t <= H_tmp && l + 1 <= r && r <= W_tmp) {
      const double multiplier = 1.0 * height * width / ((yticks_tmp[t] - yticks_tmp[b]) * (xticks_tmp[r] - xticks_tmp[l]));
      Point bottomLeft_itr = Point(xticks_tmp[l], yticks_tmp[b]), topRight_itr = Point(xticks_tmp[r], yticks_tmp[t]);
      colors_tmp[b][t][l][r] = geometricMedianColor(*painting,bottomLeft_itr, topRight_itr, false, 10).value();
      double cost = 0.0;
      for (int y = yticks_tmp[b]; y < yticks_tmp[t]; ++y) {
        for (int x = xticks_tmp[l]; x < xticks_tmp[r]; ++x) {
          double distance2 = 0.0;
          for (int c = 0; c < 4; ++c) {
            const int diff = get_value(y, x, c) - colors_tmp[b][t][l][r][c];
            distance2 += diff * diff;
          }
          cost += std::sqrt(distance2);
        }
      }
      similarity_costs_tmp[b][t][l][r] = kAlpha * cost;
      color_costs_tmp[b][t][l][r] = kAlpha * cost;
      color_costs_tmp[b][t][l][r] += std::round(5.0 * multiplier);
    }
  }

  auto dfs_memo = CreateVector<DfsRetPtr>(H_tmp + 1, W_tmp + 1, nullptr);
  if(!last) for (int b = 0; b < H_tmp; ++b) for (int l = 0; l < W_tmp; ++l) dfs_memo[b][l] = SolveDfs(Point(xticks_tmp[l], yticks_tmp[b]), Point(xticks_tmp[l + 1], yticks_tmp[b + 1]), edge, painting, "", allow_point_cut);

  auto best_divisions_tmp = CreateVector<int>(H_tmp, H_tmp + 1, W_tmp, W_tmp + 1, 3, -1);
  auto best_costs_tmp = CreateVector<double>(H_tmp, H_tmp + 1, W_tmp, W_tmp + 1, std::numeric_limits<double>::infinity());
  for (int h = 1; h <= H_tmp; ++h) {
    for (int w = 1; w <= W_tmp; ++w) {
      for (int b = 0; b + h <= H_tmp; ++b) {
        const int t = b + h;
        for (int l = 0; l + w <= W_tmp; ++l) {
          const int r = l + w;
          auto& best_cost = best_costs_tmp[b][t][l][r];
          best_cost = color_costs_tmp[b][t][l][r];
          if (!last && t == b + 1 && r == l + 1) best_cost = dfs_memo[b][l]->first;
          const int point_cut_cost = std::round(10.0 * height * width / ((yticks_tmp[t] - yticks_tmp[b]) * (xticks_tmp[r] - xticks_tmp[l])));
          const int cut_cost = std::round(7.0 * height * width / ((yticks_tmp[t] - yticks_tmp[b]) * (xticks_tmp[r] - xticks_tmp[l])));
          const int color_cost = std::round(5.0 * height * width / ((yticks_tmp[t] - yticks_tmp[b]) * (xticks_tmp[r] - xticks_tmp[l])));
          if (allow_point_cut) {
            for (int my = b + 1; my < t; ++my) {
              for (int mx = l + 1; mx < r; ++mx) {
                { // Point cut してから再帰的に処理する
                  const double cost = point_cut_cost + best_costs_tmp[b][my][l][mx] + best_costs_tmp[b][my][mx][r] + best_costs_tmp[my][t][l][mx] + best_costs_tmp[my][t][mx][r];
                  if (cost < best_cost) {
                    best_cost = cost;
                    best_divisions_tmp[b][t][l][r] = { my, mx, -1 };
                  }
                }
                { // .0の色に塗ってから...
                  const double cost = color_cost + point_cut_cost + similarity_costs_tmp[b][my][l][mx] + best_costs_tmp[b][my][mx][r] + best_costs_tmp[my][t][l][mx] + best_costs_tmp[my][t][mx][r];
                  if (cost < best_cost) {
                    best_cost = cost;
                    best_divisions_tmp[b][t][l][r] = { my, mx, 0 };
                  }
                }
                { // .1の色に塗ってから...
                  const double cost = color_cost + point_cut_cost + best_costs_tmp[b][my][l][mx] + similarity_costs_tmp[b][my][mx][r] + best_costs_tmp[my][t][l][mx] + best_costs_tmp[my][t][mx][r];
                  if (cost < best_cost) {
                    best_cost = cost;
                    best_divisions_tmp[b][t][l][r] = { my, mx, 1 };
                  }
                }
                { // .3の色に塗ってから...
                  const double cost = color_cost + point_cut_cost + best_costs_tmp[b][my][l][mx] + best_costs_tmp[b][my][mx][r] + similarity_costs_tmp[my][t][l][mx] + best_costs_tmp[my][t][mx][r];
                  if (cost < best_cost) {
                    best_cost = cost;
                    best_divisions_tmp[b][t][l][r] = { my, mx, 3 };
                  }
                }
                { // .2の色に塗ってから...
                  const double cost = color_cost + point_cut_cost + best_costs_tmp[b][my][l][mx] + best_costs_tmp[b][my][mx][r] + best_costs_tmp[my][t][l][mx] + similarity_costs_tmp[my][t][mx][r];
                  if (cost < best_cost) {
                    best_cost = cost;
                    best_divisions_tmp[b][t][l][r] = { my, mx, 2 };
                  }
                }
              }
            }
          }
          for (int m = b + 1; m < t; ++m) {
            { // Line cut してから上下を再帰的に処理する
              const double cost = cut_cost + best_costs_tmp[b][m][l][r] + best_costs_tmp[m][t][l][r];
              if (cost < best_cost) {
                best_cost = cost;
                best_divisions_tmp[b][t][l][r] = { m, -1, -1 };
              }
            }
            { // 下の block の色に塗ってから line cut し、上の block だけ再帰的に処理する
              const double cost = color_cost + cut_cost + similarity_costs_tmp[b][m][l][r] + best_costs_tmp[m][t][l][r];
              if (cost < best_cost) {
                best_cost = cost;
                best_divisions_tmp[b][t][l][r] = { m, -1, 0 };
              }
            }
            { // 上の block の色に塗ってから line cut し、下の block だけ再帰的に処理する
              const double cost = color_cost + cut_cost + best_costs_tmp[b][m][l][r] + similarity_costs_tmp[m][t][l][r];
              if (cost < best_cost) {
                best_cost = cost;
                best_divisions_tmp[b][t][l][r] = { m, -1, 1 };
              }
            }
          }
          for (int m = l + 1; m < r; ++m) {
            { // Line cut してから左右を再帰的に処理する
              const double cost = cut_cost + best_costs_tmp[b][t][l][m] + best_costs_tmp[b][t][m][r];
              if (cost < best_cost) {
                best_cost = cost;
                best_divisions_tmp[b][t][l][r] = { -1, m, -1 };
              }
            }
            { // 左の block の色に塗ってから line cut し、右の block だけ再帰的に処理する
              const double cost = color_cost + cut_cost + similarity_costs_tmp[b][t][l][m] + best_costs_tmp[b][t][m][r];
              if (cost < best_cost) {
                best_cost = cost;
                best_divisions_tmp[b][t][l][r] = { -1, m, 0 };
              }
            }
            { // 右の block の色に塗ってから line cut し、左の block だけ再帰的に処理する
              const double cost = color_cost + cut_cost + best_costs_tmp[b][t][l][m] + similarity_costs_tmp[b][t][m][r];
              if (cost < best_cost) {
                best_cost = cost;
                best_divisions_tmp[b][t][l][r] = { -1, m, 1 };
              }
            }
          }
        }
      }
    }
  }
  std::vector<std::shared_ptr<Instruction>> solution;
    std::vector<std::tuple<int, int, int, int, std::string>> stack_tmp;
    stack_tmp.emplace_back(0, H_tmp, 0, W_tmp, id);
    while (!stack_tmp.empty()) {
      const auto [b, t, l, r, name] = stack_tmp.back();
      stack_tmp.pop_back();
      const auto& division = best_divisions_tmp[b][t][l][r];
      if (!last && t == b + 1 && r == l + 1) {
        auto child_solution = dfs_memo[b][l]->second;
        AddPrefix(child_solution, name);
        for (auto e : child_solution) solution.push_back(e);
      }
      else if (division[0] >= 0 && division[1] >= 0) {
        assert(allow_point_cut);
        if (division[2] == 0) {
          solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[b][division[0]][l][division[1]]));
          solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks_tmp[division[1]], yticks_tmp[division[0]])));
          stack_tmp.emplace_back(b, division[0], division[1], r, name + ".1");
          stack_tmp.emplace_back(division[0], t, l, division[1], name + ".3");
          stack_tmp.emplace_back(division[0], t, division[1], r, name + ".2");
        }
        else if (division[2] == 1) {
          solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[b][division[0]][division[1]][r]));
          solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks_tmp[division[1]], yticks_tmp[division[0]])));
          stack_tmp.emplace_back(b, division[0], l, division[1], name + ".0");
          stack_tmp.emplace_back(division[0], t, l, division[1], name + ".3");
          stack_tmp.emplace_back(division[0], t, division[1], r, name + ".2");
        }
        else if (division[2] == 3) {
          solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[division[0]][t][l][division[1]]));
          solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks_tmp[division[1]], yticks_tmp[division[0]])));
          stack_tmp.emplace_back(b, division[0], l, division[1], name + ".0");
          stack_tmp.emplace_back(b, division[0], division[1], r, name + ".1");
          stack_tmp.emplace_back(division[0], t, division[1], r, name + ".2");
        }
        else if (division[2] == 2) {
          solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[division[0]][t][division[1]][r]));
          solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks_tmp[division[1]], yticks_tmp[division[0]])));
          stack_tmp.emplace_back(b, division[0], l, division[1], name + ".0");
          stack_tmp.emplace_back(b, division[0], division[1], r, name + ".1");
          stack_tmp.emplace_back(division[0], t, l, division[1], name + ".3");
        }
        else {
          solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks_tmp[division[1]], yticks_tmp[division[0]])));
          stack_tmp.emplace_back(b, division[0], l, division[1], name + ".0");
          stack_tmp.emplace_back(b, division[0], division[1], r, name + ".1");
          stack_tmp.emplace_back(division[0], t, l, division[1], name + ".3");
          stack_tmp.emplace_back(division[0], t, division[1], r, name + ".2");
        }
      }
      else if (division[0] >= 0) {
        if (division[2] == 0) {
          solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[b][division[0]][l][r]));
          solution.push_back(std::make_shared<HorizontalCutInstruction>(name, yticks_tmp[division[0]]));
          stack_tmp.emplace_back(division[0], t, l, r, name + ".1");
        }
        else if (division[2] == 1) {
          solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[division[0]][t][l][r]));
          solution.push_back(std::make_shared<HorizontalCutInstruction>(name, yticks_tmp[division[0]]));
          stack_tmp.emplace_back(b, division[0], l, r, name + ".0");
        }
        else {
          solution.push_back(std::make_shared<HorizontalCutInstruction>(name, yticks_tmp[division[0]]));
          stack_tmp.emplace_back(b, division[0], l, r, name + ".0");
          stack_tmp.emplace_back(division[0], t, l, r, name + ".1");
        }
      }
      else if (division[1] >= 0) {
        if (division[2] == 0) {
          solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[b][t][l][division[1]]));
          solution.push_back(std::make_shared<VerticalCutInstruction>(name, xticks_tmp[division[1]]));
          stack_tmp.emplace_back(b, t, division[1], r, name + ".1");
        }
        else if (division[2] == 1) {
          solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[b][t][division[1]][r]));
          solution.push_back(std::make_shared<VerticalCutInstruction>(name, xticks_tmp[division[1]]));
          stack_tmp.emplace_back(b, t, l, division[1], name + ".0");
        }
        else {
          solution.push_back(std::make_shared<VerticalCutInstruction>(name, xticks_tmp[division[1]]));
          stack_tmp.emplace_back(b, t, l, division[1], name + ".0");
          stack_tmp.emplace_back(b, t, division[1], r, name + ".1");
        }
      }
      else {
        solution.push_back(std::make_shared<ColorInstruction>(name, colors_tmp[b][t][l][r]));
      }
  }
  return std::make_shared<DfsRet>(std::make_pair(best_costs_tmp[0][H_tmp][0][W_tmp], solution));
}




class IntervalDPSolverBorder2 : public SolverBase {
public:
  struct Option : public OptionBase {
    int num_intervals = 40;
    double prune_threshold = 8.0;
    bool allow_point_cut = false;
    bool inherit_ticks = false;
    int num_threads = 0; // 0: auto
    void setOptionParser(CLI::App* app) override {
      app->add_option("--interval-dp-border2-num-intervals", num_intervals);
      app->add_option("--interval-dp-border2-prune-threshold", prune_threshold);
      app->add_flag("--interval-dp-border2-allow-point-cut", allow_point_cut);
      app->add_flag("--interval-dp-border2-inherit-ticks", inherit_ticks);
      app->add_option("--interval-dp-border2-num-threads", num_threads);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

  IntervalDPSolverBorder2() { }
  SolverOutputs solve(const SolverArguments& args) override {
    const int num_intervals = getOption<Option>()->num_intervals;

    const double prune_threshold = getOption<Option>()->prune_threshold;
    const bool allow_point_cut = getOption<Option>()->allow_point_cut;
    const bool inherit_ticks = getOption<Option>()->inherit_ticks;
    int num_threads = getOption<Option>()->num_threads;
    if (num_threads <= 0) num_threads = omp_get_max_threads();
    LOG(INFO) << "num_intervals = " << num_intervals;
    LOG(INFO) << "prune_threshold = " << prune_threshold;
    LOG(INFO) << "allow_point_cut = " << allow_point_cut;
    LOG(INFO) << "inherit_ticks = " << inherit_ticks;
    LOG(INFO) << "num_threads = " << num_threads;
    const auto canvas = inherit_ticks ? args.initial_canvas : args.previous_canvas;
    const auto top_level_id = std::to_string(canvas->calcTopLevelId());
    assert(canvas->blocks[top_level_id]->size == canvas->size());
    const int height = args.painting->height;
    const int width = args.painting->width;
    auto bottomLeft = Point(0, 0), topRight = Point(width, height);
    const auto get_value = [&, frame = args.painting->frame](int y, int x, int c) {
      return frame[x + y * width][c];
    };

    auto edge = EdgeDetect(args.painting);
    auto dfs_result = SolveDfs(bottomLeft, topRight, edge, args.painting, "0");
    SolverOutputs ret;
    ret.solution = dfs_result->second;
    return ret;
  }
};

//REGISTER_SOLVER("IntervalDPSolverBorder", IntervalDPSolverBorder);
REGISTER_SOLVER_WITH_OPTION("IntervalDPSolverBorder2", IntervalDPSolverBorder2, IntervalDPSolverBorder2::Option);
// vim:ts=2 sw=2 sts=2 et ci

#endif
