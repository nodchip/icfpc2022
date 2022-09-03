#pragma once

#include <memory>
#include <tuple>
#include <variant>
#include <vector>

#include "instruction.h"

struct ParseError {
  int line_number;
  std::string message;
  ParseError(int line_number, std::string message) : line_number(line_number), message(message) {}
};

using ParseResult = std::variant<std::shared_ptr<Instruction>, std::shared_ptr<ParseError>>;

struct Parser
{
  ParseResult ParseLine(int line_number, std::string line);
  std::vector<std::shared_ptr<Instruction>> ParseFile(const std::string& isl_file_path);
};
