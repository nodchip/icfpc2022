#include "stdafx.h"
#include <cassert>

#include "block.h"

Block::Block(BlockType typ, const std::string& id, const Point& bottomLeft, const Point& topRight)
    : typ(typ), id(id), bottomLeft(bottomLeft), topRight(topRight) {}

ComplexBlock::ComplexBlock(const std::string& id, const Point& bottomLeft, const Point& topRight, const std::vector<std::shared_ptr<Block>>& subBlocks) : Block(BlockType::ComplexBlockType, id, bottomLeft, topRight), subBlocks(subBlocks) {
  assert(size.getScalarSize() > 0);
}

std::vector<std::shared_ptr<Block>> ComplexBlock::getChildren() const {
  return subBlocks;
}

SimpleBlock::SimpleBlock(const std::string& id, const Point& bottomLeft, const Point& topRight, const RGBA& color)
  : Block(BlockType::SimpleBlockType, id, bottomLeft, topRight), color(color)
{
  size = topRight.getDiff(bottomLeft);
  assert(bottomLeft.px <= topRight.px && bottomLeft.py <= topRight.py);
  assert(size.getScalarSize() > 0);
}

std::vector<std::shared_ptr<Block>> SimpleBlock::getChildren() const {
  std::vector<std::shared_ptr<Block>> children;
  children.push_back(std::make_shared<SimpleBlock>(*this));
  return children;
}
