#pragma once

#include <memory>
#include <string>
#include <optional>
#include <cassert>

#include <parser.h>
#include <point.h>
#include <canvas.h>
#include <program.h>
#include <painter.h>
#include <instruction.h>
#include <instruction_cost_calculator.h>

struct InterpreterResult
{
  std::shared_ptr<Canvas> canvas;
  int cost;

  InterpreterResult(const std::shared_ptr<Canvas>& canvas, int cost);
};

struct Interpreter
{
  int top_level_id_counter = 0;

  std::shared_ptr<InterpreterResult> Run(const Program& program);

private:

  // TODO: 後で .cpp に移動
  std::shared_ptr<InterpreterResult> Interpret(int line_number, const std::shared_ptr<Canvas>& context, const std::shared_ptr<Instruction>& instruction);

  std::shared_ptr<InterpreterResult> ColorCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<ColorInstruction>& color_instruction);

  std::shared_ptr<InterpreterResult> PointCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<PointCutInstruction>& point_cut_instruction);

  std::shared_ptr<InterpreterResult> VerticalCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<VerticalCutInstruction>& vertical_cut_instruction);

  std::shared_ptr<InterpreterResult> HorizontalCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<HorizontalCutInstruction>& horizontal_cut_instruction);

  std::shared_ptr<InterpreterResult> SwapCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<SwapInstruction>& swap_instruction);

  std::shared_ptr<InterpreterResult> MergeCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<MergeInstruction>& merge_instruction);
};

struct CostBreakdown {
  int instruction;
  int similarity;
  int total;
  std::shared_ptr<Canvas> canvas;
};
std::optional<CostBreakdown> computeCost(const Painting& problem, const std::vector<std::shared_ptr<Instruction>>& instructions);

