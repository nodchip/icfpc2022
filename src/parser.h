#pragma once

#include <memory>
#include <tuple>
#include <variant>
#include <vector>

#include "instruction.h"
#include "program.h"

struct ParseError {
  int line_number;
  std::string message;
  ParseError(int line_number, std::string message) : line_number(line_number), message(message) {}
};

using ParseResult = std::variant<std::shared_ptr<Program>, std::shared_ptr<Instruction>, std::shared_ptr<ParseError>>;

struct Parser
{
  ParseResult Parse(const std::string& code);
  ParseResult ParseLine(int line_number, std::string line);
};
