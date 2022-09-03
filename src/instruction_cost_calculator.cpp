#include "stdafx.h"
#include "instruction_cost_calculator.h"

#include <cassert>

int getCost(const Instruction& instruction, int blockSize, int canvasSize) {
  assert(canvasSize > 0);
  assert(blockSize > 0);
  return int(std::round(double(instruction.getBaseCost()) * double(canvasSize) / double(blockSize)));
}

