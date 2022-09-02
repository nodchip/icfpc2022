#pragma once

#include <memory>
#include <tuple>
#include <variant>
#include <vector>

#include "instruction.h"
#include "program.h"

using ParseError = std::tuple<int, std::string>;

struct ParseResult
{
  std::string typ;
  std::variant<std::shared_ptr<Program>, std::shared_ptr<Instruction>, std::shared_ptr<ParseError>> result;
};

struct Parser
{
  std::shared_ptr<ParseResult> Parse(const std::string& code);
};
