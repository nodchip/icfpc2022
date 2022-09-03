#include "stdafx.h"
#include <thread>
#include <queue>
#include <format>
#include "solver_registry.h"
#include <fmt/format.h>
#include "instruction.h"
#include "instruction_cost_calculator.h"


using ColorMap = std::map<Color, int>;
ColorMap operator+(const ColorMap& a, const ColorMap& b) {
    ColorMap ret(a);
    for (const auto& e : b) {
        if (ret.count(e.first)) ret[e.first] += e.second;
        else ret[e.first] = e.second;
    }
    return ret;
}
ColorMap operator+(const ColorMap& a, const Color& col) {
    ColorMap ret(a);
    if (ret.count(col)) ret[col] += 1;
    else ret[col] = 1;
    return ret;
}
ColorMap operator-(const ColorMap& a, const ColorMap& b) {
    ColorMap ret(a);
    for (const auto& e : b) {
        assert(ret.count(e.first));
        if (ret[e.first] != e.second) ret[e.first] -= e.second;
        else ret.erase(e.first);
    }
    return ret;
}


struct CumulativeColorMap {
    PaintingPtr painting;
    std::vector<std::vector<ColorMap>> cumulative;
    CumulativeColorMap(PaintingPtr painting_) : painting(painting_) {
        cumulative.resize(painting->height + 1, std::vector<ColorMap>(painting->width + 1));
        for (int y = 0; y < painting->height; ++y) {
            LOG(INFO) << fmt::format("y is : {}", y);
            for (int x = 0; x < painting->width; ++x) {
                cumulative[y + 1][x + 1] = cumulative[y][x+1] + (cumulative[y+1][x] - cumulative[y][x]) + painting->operator()(x, y);
            }
        }
    }
    ColorMap Get(const Point& bottomLeft, const Point& topRight) {
        return cumulative[topRight.py][topRight.px] + cumulative[bottomLeft.py][bottomLeft.py] - cumulative[topRight.py][bottomLeft.px] - cumulative[bottomLeft.py][topRight.px];
    }
};
using CumulativeColorMapPtr = std::shared_ptr<CumulativeColorMap>;

CumulativeColorMapPtr color_map;


struct DividedPainting {
    PaintingPtr painting;
    SimpleBlock block;
    DividedPainting(PaintingPtr painting_, const SimpleBlock& block_) : painting(painting_), block(block_) {}
    double PointCut(const Point& point, std::vector<std::shared_ptr<Instruction>>& solution, std::vector<DividedPainting>& children) {
        assert(point.isStrictryInside(block.bottomLeft, block.topRight));
        auto instruction = std::make_shared<PointCutInstruction>(block.id, point);
        solution.push_back(instruction);
        children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".0", block.bottomLeft, point, block.color)));
        children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".1", Point(point.px, block.bottomLeft.py), Point(block.topRight.px, point.py), block.color)));
        children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".2", point, block.topRight, block.color)));
        children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".3", Point(block.bottomLeft.px, point.py), Point(point.px, block.topRight.py), block.color)));
        return getCost((*instruction), block.size.getScalarSize(), painting->width * painting->height);
    }
    double LineCutX(int px, std::vector<std::shared_ptr<Instruction>>& solution, std::vector<DividedPainting>& children) {
        assert(block.bottomLeft.px < px);
        assert(px < block.topRight.px);
        auto instruction = std::make_shared<VerticalCutInstruction>(block.id, px);
        solution.push_back(instruction);
        std::vector<DividedPainting> ret;
        children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".0", block.bottomLeft, Point(px, block.topRight.py), block.color)));
        children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".1", Point(px, block.bottomLeft.py), block.topRight, block.color)));
        return getCost((*instruction), block.size.getScalarSize(), painting->width * painting->height);
    }
    double LineCutY(int py, std::vector<std::shared_ptr<Instruction>>& solution, std::vector<DividedPainting>& children) {
        assert(block.bottomLeft.py < py);
        assert(py < block.topRight.py);
        auto instruction = std::make_shared<HorizontalCutInstruction>(block.id, py);
        solution.push_back(instruction);
        std::vector<DividedPainting> ret;
        children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".0", block.bottomLeft, Point(block.topRight.px, py), block.color)));
        children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".1", Point(block.bottomLeft.px, py), block.topRight, block.color)));
        return getCost((*instruction), block.size.getScalarSize(), painting->width * painting->height);
    }
    double Cut(const Point& point, std::vector<std::shared_ptr<Instruction>>& solution, std::vector<DividedPainting>& children) {
        if (point.isStrictryInside(block.bottomLeft, block.topRight)) return PointCut(point, solution, children);
        if (point.px == block.bottomLeft.px || point.px == block.topRight.px) return LineCutY(point.py, solution, children);
        return LineCutX(point.px, solution, children);
        
    }

    double Coloring(const Color& col, std::vector<std::shared_ptr<Instruction>>& solution) {
        auto instruction = std::make_shared<ColorInstruction>(block.id, col);
        solution.push_back(instruction);
        block.color = col;
        return getCost((*instruction), block.size.getScalarSize(), painting->width * painting->height);
    }

    ColorMap CountColorVariation() {
#if 0
        if(!color_map) color_map = std::make_shared<CumulativeColorMap>(CumulativeColorMap(painting));
        return color_map->Get(block.bottomLeft, block.topRight);
#else
        ColorMap ret;
        for (int y = block.bottomLeft.py; y < block.topRight.py; ++y)for (int x = block.bottomLeft.px; x < block.topRight.px; ++x) {
            auto col = painting->operator()(x, y);
            if (ret.count(col)) ret[col] += 1;
            else ret[col] = 1;

        }
        return ret;
#endif
    }

    Color MostCommonColor(const ColorMap& color_var) {
        int max_count = -1;
        Color ret = block.color;
        for (auto e : color_var) if (e.second > max_count) {
            max_count = e.second;
            ret = e.first;
        }
        return ret;
    }
    Point MidPoint() { return Point((block.bottomLeft.px + block.topRight.px) / 2, (block.bottomLeft.py + block.topRight.py) / 2); }
    Point EdgePoint(std::mt19937_64& rand, int itr_max = 5) {
        Point ret(block.bottomLeft.px + 1, block.bottomLeft.py + 1);
        if (block.topRight.px - block.bottomLeft.px > 2) {
            std::uniform_int_distribution<> randX(block.bottomLeft.px + 1, block.topRight.px - 2);
            int max_count = -1;
            int max_px = -1;
            for (int itr = 0; itr < itr_max; ++itr) {
                auto px = randX(rand);
                assert(0 <= px);
                assert(px < painting->width - 1);
                int count = 0;
                for (int py = block.bottomLeft.py; py < block.topRight.py; ++py) {
                    if (painting->operator()(px, py) != painting->operator()(px + 1, py)) count++;
                }
                if (count > max_count) {
                    max_px = px;
                    max_count = count;
                }
            }
            ret.px = max_px;
        }
        if (block.topRight.py - block.bottomLeft.py > 2) {
            std::uniform_int_distribution<> randY(block.bottomLeft.py + 1, block.topRight.py - 2);
            int max_count = -1;
            int max_py = -1;
            for (int itr = 0; itr < itr_max; ++itr) {
                auto py = randY(rand);
                assert(0 <= py);
                assert(py < painting->height - 1);
                int count = 0;
                for (int px = block.bottomLeft.px; px < block.topRight.px; ++px) {
                    if (painting->operator()(px, py) != painting->operator()(px, py+1)) count++;
                }
                if (count > max_count) {
                    max_py = py;
                    max_count = count;
                }
            }
            ret.py = max_py;
        }
        return ret;

    }
};


class QuadTree : public SolverBase {
public:
    struct Option : public OptionBase {
      int min_cell_size = -1;
      int mid_point_itr = -1;
      void setOptionParser(CLI::App* app) override {
        app->add_option("--quad-tree-min-cell-size", min_cell_size);
        app->add_option("--mid-point-iter", mid_point_itr);
      }
    };
    virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

    QuadTree() { }
    SolverOutputs solve(const SolverArguments& args) override {
        LOG(INFO) << "min_cell_size = " << getOption<Option>()->min_cell_size;
        SolverOutputs ret;
        assert(args.painting);
        LOG(INFO) << "arrived";
        LOG(INFO) << fmt::format("width is{}, height is{}", args.painting->width, args.painting->height);
        auto initial_paint = DividedPainting(args.painting, SimpleBlock("0", Point(0, 0), Point(args.painting->width, args.painting->height), Color(0, 0, 0, 0)));
        std::queue<DividedPainting> Q;
        Q.push(initial_paint);
        double cost = 0;
        std::mt19937_64 rand(0);

        while (!Q.empty()) {
            auto paint = Q.front();
            Q.pop();
            auto color_var = paint.CountColorVariation();
            auto most_common_color = paint.MostCommonColor(color_var);
            if (paint.block.color != most_common_color) cost += paint.Coloring(most_common_color, ret.solution);

            if (std::min(paint.block.topRight.px - paint.block.bottomLeft.px, paint.block.topRight.py - paint.block.bottomLeft.py) <= getOption<Option>()->min_cell_size)
              continue;

            if (color_var.size() > 1) {
                auto itr = getOption<Option>()->mid_point_itr;
                auto next_point = itr > 0 ? paint.EdgePoint(rand,itr) : paint.MidPoint();
                assert(paint.block.bottomLeft.px != next_point.px || paint.block.bottomLeft.py != next_point.py);
                std::vector<DividedPainting> children;
                cost += paint.Cut(next_point, ret.solution, children);
                for (const auto& e : children) Q.push(e);
            }
        }
        LOG(INFO) << fmt::format("Cost = {}", cost);
        return ret;
    }
};

REGISTER_SOLVER_WITH_OPTION("QuadTree", QuadTree, QuadTree::Option);
// vim:ts=2 sw=2 sts=2 et ci
