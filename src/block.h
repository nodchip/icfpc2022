#pragma once

#include <memory>

#include "color.h"
#include "point.h"

using Size = Point;
enum BlockType { SimpleBlockType, ComplexBlockType };

struct SimpleBlock;

struct Block {

  BlockType typ;
  std::string id;
  Point bottomLeft;
  Point topRight;
  Size size;

  virtual std::vector<std::shared_ptr<SimpleBlock>> getChildren() const = 0;

  Block(BlockType typ, const std::string& id, const Point& bottomLeft, const Point& topRight);

};

struct SimpleBlock : Block
{

  RGBA color;

  SimpleBlock(const std::string& id, const Point& bottomLeft, const Point& topRight, const RGBA& color);

  std::vector<std::shared_ptr<SimpleBlock>> getChildren() const override;

};

struct ComplexBlock : Block
{

  std::vector<std::shared_ptr<SimpleBlock>> subBlocks;

  ComplexBlock(
    const std::string& id, const Point& bottomLeft, const Point& topRight,
    const std::vector<std::shared_ptr<SimpleBlock>>& subBlocks
  );

  std::vector<std::shared_ptr<SimpleBlock>> getChildren() const override;

  std::vector<std::shared_ptr<SimpleBlock>> offsetChildren(const Point& newBottomLeft) const;

};


