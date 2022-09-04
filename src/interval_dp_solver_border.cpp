#include "stdafx.h"
#include <thread>
#include <queue>
#include <format>
#include "solver_registry.h"
#include <fmt/format.h>
#include "instruction.h"
#include "instruction_cost_calculator.h"
#include "similarity_checker.h";
#include <opencv2/opencv.hpp>
#include <ppl.h>
#include <omp.h>
#include "edge_detect.h"


#if 0
std::vector<SimpleBlock> PointCut(const SimpleBlock& block, const Point& point) {
    std::vector<SimpleBlock> ret;
    ret.push_back(SimpleBlock(block.id + ".0", block.bottomLeft, point, block.color));
    ret.push_back(SimpleBlock(block.id + ".1", Point(point.px, block.bottomLeft.py), Point(block.topRight.px, point.py), block.color));
    ret.push_back(SimpleBlock(block.id + ".2", point, block.topRight, block.color));
    ret.push_back(SimpleBlock(block.id + ".3", Point(block.bottomLeft.px, point.py), Point(point.px, block.topRight.py), block.color));
    return ret;
}

std::vector<SimpleBlock> VerticalCut(const SimpleBlock& block, int px) {
    assert(block.bottomLeft.px < px);
    assert(px < block.topRight.px);
    std::vector<SimpleBlock> children;
    children.push_back(SimpleBlock(block.id + ".0", block.bottomLeft, Point(px, block.topRight.py), block.color));
    children.push_back(SimpleBlock(block.id + ".1", Point(px, block.bottomLeft.py), block.topRight, block.color));
    return children;
}
std::vector<SimpleBlock> HorizontalCut(const SimpleBlock& block, int py) {
    assert(block.bottomLeft.py < py);
    assert(py < block.topRight.py);
    std::vector<SimpleBlock> children;
    children.push_back(SimpleBlock(block.id + ".0", block.bottomLeft, Point(block.topRight.px, py), block.color));
    children.push_back(SimpleBlock(block.id + ".1", Point(block.bottomLeft.px, py), block.topRight, block.color));
    return children;

}

#endif


class DebugEdgeDetect : public SolverBase {
public:
    DebugEdgeDetect() { }
    SolverOutputs solve(const SolverArguments& args) override {
        SolverOutputs ret;
        ret.solution = args.optional_initial_solution;
        auto cv = EdgeDetect(args.painting);
        cv.Show();
        return ret;
    }
};
REGISTER_SOLVER("DebugEdgeDetect", DebugEdgeDetect);

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


//Solverの大半はinterval_dp_solver3からコピペ、xticks, yticksの選定箇所のみ変更
class IntervalDPSolverBorder : public SolverBase {
public:
    struct Option : public OptionBase {
        int num_intervals = 10;
        double prune_threshold = 8.0;
        bool allow_point_cut = false;
        bool inherit_ticks = false;
        int num_threads = 0; // 0: auto
        void setOptionParser(CLI::App* app) override {
            app->add_option("--interval-dp-border-num-intervals", num_intervals);
            app->add_option("--interval-dp-border-prune-threshold", prune_threshold);
            app->add_flag("--interval-dp-border-allow-point-cut", allow_point_cut);
            app->add_flag("--interval-dp-border-inherit-ticks", inherit_ticks);
            app->add_option("--interval-dp-border-num-threads", num_threads);
        }
    };
    virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

    IntervalDPSolverBorder() { }
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
        const auto get_value = [&, frame = args.painting->frame](int y, int x, int c) {
            return frame[x + y * width][c];
        };

        // グリッドに分割する
        std::vector<int> yticks, xticks;
        if (inherit_ticks) {
            // 既存解で使われた境界を候補にする
            std::set<int> yticks_set = { 0, height };
            std::set<int> xticks_set = { 0, width };
            for (const auto& instruction : args.optional_initial_solution) {
                if (const auto hcut = std::dynamic_pointer_cast<HorizontalCutInstruction>(instruction)) {
                    yticks_set.insert(hcut->lineNumber);
                }
                if (const auto vcut = std::dynamic_pointer_cast<VerticalCutInstruction>(instruction)) {
                    xticks_set.insert(vcut->lineNumber);
                }
            }
            yticks.insert(yticks.end(), yticks_set.begin(), yticks_set.end());
            xticks.insert(xticks.end(), xticks_set.begin(), xticks_set.end());
        }
        else {
            auto edge = EdgeDetect(args.painting, args.painting->width / (num_intervals * 0.75), num_intervals * 0.75);
            xticks = edge.XTicks();
            yticks = edge.YTicks();
        }
        const int H = yticks.size() - 1;
        const int W = xticks.size() - 1;
        LOG(INFO) << "H = " << H << ", W = " << W;

        // グリッドに沿った長方形を color move で平均色に塗ったときのコストを求める
        // move のコストと similarity のコストを両方足しておく
        // O(height * width * (HW)^2)
        constexpr double kAlpha = 0.005;
        auto colors = CreateVector<Color>(H, H + 1, W, W + 1, Color());
        auto color_costs = CreateVector<double>(H, H + 1, W, W + 1, std::numeric_limits<double>::infinity());
        auto similarity_costs = CreateVector<double>(H, H + 1, W, W + 1, std::numeric_limits<double>::infinity());

#pragma omp parallel num_threads(num_threads) if(num_threads > 1)
        {
#pragma omp for
            for (int b = 0; b < H; ++b) {
                for (int t = b + 1; t <= H; ++t) {
                    for (int l = 0; l < W; ++l) {
                        for (int r = l + 1; r <= W; ++r) {
                            const double multiplier = 1.0 * height * width / ((yticks[t] - yticks[b]) * (xticks[r] - xticks[l]));
                            if (multiplier < prune_threshold) continue;  // 面積が大きい領域は、 geometricMedianColor() が重い && 単色でまとめて塗る可能性が低いので枝刈りする

                            colors[b][t][l][r] = geometricMedianColor(*args.painting, Point(xticks[l], yticks[b]), Point(xticks[r], yticks[t]), false, 10).value();
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
                            similarity_costs[b][t][l][r] = kAlpha * cost;
                            color_costs[b][t][l][r] = kAlpha * cost;
                            color_costs[b][t][l][r] += std::round(5.0 * multiplier);
                        }
                    }
                }
            }
        }

        // Line cut move で分割する手順を区間 DP で求める
        // O((H+W) * (HW)^2)
        auto best_divisions = CreateVector<int>(H, H + 1, W, W + 1, 3, -1);
        auto best_costs = CreateVector<double>(H, H + 1, W, W + 1, std::numeric_limits<double>::infinity());
        for (int h = 1; h <= H; ++h) {
            for (int w = 1; w <= W; ++w) {
                for (int b = 0; b + h <= H; ++b) {
                    const int t = b + h;
                    for (int l = 0; l + w <= W; ++l) {
                        const int r = l + w;
                        auto& best_cost = best_costs[b][t][l][r];
                        best_cost = color_costs[b][t][l][r];
                        const int point_cut_cost = std::round(10.0 * height * width / ((yticks[t] - yticks[b]) * (xticks[r] - xticks[l])));
                        const int cut_cost = std::round(7.0 * height * width / ((yticks[t] - yticks[b]) * (xticks[r] - xticks[l])));
                        const int color_cost = std::round(5.0 * height * width / ((yticks[t] - yticks[b]) * (xticks[r] - xticks[l])));
                        if (allow_point_cut) {
                            for (int my = b + 1; my < t; ++my) {
                                for (int mx = l + 1; mx < r; ++mx) {
                                    { // Point cut してから再帰的に処理する
                                        const double cost = point_cut_cost + best_costs[b][my][l][mx] + best_costs[b][my][mx][r] + best_costs[my][t][l][mx] + best_costs[my][t][mx][r];
                                        if (cost < best_cost) {
                                            best_cost = cost;
                                            best_divisions[b][t][l][r] = { my, mx, -1 };
                                        }
                                    }
                                    { // .0の色に塗ってから...
                                        const double cost = color_cost + point_cut_cost + similarity_costs[b][my][l][mx] + best_costs[b][my][mx][r] + best_costs[my][t][l][mx] + best_costs[my][t][mx][r];
                                        if (cost < best_cost) {
                                            best_cost = cost;
                                            best_divisions[b][t][l][r] = { my, mx, 0 };
                                        }
                                    }
                                    { // .1の色に塗ってから...
                                        const double cost = color_cost + point_cut_cost + best_costs[b][my][l][mx] + similarity_costs[b][my][mx][r] + best_costs[my][t][l][mx] + best_costs[my][t][mx][r];
                                        if (cost < best_cost) {
                                            best_cost = cost;
                                            best_divisions[b][t][l][r] = { my, mx, 1 };
                                        }
                                    }
                                    { // .3の色に塗ってから...
                                        const double cost = color_cost + point_cut_cost + best_costs[b][my][l][mx] + best_costs[b][my][mx][r] + similarity_costs[my][t][l][mx] + best_costs[my][t][mx][r];
                                        if (cost < best_cost) {
                                            best_cost = cost;
                                            best_divisions[b][t][l][r] = { my, mx, 3 };
                                        }
                                    }
                                    { // .2の色に塗ってから...
                                        const double cost = color_cost + point_cut_cost + best_costs[b][my][l][mx] + best_costs[b][my][mx][r] + best_costs[my][t][l][mx] + similarity_costs[my][t][mx][r];
                                        if (cost < best_cost) {
                                            best_cost = cost;
                                            best_divisions[b][t][l][r] = { my, mx, 2 };
                                        }
                                    }
                                }
                            }
                        }
                        for (int m = b + 1; m < t; ++m) {
                            { // Line cut してから上下を再帰的に処理する
                                const double cost = cut_cost + best_costs[b][m][l][r] + best_costs[m][t][l][r];
                                if (cost < best_cost) {
                                    best_cost = cost;
                                    best_divisions[b][t][l][r] = { m, -1, -1 };
                                }
                            }
                            { // 下の block の色に塗ってから line cut し、上の block だけ再帰的に処理する
                                const double cost = color_cost + cut_cost + similarity_costs[b][m][l][r] + best_costs[m][t][l][r];
                                if (cost < best_cost) {
                                    best_cost = cost;
                                    best_divisions[b][t][l][r] = { m, -1, 0 };
                                }
                            }
                            { // 上の block の色に塗ってから line cut し、下の block だけ再帰的に処理する
                                const double cost = color_cost + cut_cost + best_costs[b][m][l][r] + similarity_costs[m][t][l][r];
                                if (cost < best_cost) {
                                    best_cost = cost;
                                    best_divisions[b][t][l][r] = { m, -1, 1 };
                                }
                            }
                        }
                        for (int m = l + 1; m < r; ++m) {
                            { // Line cut してから左右を再帰的に処理する
                                const double cost = cut_cost + best_costs[b][t][l][m] + best_costs[b][t][m][r];
                                if (cost < best_cost) {
                                    best_cost = cost;
                                    best_divisions[b][t][l][r] = { -1, m, -1 };
                                }
                            }
                            { // 左の block の色に塗ってから line cut し、右の block だけ再帰的に処理する
                                const double cost = color_cost + cut_cost + similarity_costs[b][t][l][m] + best_costs[b][t][m][r];
                                if (cost < best_cost) {
                                    best_cost = cost;
                                    best_divisions[b][t][l][r] = { -1, m, 0 };
                                }
                            }
                            { // 右の block の色に塗ってから line cut し、左の block だけ再帰的に処理する
                                const double cost = color_cost + cut_cost + best_costs[b][t][l][m] + similarity_costs[b][t][m][r];
                                if (cost < best_cost) {
                                    best_cost = cost;
                                    best_divisions[b][t][l][r] = { -1, m, 1 };
                                }
                            }
                        }
                    }
                }
            }
        }
        LOG(INFO) << fmt::format("best cost = {0}", best_costs[0][H][0][W]);

        // 操作列を構築する
        SolverOutputs ret;
        if (!inherit_ticks) {
            ret.solution = args.optional_initial_solution;
        }
        const auto add_comment = [&](const std::string& comment) {
            ret.solution.push_back(std::make_shared<CommentInstruction>(comment));
        };
        add_comment(fmt::format("cost = {0}", static_cast<int>(std::round(best_costs[0][H][0][W]))));
        if (!inherit_ticks) {
            add_comment(fmt::format("num_intervals = {0}", num_intervals));
        }
        std::vector<std::tuple<int, int, int, int, std::string>> stack;
        stack.emplace_back(0, H, 0, W, top_level_id);
        while (!stack.empty()) {
            const auto [b, t, l, r, name] = stack.back();
            stack.pop_back();
            const auto& division = best_divisions[b][t][l][r];
            if (division[0] >= 0 && division[1] >= 0) {
                assert(allow_point_cut);
                if (division[2] == 0) {
                    ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[b][division[0]][l][division[1]]));
                    ret.solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks[division[1]], yticks[division[0]])));
                    stack.emplace_back(b, division[0], division[1], r, name + ".1");
                    stack.emplace_back(division[0], t, l, division[1], name + ".3");
                    stack.emplace_back(division[0], t, division[1], r, name + ".2");
                }
                else if (division[2] == 1) {
                    ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[b][division[0]][division[1]][r]));
                    ret.solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks[division[1]], yticks[division[0]])));
                    stack.emplace_back(b, division[0], l, division[1], name + ".0");
                    stack.emplace_back(division[0], t, l, division[1], name + ".3");
                    stack.emplace_back(division[0], t, division[1], r, name + ".2");
                }
                else if (division[2] == 3) {
                    ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[division[0]][t][l][division[1]]));
                    ret.solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks[division[1]], yticks[division[0]])));
                    stack.emplace_back(b, division[0], l, division[1], name + ".0");
                    stack.emplace_back(b, division[0], division[1], r, name + ".1");
                    stack.emplace_back(division[0], t, division[1], r, name + ".2");
                }
                else if (division[2] == 2) {
                    ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[division[0]][t][division[1]][r]));
                    ret.solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks[division[1]], yticks[division[0]])));
                    stack.emplace_back(b, division[0], l, division[1], name + ".0");
                    stack.emplace_back(b, division[0], division[1], r, name + ".1");
                    stack.emplace_back(division[0], t, l, division[1], name + ".3");
                }
                else {
                    ret.solution.push_back(std::make_shared<PointCutInstruction>(name, Point(xticks[division[1]], yticks[division[0]])));
                    stack.emplace_back(b, division[0], l, division[1], name + ".0");
                    stack.emplace_back(b, division[0], division[1], r, name + ".1");
                    stack.emplace_back(division[0], t, l, division[1], name + ".3");
                    stack.emplace_back(division[0], t, division[1], r, name + ".2");
                }
            }
            else if (division[0] >= 0) {
                if (division[2] == 0) {
                    ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[b][division[0]][l][r]));
                    ret.solution.push_back(std::make_shared<HorizontalCutInstruction>(name, yticks[division[0]]));
                    stack.emplace_back(division[0], t, l, r, name + ".1");
                }
                else if (division[2] == 1) {
                    ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[division[0]][t][l][r]));
                    ret.solution.push_back(std::make_shared<HorizontalCutInstruction>(name, yticks[division[0]]));
                    stack.emplace_back(b, division[0], l, r, name + ".0");
                }
                else {
                    ret.solution.push_back(std::make_shared<HorizontalCutInstruction>(name, yticks[division[0]]));
                    stack.emplace_back(b, division[0], l, r, name + ".0");
                    stack.emplace_back(division[0], t, l, r, name + ".1");
                }
            }
            else if (division[1] >= 0) {
                if (division[2] == 0) {
                    ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[b][t][l][division[1]]));
                    ret.solution.push_back(std::make_shared<VerticalCutInstruction>(name, xticks[division[1]]));
                    stack.emplace_back(b, t, division[1], r, name + ".1");
                }
                else if (division[2] == 1) {
                    ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[b][t][division[1]][r]));
                    ret.solution.push_back(std::make_shared<VerticalCutInstruction>(name, xticks[division[1]]));
                    stack.emplace_back(b, t, l, division[1], name + ".0");
                }
                else {
                    ret.solution.push_back(std::make_shared<VerticalCutInstruction>(name, xticks[division[1]]));
                    stack.emplace_back(b, t, l, division[1], name + ".0");
                    stack.emplace_back(b, t, division[1], r, name + ".1");
                }
            }
            else {
                ret.solution.push_back(std::make_shared<ColorInstruction>(name, colors[b][t][l][r]));
            }
        }

        return ret;
    }
};

//REGISTER_SOLVER("IntervalDPSolverBorder", IntervalDPSolverBorder);
REGISTER_SOLVER_WITH_OPTION("IntervalDPSolverBorder", IntervalDPSolverBorder, IntervalDPSolverBorder::Option);
// vim:ts=2 sw=2 sts=2 et ci

#endif
