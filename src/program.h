#pragma once

#include <vector>

#include "color.h"
#include "instruction.h"

struct ProgramMetaData
{
  int width;
  int height;
  RGBA backgroundColor;
};

struct Program {

  ProgramMetaData metaData;
  std::vector<std::shared_ptr<Instruction>> instructions;

  Program(const ProgramMetaData& metaData, const std::vector<std::shared_ptr<Instruction>>& instructions);

};
