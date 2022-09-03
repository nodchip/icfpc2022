#include "stdafx.h"
#include "instruction.h"
#include <cassert>
#include <fstream>
#include <fmt/core.h>

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
  return fmt::format("color [{}] [{}, {}, {}, {}]", block_id, color.r, color.g, color.b, color.a);
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

bool dumpInstructions(const std::string& file_path, const std::vector<std::shared_ptr<Instruction>>& instructions) {
  std::ofstream ofs(file_path.c_str());
  for (const auto& inst : instructions) {
    assert(inst);
    ofs << inst->toString() << std::endl;
  }
  return true;
}
