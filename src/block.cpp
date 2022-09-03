#include "stdafx.h"
#include <cassert>

#include "block.h"

SimpleBlock::SimpleBlock(const std::string& id, const Point& bottomLeft, const Point& topRight, const RGBA& color)
  : Block(BlockType::SimpleBlockType, id, bottomLeft, topRight), color(color)
{
  assert(bottomLeft.px <= topRight.px && bottomLeft.py <= topRight.py);
  assert(size.getScalarSize() > 0);
}

std::vector<std::shared_ptr<SimpleBlock>> SimpleBlock::getChildren() const {
  std::vector<std::shared_ptr<SimpleBlock>> children;
  children.push_back(std::make_shared<SimpleBlock>(*this));
  return children;
}

ComplexBlock::ComplexBlock(const std::string& id, const Point& bottomLeft, const Point& topRight, const std::vector<std::shared_ptr<SimpleBlock>>& subBlocks) : Block(BlockType::ComplexBlockType, id, bottomLeft, topRight), subBlocks(subBlocks)
{
  assert(size.getScalarSize() > 0);
}

std::vector<std::shared_ptr<SimpleBlock>> ComplexBlock::getChildren() const {
  return subBlocks;
}

std::vector<std::shared_ptr<SimpleBlock>> ComplexBlock::offsetChildren(const Point& newBottomLeft) const {
    std::vector<std::shared_ptr<SimpleBlock>> newChildren;
    for (const auto& block : subBlocks) {
        newChildren.push_back(std::make_shared<SimpleBlock>(
            "child",
            block->bottomLeft.add(newBottomLeft).subtract(bottomLeft),
            block->topRight.add(newBottomLeft).subtract(bottomLeft),
            block->color
            ));
    }
    return newChildren;
}

Block::Block(BlockType typ, const std::string& id, const Point& bottomLeft, const Point& topRight)
  : typ(typ), id(id), bottomLeft(bottomLeft), topRight(topRight), size(topRight.getDiff(bottomLeft)) {}
