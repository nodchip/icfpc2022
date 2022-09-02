#pragma once

#include "color.h"
#include "point.h"

using Size = Point;
enum BlockType { SimpleBlockType, ComplexBlockType };

struct Block
{

  BlockType typ;
  std::string id;
  Point bottomLeft;
  Point topRight;
  Size size;

  Block(BlockType typ, const std::string& id, const Point& bottomLeft, const Point& topRight);

  virtual std::vector<std::shared_ptr<Block>> getChildren() const = 0;
  virtual ~Block() = default;

};

struct ComplexBlock : public Block
{

  std::vector<std::shared_ptr<Block>> subBlocks;

  ComplexBlock(
    const std::string& id, const Point& bottomLeft, const Point& topRight,
    const std::vector<std::shared_ptr<Block>>& subBlocks
  );

  std::vector<std::shared_ptr<Block>> getChildren() const override;

};

struct SimpleBlock : public Block
{

  RGBA color;

  SimpleBlock(const std::string& id, const Point& bottomLeft, const Point& topRight, const RGBA& color);

  std::vector<std::shared_ptr<Block>> getChildren() const override;

};
