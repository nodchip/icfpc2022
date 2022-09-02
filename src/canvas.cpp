#include "stdafx.h"
#include "canvas.h"

Canvas::Canvas(int width, int height, const Color& backgroundColor)
    : width(width), height(height), backgroundColor(backgroundColor)
{
    blocks["0"] = std::make_shared<SimpleBlock>("0", Point(0, 0), Point(width, height), backgroundColor);
}

Size Canvas::size() const {
  return { width, height };
}

std::vector<std::shared_ptr<SimpleBlock>> Canvas::simplify() const {
  std::vector<std::shared_ptr<SimpleBlock>> simplifiedBlocks;
  for (const auto& [id, block] : blocks) {
    for (const auto& child : block->getChildren()) {
      simplifiedBlocks.push_back(child);
    }
  }
  return simplifiedBlocks;
}
