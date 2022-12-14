#include "stdafx.h"
#include "instruction.h"
#include <cassert>
#include <fstream>
#include <fmt/core.h>

int ColorInstruction::kBaseCost = 5;
int PointCutInstruction::kBaseCost = 10;
int VerticalCutInstruction::kBaseCost = 7;
int HorizontalCutInstruction::kBaseCost = 7;
int SwapInstruction::kBaseCost = 3;
int MergeInstruction::kBaseCost = 1;

void globallySetInstructionCost(bool hasBackground) {
  if (hasBackground) {
    LOG(INFO) << "using with-background table";
    ColorInstruction::kBaseCost = 5;
    PointCutInstruction::kBaseCost = 3;
    VerticalCutInstruction::kBaseCost = 2;
    HorizontalCutInstruction::kBaseCost = 2;
    SwapInstruction::kBaseCost = 3;
    MergeInstruction::kBaseCost = 1;
  } else {
    LOG(INFO) << "using no-background table";
    ColorInstruction::kBaseCost = 5;
    PointCutInstruction::kBaseCost = 10;
    VerticalCutInstruction::kBaseCost = 7;
    HorizontalCutInstruction::kBaseCost = 7;
    SwapInstruction::kBaseCost = 3;
    MergeInstruction::kBaseCost = 1;
  }
}

std::string Instruction::toString() const {
  return "INVALID";
}

std::string NopInstruction::toString() const {
  return fmt::format("");
}

std::string CommentInstruction::toString() const {
  return fmt::format("# {}", comment);
}

std::string ColorInstruction::toString() const {
  return fmt::format("color [{}] [{}, {}, {}, {}]", block_id, color[0], color[1], color[2], color[3]);
}

std::string PointCutInstruction::toString() const {
  return fmt::format("cut [{}] [{}, {}]", block_id, point.px, point.py);
}

std::string VerticalCutInstruction::toString() const {
  return fmt::format("cut [{}] [x] [{}]", block_id, lineNumber);
}

std::string HorizontalCutInstruction::toString() const {
  return fmt::format("cut [{}] [y] [{}]", block_id, lineNumber);
}

std::string SwapInstruction::toString() const {
  return fmt::format("swap [{}] [{}]", block_id1, block_id2);
}

std::string MergeInstruction::toString() const {
  return fmt::format("merge [{}] [{}]", block_id1, block_id2);
}

bool dumpInstructions(const std::string& file_path, const std::vector<std::shared_ptr<Instruction>>& header, const std::vector<std::shared_ptr<Instruction>>& instructions) {
  std::ofstream ofs(file_path.c_str());
  for (const auto& inst : header) {
    assert(inst);
    ofs << inst->toString() << std::endl;
  }
  for (const auto& inst : instructions) {
    assert(inst);
    ofs << inst->toString() << std::endl;
  }
  return true;
}
