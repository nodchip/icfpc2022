#pragma once

#include <memory>
#include <string>
#include <cassert>

#include <parser.h>
#include <point.h>
#include <canvas.h>
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

  std::shared_ptr<InterpreterResult> Run(const std::string& code);

private:

  // TODO: 後で .cpp に移動
  std::shared_ptr<InterpreterResult> Interpret(int line_number, const std::shared_ptr<Canvas>& context, const std::shared_ptr<Instruction>& instruction) {
    if (auto color_instruction = std::dynamic_pointer_cast<ColorInstruction>(instruction)) return ColorCanvas(line_number, context, color_instruction);
    else if (auto point_cut_instruction = std::dynamic_pointer_cast<PointCutInstruction>(instruction)) return PointCutCanvas(line_number, context, point_cut_instruction);
    else if (auto vertical_cut_instruction = std::dynamic_pointer_cast<VerticalCutInstruction>(instruction)) return VerticalCutCanvas(line_number, context, vertical_cut_instruction);
    else if (auto horizontal_cut_instruction = std::dynamic_pointer_cast<HorizontalCutInstruction>(instruction)) return HorizontalCutCanvas(line_number, context, horizontal_cut_instruction);
    else if (auto swap_instruction = std::dynamic_pointer_cast<SwapInstruction>(instruction)) return SwapCanvas(line_number, context, swap_instruction);
    else if (auto merge_instruction = std::dynamic_pointer_cast<MergeInstruction>(instruction)) return MergeCanvas(line_number, context, merge_instruction);
    else if (auto nop_instruction = std::dynamic_pointer_cast<NopInstruction>(instruction)) {
      return std::make_shared<InterpreterResult>(context, 0);
    }
    else if (auto comment_instruction = std::dynamic_pointer_cast<CommentInstruction>(instruction)) {
      return std::make_shared<InterpreterResult>(context, 0);
    }
    else assert(false);
    return nullptr;
  }

  std::shared_ptr<InterpreterResult> ColorCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<ColorInstruction>& color_instruction);

  std::shared_ptr<InterpreterResult> PointCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<PointCutInstruction>& point_cut_instruction);

  std::shared_ptr<InterpreterResult> VerticalCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<VerticalCutInstruction>& vertical_cut_instruction);

  std::shared_ptr<InterpreterResult> HorizontalCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<HorizontalCutInstruction>& horizontal_cut_instruction);

  std::shared_ptr<InterpreterResult> SwapCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<SwapInstruction>& swap_instruction);

  std::shared_ptr<InterpreterResult> MergeCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<MergeInstruction>& merge_instruction);
};

