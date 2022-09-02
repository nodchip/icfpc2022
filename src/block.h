#pragma once

#include "color.h"
#include "point.h"

using Size = Point;
enum BlockType { SimpleBlockType, ComplexBlockType };

struct SimpleBlock
{

  BlockType typ;
  std::string id;
  Point bottomLeft;
  Point topRight;
  Size size;
  RGBA color;

  SimpleBlock(const std::string& id, const Point& bottomLeft, const Point& topRight, const RGBA& color);

  std::vector<std::shared_ptr<SimpleBlock>> getChildren() const;

};

struct ComplexBlock
{

  BlockType typ;
  std::string id;
  Point bottomLeft;
  Point topRight;
  Size size;
  std::vector<std::shared_ptr<SimpleBlock>> subBlocks;

  ComplexBlock(
    const std::string& id, const Point& bottomLeft, const Point& topRight,
    const std::vector<std::shared_ptr<SimpleBlock>>& subBlocks
  );

  std::vector<std::shared_ptr<SimpleBlock>> getChildren() const;

  std::vector<std::shared_ptr<SimpleBlock>> offsetChildren(const Point& newBottomLeft) const;

};


