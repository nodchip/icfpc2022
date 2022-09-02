#pragma once

#include <string>

#include "canvas.h"
#include "point.h"

struct Instruction
{
  virtual ~Instruction() = default;
  virtual std::string toString() const;
};

struct ColorInstruction : public Instruction
{
  std::string block_id;
  Color color;
  ColorInstruction(std::string block_id, Color color) : block_id(block_id), color(color) {}
  std::string toString() const override;
};

struct PointCutInstruction : public Instruction
{
  std::string block_id;
  Point point;
  PointCutInstruction(std::string block_id, Point point) : block_id(block_id), point(point) {}
  std::string toString() const override;
};

struct VerticalCutInstruction : public Instruction
{
  std::string block_id;
  int lineNumber;
  VerticalCutInstruction(std::string block_id, int lineNumber) : block_id(block_id), lineNumber(lineNumber) {}
  std::string toString() const override;
};

struct HorizontalCutInstruction : public Instruction
{
  std::string block_id;
  int lineNumber;
  HorizontalCutInstruction(std::string block_id, int lineNumber) : block_id(block_id), lineNumber(lineNumber) {}
  std::string toString() const override;
};

struct SwapInstruction : public Instruction
{
  std::string block_id1;
  std::string block_id2;
  SwapInstruction(std::string block_id1, std::string block_id2) : block_id1(block_id1), block_id2(block_id2) {}
  std::string toString() const override;
};

struct MergeInstruction : public Instruction
{
  std::string block_id1;
  std::string block_id2;
  MergeInstruction(std::string block_id1, std::string block_id2) : block_id1(block_id1), block_id2(block_id2) {}
  std::string toString() const override;
};
