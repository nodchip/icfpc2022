#include "stdafx.h"
#include "interpreter.h"

InterpreterResult::InterpreterResult(const std::shared_ptr<Canvas>& canvas, int cost) : canvas(canvas), cost(cost) {}

std::shared_ptr<InterpreterResult> Interpreter::Run(const std::string& code)
{
  // TODO(nodchip): Implement.
  return nullptr;
}

//std::shared_ptr<InterpreterResult> Interpreter::Interpret(int line_number, const std::shared_ptr<Canvas>& context, const std::shared_ptr<Instruction>& instruction)
//{
//  // TODO(nodchip): Implement.
//  return nullptr;
//}

std::shared_ptr<InterpreterResult> Interpreter::ColorCanvas(int line, const std::shared_ptr<Canvas>& context, const std::shared_ptr<ColorInstruction>& color_instruction) {
  // TypeCheck Starts
  const auto& [blockId, color] = *color_instruction;
  assert(context->blocks.count(blockId));
  const auto& block = context->blocks[blockId];
  // TypeCheck Ends

  // Scoring Starts
  const auto cost = getCost(
    *color_instruction,
    block->size.getScalarSize(),
    context->size().getScalarSize()
  );
  // Scoring Ends

  // Processing Starts
  if (block->typ == BlockType::SimpleBlockType) {
    const auto& actualBlock = std::dynamic_pointer_cast<SimpleBlock>(block);
    actualBlock->color = color;
    return std::make_shared<InterpreterResult>(context, cost);
  }

  if (block->typ == BlockType::ComplexBlockType) {
    const auto& actualBlock = std::dynamic_pointer_cast<ComplexBlock>(block);
    context->blocks[blockId] = std::make_shared<SimpleBlock>(
      actualBlock->id,
      actualBlock->bottomLeft,
      actualBlock->topRight,
      color
      );
    return std::make_shared<InterpreterResult>(context, cost);
  }
  // Processing Ends
  assert(false);
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
