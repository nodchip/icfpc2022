#include "stdafx.h"
#include <cassert>

#include "block.h"

SimpleBlock::SimpleBlock(const std::string& id, const Point& bottomLeft, const Point& topRight, const RGBA& color)
  : typ(BlockType::SimpleBlockType), id(id), bottomLeft(bottomLeft), topRight(topRight), color(color)
{
  size = topRight.getDiff(bottomLeft);
  assert(bottomLeft.px <= topRight.px && bottomLeft.py <= topRight.py);
  assert(size.getScalarSize() > 0);
}

std::vector<std::shared_ptr<SimpleBlock>> SimpleBlock::getChildren() const {
  std::vector<std::shared_ptr<SimpleBlock>> children;
  children.push_back(std::make_shared<SimpleBlock>(*this));
  return children;
}

ComplexBlock::ComplexBlock(
  const std::string& id, const Point& bottomLeft, const Point& topRight,
  const std::vector<std::shared_ptr<SimpleBlock>>& subBlocks
) : typ(BlockType::ComplexBlockType), id(id), bottomLeft(bottomLeft), topRight(topRight), subBlocks(subBlocks)
{
  assert(size.getScalarSize() > 0);
}

std::vector<std::shared_ptr<SimpleBlock>> ComplexBlock::getChildren() const {
  return subBlocks;
}
