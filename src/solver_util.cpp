#include "stdafx.h"
#include "solver_util.h"
#include <fmt/core.h>
#include "instruction.h"
#include "painter.h"
#include "interpreter.h"

std::vector<std::shared_ptr<Instruction>> replaceColorInstructionOptimal(GeometricMedianColorCache& cache, Painting& painting, CanvasPtr initial_canvas, const std::vector<std::shared_ptr<Instruction>>& instructions) {
  auto result = instructions;

  const RGBA white(255, 255, 255, 255);
  const RGBA marker(0, 0, 0, 0);
  auto white_instructions = instructions;
  for (auto& inst : white_instructions) {
    if (auto color = std::dynamic_pointer_cast<ColorInstruction>(inst)) {
      color->color = white;
    }
  }

  for (int i = 0; i < instructions.size(); ++i) {
    if (auto col = std::dynamic_pointer_cast<ColorInstruction>(instructions[i])) {
      // このcolorの最終的な描画先を探す
      auto work = white_instructions;
      work[i] = std::make_shared<ColorInstruction>(col->block_id, marker);
      // 各color命令が最終的に一つのSimpleBlockと仮定している(ソルバーによっては成立しない)
      Point bottomLeft(0, 0), topRight(0, 0);
      {
        auto canvas = computeCost(painting, initial_canvas, work)->canvas;
        assert(canvas);
        for (const auto& [_, block] : canvas->blocks) {
          if (auto simple_block = std::dynamic_pointer_cast<SimpleBlock>(block)) {
            if (simple_block->color == marker) {
              bottomLeft = simple_block->bottomLeft;
              topRight = simple_block->topRight;
              break;
            }
          }
        }
      }
      if (bottomLeft != topRight) {
        auto color = cache.getColor(bottomLeft, topRight);
        if (color) {
          auto new_col = std::make_shared<ColorInstruction>(col->block_id, *color);
          result[i] = new_col;
        }
      }
    }
  }
  return result;
}
