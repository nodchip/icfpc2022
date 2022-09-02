#pragma once

#include <memory>
#include <string>

struct Canvas;
struct Instruction;
struct ColorInstruction;
struct PointCutInstruction;
struct VerticalCutInstruction;
struct HorizontalCutInstruction;
struct SwapInstruction;
struct MergeInstruction;

struct InterpreterResult
{
  std::shared_ptr<Canvas> canvas;
  int cost;
};

struct Interpreter
{
  int top_level_id_counter = 0;

  std::shared_ptr<InterpreterResult> Run(const std::string& code);

private:
  std::shared_ptr<InterpreterResult> Interpret(int line_number, const std::shared_ptr<Canvas>& context, const std::shared_ptr<Instruction>& instruction);

  std::shared_ptr<InterpreterResult> ColorCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<ColorInstruction>& color_instruction);

  std::shared_ptr<InterpreterResult> PointCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<PointCutInstruction>& point_cut_instruction);

  std::shared_ptr<InterpreterResult> VerticalCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<VerticalCutInstruction>& vertical_cut_instruction);

  std::shared_ptr<InterpreterResult> HorizontalCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<HorizontalCutInstruction>& horizontal_cut_instruction);

  std::shared_ptr<InterpreterResult> SwapCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<SwapInstruction>& swap_instruction);

  std::shared_ptr<InterpreterResult> MergeCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<MergeInstruction>& merge_instruction);
};

