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
    children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".0", block.bottomLeft, Point(block.topRight.px, py), block.color)));
    children.push_back(DividedPainting(painting, SimpleBlock(block.id + ".1", Point(block.bottomLeft.px, py), block.topRight, block.color)));
    return children;

}


