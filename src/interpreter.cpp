#include "stdafx.h"
#include "interpreter.h"

std::shared_ptr<InterpreterResult> Interpreter::Run(const std::string& code)
{
  // TODO(nodchip): Implement.
  return nullptr;
}

std::shared_ptr<InterpreterResult> Interpreter::Interpret(int line_number, const std::shared_ptr<Canvas>& context, const std::shared_ptr<Instruction>& instruction)
{
  // TODO(nodchip): Implement.
  return nullptr;
}

std::shared_ptr<InterpreterResult> Interpreter::ColorCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<ColorInstruction>& color_instruction)
{
  // TODO(nodchip): Implement.
  return nullptr;
}

std::shared_ptr<InterpreterResult> Interpreter::PointCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<PointCutInstruction>& point_cut_instruction)
{
  // TODO(nodchip): Implement.
  return nullptr;
}

std::shared_ptr<InterpreterResult> Interpreter::VerticalCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<VerticalCutInstruction>& vertical_cut_instruction)
{
  // TODO(nodchip): Implement.
  return nullptr;
}

std::shared_ptr<InterpreterResult> Interpreter::HorizontalCutCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<HorizontalCutInstruction>& horizontal_cut_instruction)
{
  // TODO(nodchip): Implement.
  return nullptr;
}

std::shared_ptr<InterpreterResult> Interpreter::SwapCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<SwapInstruction>& swap_instruction)
{
  // TODO(nodchip): Implement.
  return nullptr;
}

std::shared_ptr<InterpreterResult> Interpreter::MergeCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<MergeInstruction>& merge_instruction)
{
  // TODO(nodchip): Implement.
  return nullptr;
}
