#include "stdafx.h"
#include "solver_util.h"
#include <fmt/core.h>
#include "instruction.h"
#include "painter.h"
#include "interpreter.h"

std::vector<std::shared_ptr<Instruction>> replaceColorInstructionOptimal(GeometricMedianColorCache& cache, Painting& painting, CanvasPtr initial_canvas, const std::vector<std::shared_ptr<Instruction>>& instructions) {
  auto result = instructions;

  const RGBA white(255, 255, 255, 255);
  const RGBA marker(128, 128, 128, 128);
  auto work = instructions;
  for (size_t i = 0; i < work.size(); ++i) {
    if (auto color = std::dynamic_pointer_cast<ColorInstruction>(instructions[i])) {
      work[i] = std::make_shared<ColorInstruction>(color->block_id, white);
    }
  }

  for (int i = 0; i < instructions.size(); ++i) {
    if (auto col = std::dynamic_pointer_cast<ColorInstruction>(instructions[i])) {
      // このcolorの最終的な描画先を探す
      work[i] = std::make_shared<ColorInstruction>(col->block_id, marker);
      Point bottomLeft(0, 0), topRight(0, 0);
      int found = 0;
      {
        auto canvas = computeCost(painting, initial_canvas, work)->canvas;
        assert(canvas);
        for (const auto& [_, block] : canvas->blocks) {
          if (auto simple_block = std::dynamic_pointer_cast<SimpleBlock>(block)) {
            if (simple_block->color == marker) {
              bottomLeft = simple_block->bottomLeft;
              topRight = simple_block->topRight;
              ++found;
            }
          }
        }
      }
      assert(found <= 1); // 各color命令が最終的に一つのSimpleBlockと仮定している(ソルバーによっては成立しない)
      if (bottomLeft != topRight) {
        auto color = cache.getColor(bottomLeft, topRight);
        if (color) {
          auto new_col = std::make_shared<ColorInstruction>(col->block_id, *color);
          result[i] = new_col;
        }
      }
      work[i] = col;
    }
  }

  std::optional<CostBreakdown> cost_input;
  try { 
    cost_input = computeCost(painting, initial_canvas, instructions);
  } catch (const InvalidInstructionException& e) {
    return instructions;
  }

  std::optional<CostBreakdown> cost_opt;
  try { 
    cost_opt = computeCost(painting, initial_canvas, result);
  } catch (const InvalidInstructionException& e) {
    return instructions;
  }

  if (cost_input && cost_opt && cost_opt->total < cost_input->total) {
    LOG(INFO) << fmt::format("replaceColorInstructionOptimal: {} -> {}", cost_input->total, cost_opt->total);
    return result;
  }
  // 誤差の影響でスコアが悪化することもあるので、その場合は何もしない
  return instructions;
}
