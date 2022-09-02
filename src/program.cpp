#include "stdafx.h"
#include "program.h"

Program::Program(const ProgramMetaData& metaData, const std::vector<std::shared_ptr<Instruction>>& instructions)
  : metaData(metaData), instructions(instructions) {}
