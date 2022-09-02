#pragma once

#include <memory>
#include <vector>

#include "instruction.h"

struct Parser
{
  bool Parse(const std::string& code, std::vector<std::shared_ptr<Instruction>>& program);
};
