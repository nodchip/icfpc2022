#pragma once

#include <string>

#include "canvas.h"
#include "point.h"

struct Instruction {
  virtual ~Instruction() = default;
  virtual int getBaseCost() const { return 0; }
  virtual std::string toString() const;
};

struct NopInstruction : public Instruction {
  static constexpr int kBaseCost = 0;
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct CommentInstruction : public Instruction {
  static constexpr int kBaseCost = 0;
  std::string comment;
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct ColorInstruction : public Instruction {
  static constexpr int kBaseCost = 5;
  std::string block_id;
  Color color;
  ColorInstruction(std::string block_id, Color color)
      : block_id(block_id), color(color) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct PointCutInstruction : public Instruction {
  static constexpr int kBaseCost = 10;
  std::string block_id;
  Point point;
  PointCutInstruction(std::string block_id, Point point)
      : block_id(block_id), point(point) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct VerticalCutInstruction : public Instruction {
  static constexpr int kBaseCost = 7;
  std::string block_id;
  int lineNumber;
  VerticalCutInstruction(std::string block_id, int lineNumber)
      : block_id(block_id), lineNumber(lineNumber) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct HorizontalCutInstruction : public Instruction {
  static constexpr int kBaseCost = 7;
  std::string block_id;
  int lineNumber;
  HorizontalCutInstruction(std::string block_id, int lineNumber)
      : block_id(block_id), lineNumber(lineNumber) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct SwapInstruction : public Instruction {
  static constexpr int kBaseCost = 3;
  std::string block_id1;
  std::string block_id2;
  SwapInstruction(std::string block_id1, std::string block_id2)
      : block_id1(block_id1), block_id2(block_id2) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct MergeInstruction : public Instruction {
  static constexpr int kBaseCost = 1;
  std::string block_id1;
  std::string block_id2;
  MergeInstruction(std::string block_id1, std::string block_id2)
      : block_id1(block_id1), block_id2(block_id2) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

bool dumpInstructions(
    const std::string& file_path,
    const std::vector<std::shared_ptr<Instruction>>& instructions);
