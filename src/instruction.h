#pragma once

#include <string>

#include "canvas.h"
#include "point.h"

#define assert_throw_invalid_instruction(x) do { if (!(x)) throw InvalidInstructionException(); } while (false)
class InvalidInstructionException : public std::runtime_error {
public:
  InvalidInstructionException() : std::runtime_error("invalid instruction detected") {}
};

enum class InstructionType {
  NopInstructionType,
  CommentInstructionType,
  ColorInstructionType,
  PointCutInstructionType,
  VerticalCutInstructionType,
  HorizontalCutInstructionType,
  SwapInstructionType,
  MergeInstructionType,
};

struct Instruction {
  virtual ~Instruction() = default;
  virtual int getBaseCost() const { return 0; }
  virtual std::string toString() const;
  virtual std::string GetBlockID() { return ""; }
  virtual void SetBlockID(const std::string& name) { return; }
};

struct NopInstruction : public Instruction {
  static constexpr int kBaseCost = 0;
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct CommentInstruction : public Instruction {
  static constexpr int kBaseCost = 0;
  std::string comment;
  CommentInstruction(const std::string& comment) : comment(comment) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct ColorInstruction : public Instruction {
  static int kBaseCost;
  std::string block_id;
  Color color;
  ColorInstruction(std::string block_id, Color color) : block_id(block_id), color(color) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
  std::string GetBlockID() override { return block_id; }
  virtual void SetBlockID(const std::string& name) { block_id = name; }
};

struct PointCutInstruction : public Instruction {
  static int kBaseCost;
  std::string block_id;
  Point point;
  PointCutInstruction(std::string block_id, Point point) : block_id(block_id), point(point) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
  std::string GetBlockID() override { return block_id; }
  virtual void SetBlockID(const std::string& name) { block_id = name; }
};

struct VerticalCutInstruction : public Instruction {
  static int kBaseCost;
  std::string block_id;
  int lineNumber;
  VerticalCutInstruction(std::string block_id, int lineNumber) : block_id(block_id), lineNumber(lineNumber) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
  std::string GetBlockID() override { return block_id; }
  virtual void SetBlockID(const std::string& name) { block_id = name; }
};

struct HorizontalCutInstruction : public Instruction {
  static int kBaseCost;
  std::string block_id;
  int lineNumber;
  HorizontalCutInstruction(std::string block_id, int lineNumber) : block_id(block_id), lineNumber(lineNumber) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
  std::string GetBlockID() override { return block_id; }
  virtual void SetBlockID(const std::string& name) { block_id = name; }
};

struct SwapInstruction : public Instruction {
  static int kBaseCost;
  std::string block_id1;
  std::string block_id2;
  SwapInstruction(std::string block_id1, std::string block_id2) : block_id1(block_id1), block_id2(block_id2) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

struct MergeInstruction : public Instruction {
  static int kBaseCost;
  std::string block_id1;
  std::string block_id2;
  MergeInstruction(std::string block_id1, std::string block_id2) : block_id1(block_id1), block_id2(block_id2) {}
  int getBaseCost() const override { return kBaseCost; }
  std::string toString() const override;
};

void globallySetInstructionCost(bool hasBackground);

bool dumpInstructions(
    const std::string& file_path,
    const std::vector<std::shared_ptr<Instruction>>& header,
    const std::vector<std::shared_ptr<Instruction>>& instructions);
