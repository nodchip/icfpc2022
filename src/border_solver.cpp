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

struct EdgeDetect {
    std::mt19937_64 rand;
    PaintingPtr painting;
    int tick_size;
    int minimum_interval;
    cv::Mat color[4];
    cv::Mat after_process[4];
    cv::Mat after_soberX[4];
    cv::Mat after_soberY[4];
    cv::Mat border_mapX, border_mapY;
    void process(int i) {
        cv::GaussianBlur(color[i], after_process[i], cv::Size(5, 5), 5);
        cv::Sobel(after_process[i], after_soberX[i], -1, 1, 0);
        cv::Sobel(after_process[i], after_soberY[i], -1, 0, 1);
        //cv::Laplacian(after_process[i].clone(), after_process[i], -1, 5);
    }

    EdgeDetect(PaintingPtr painting_, int tick_size_ = 20, int minimum_interval_ = 7) : rand(0), painting(painting_), tick_size(tick_size_), minimum_interval(minimum_interval_) {
        border_mapX = cv::Mat::zeros(painting->width, painting->height, CV_8UC1);
        border_mapY = cv::Mat::zeros(painting->width, painting->height, CV_8UC1);
        auto interpret = [&](int i) {
            color[i] = cv::Mat(painting->width, painting->height, CV_8UC1);
            for (int y = 0; y < painting->height; ++y) for (int x = 0; x < painting->width; ++x) {
                color[i].at<uchar>(x, y) = painting->operator()(x, y)[i];
            }
            after_process[i] = cv::Mat(painting->width, painting->height, CV_8UC1);
            after_soberX[i] = cv::Mat(painting->width, painting->height, CV_8UC1);
            after_soberY[i] = cv::Mat(painting->width, painting->height, CV_8UC1);
        };
        for (int i = 0; i < 4; ++i) interpret(i);
        for (int i = 0; i < 4; ++i) process(i);

        //concurrency::parallel_for(0, 4, interpret);
        //concurrency::parallel_for(0, 4, [this](int i) {process(i); });
        for (int i = 0; i < 4; ++i) {
            border_mapX += 0.2 * after_soberX[i];
            border_mapY += 0.2 * after_soberY[i];
        }
#if 0
        border_culmative.resize(painting->width * painting->height + 1, 0);
        for (int y = 0; y < painting->height; ++y) for (int x = 0; x < painting->width; ++x) {
            int idx = y * painting->width + x;
            auto val = double(border_mapX.at<uchar>(x, y));
            val = (val * val) / 30;
            border_culmative[idx + 1] = border_culmative[idx] + int(val);
            //if(y<2) LOG(INFO) << fmt::format("idx is {}, border_culmative is {}, border_map is {}", idx, border_culmative[idx], val);
            assert(border_culmative[idx + 1] >= 0);
        }
#endif

    }

    std::vector<int> YTicks(bool sort = true) {
        std::vector<std::pair<int, int>> tmp;
        for (int y = 0; y < painting->height; ++y) {
            int counter = 0;
            for (int x = 0; x < painting->width; ++x) {
                counter += border_mapX.at<uchar>(x, y);
            }
            tmp.push_back(std::make_pair(counter, y));
        }
        std::sort(tmp.begin(), tmp.end());
        std::reverse(tmp.begin(), tmp.end());
        std::vector<int> ret;
        std::vector<bool> used(painting->height, false);
        for (int i = 0; i < tmp.size(); ++i) {
            auto p = tmp[i].second;
            if (used[p]) continue;
            ret.push_back(p);
            for (int j = std::max(int(0), p - minimum_interval); j < std::min(painting->height, p + minimum_interval); ++j) used[j] = true;
            if (ret.size() >= tick_size) break;
        }
        if (sort) std::sort(ret.begin(), ret.end());
        return ret;
    }


    std::vector<int> XTicks(bool sort = true) {
        std::vector<std::pair<int, int>> tmp;
        for (int x = 0; x < painting->width; ++x) {
            int counter = 0;
            for (int y = 0; y < painting->height; ++y) {
                counter += border_mapY.at<uchar>(x, y);
            }
            tmp.push_back(std::make_pair(counter, x));
        }
        std::sort(tmp.begin(), tmp.end());
        std::reverse(tmp.begin(), tmp.end());
        std::vector<bool> used(painting->height, false);
        std::vector<int> ret;
        for (int i = 0; i < tmp.size(); ++i) {
            auto p = tmp[i].second;
            if (used[p]) continue;
            ret.push_back(p);
            for (int j = std::max(int(0), p - minimum_interval); j < std::min(painting->width, p + minimum_interval); ++j) used[j] = true;
            if (ret.size() >= tick_size) break;
        }
        if (sort) std::sort(ret.begin(), ret.end());
        return ret;
    }




#if 0
    Point RandomPoint() {
        std::uniform_int_distribution<> randint(0, border_culmative.back() - 1);
        int seed = randint(rand);
        int idx = std::upper_bound(border_culmative.begin(), border_culmative.end(), seed) - border_culmative.begin() - 1;
        int x = idx % painting->width, y = idx / painting->width;
        LOG(INFO) << fmt::format("seed is {}, idx is {}, border_val is {}, map_val is {}", seed, idx, border_culmative[idx + 1] - border_culmative[idx], border_mapX.at<uchar>(x, y));
        return Point(x, y);
    }
#endif
    void Show() {
        cv::Mat debug(painting->width, painting->height, CV_8UC3);
        for (int y = 0; y < painting->height; ++y) for (int x = 0; x < painting->width; ++x) {
            auto val = border_mapX.at<uchar>(x, y);
            debug.at<cv::Vec3b>(x, y) = cv::Vec3b(val, val, val);
        }
        auto RED = cv::Scalar(0, 0, 255);
        auto GREEN = cv::Scalar(0, 255, 0);
        auto BLUE = cv::Scalar(255, 0, 0);
#if 0
        for (int i = 0; i < 500; ++i) {
            auto p = RandomPoint();
            //LOG(INFO) << fmt::format("p is {},{} : value is {}", p.px, p.py, border_map.at<uchar>(p.px,p.py));
            cv::circle(debug, cv::Point(p.py, p.px), 2, RED, -1);
        }
#endif
        for (auto e : YTicks()) cv::line(debug, cv::Point(e, 0), cv::Point(e, 400), BLUE, 1);
        for (auto e : XTicks()) cv::line(debug, cv::Point(0, e), cv::Point(400, e), GREEN, 1);
        cv::imshow("borderX", border_mapX);
        cv::imshow("borderY", border_mapY);
        cv::imshow("debug", debug);
        cv::waitKey();
    }

};


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
