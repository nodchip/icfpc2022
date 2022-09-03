#include "stdafx.h"
#include "random_instruction_generator.h"

#include "canvas.h"
#include "color.h"
#include "instruction.h"
#include "xorshift.h"

std::shared_ptr<Instruction> RandomInstructionGenerator::GenerateRandomInstruction(const std::shared_ptr<Canvas>& state) {
  const InstructionType instruction_types[] = {
    InstructionType::ColorInstructionType,
    InstructionType::ColorInstructionType,
    InstructionType::ColorInstructionType,
    InstructionType::ColorInstructionType,
    InstructionType::HorizontalCutInstructionType,
    InstructionType::VerticalCutInstructionType,
    InstructionType::PointCutInstructionType,
    InstructionType::SwapInstructionType,
    InstructionType::MergeInstructionType,
  };
  InstructionType instruction_type = instruction_types[xor64() % std::size(instruction_types)];

  switch (instruction_type) {
  case InstructionType::ColorInstructionType: {
    return GenerateColorInstruction(state);
  }
  case InstructionType::HorizontalCutInstructionType: {
    return GenerateHorizontalCutInstruction(state);
  }
  case InstructionType::VerticalCutInstructionType: {
    return GenerateVerticalCutInstruction(state);
  }
  case InstructionType::PointCutInstructionType: {
    return GeneratePointCutInstruction(state);
  }
  case InstructionType::SwapInstructionType: {
    return GenerateSwapInstruction(state);
  }
  case InstructionType::MergeInstructionType: {
    return GenerateMergeInstruction(state);
  }
  }
  return std::make_shared<CommentInstruction>("Should not come here");
}

std::shared_ptr<Instruction> RandomInstructionGenerator::GenerateColorInstruction(const std::shared_ptr<Canvas>& state) {
  int index = static_cast<int>(xor64() % state->blocks.size());
  auto it = state->blocks.begin();
  std::advance(it, index);
  auto block = it->second;
  auto color = GenerateRandomColor();
  return std::make_shared<ColorInstruction>(block->id, color);
}

std::shared_ptr<Instruction> RandomInstructionGenerator::GenerateHorizontalCutInstruction(const std::shared_ptr<Canvas>& state)
{
  int index = static_cast<int>(xor64() % state->blocks.size());
  auto it = state->blocks.begin();
  std::advance(it, index);
  auto block = it->second;
  auto min = block->bottomLeft.py + 1;
  auto max = block->topRight.py - 1;
  if (max - min <= 1) {
    return std::shared_ptr<NopInstruction>();
  }
  auto position = static_cast<int>(xor64() % (max - min + 1) + min);
  return std::make_shared<HorizontalCutInstruction>(block->id, position);
}

std::shared_ptr<Instruction> RandomInstructionGenerator::GenerateVerticalCutInstruction(const std::shared_ptr<Canvas>& state)
{
  int index = static_cast<int>(xor64() % state->blocks.size());
  auto it = state->blocks.begin();
  std::advance(it, index);
  auto block = it->second;
  auto min = block->bottomLeft.px + 1;
  auto max = block->topRight.px - 1;
  if (max - min <= 1) {
    return std::make_shared<NopInstruction>();
  }
  auto position = static_cast<int>(xor64() % (max - min + 1) + min);
  return std::make_shared<VerticalCutInstruction>(block->id, position);
}

std::shared_ptr<Instruction> RandomInstructionGenerator::GeneratePointCutInstruction(const std::shared_ptr<Canvas>& state)
{
  int index = static_cast<int>(xor64() % state->blocks.size());
  auto it = state->blocks.begin();
  std::advance(it, index);
  auto block = it->second;
  auto x_min = block->bottomLeft.px + 1;
  auto x_max = block->topRight.px - 1;
  auto y_min = block->bottomLeft.py + 1;
  auto y_max = block->topRight.py - 1;
  if (x_max - x_min <= 1 || y_max - y_min <= 1) {
    return std::make_shared<NopInstruction>();
  }
  auto x_position = static_cast<int>(xor64() % (x_max - x_min) + x_min);
  auto y_position = static_cast<int>(xor64() % (y_max - y_min) + y_min);
  Point position(x_position, y_position);
  return std::make_shared<PointCutInstruction>(block->id, position);
}

std::shared_ptr<Instruction> RandomInstructionGenerator::GenerateSwapInstruction(const std::shared_ptr<Canvas>& state)
{
  std::vector<std::pair<std::map<std::string, std::shared_ptr<Block>>::iterator, std::map<std::string, std::shared_ptr<Block>>::iterator>> swap_pairs;
  for (auto it1 = state->blocks.begin(); it1 != state->blocks.end(); ++it1) {
    for (auto it2 = it1; it2 != state->blocks.end(); ++it2) {
      if (it1 == it2) {
        continue;
      }

      if (it1->second->size != it2->second->size) {
        continue;
      }

      swap_pairs.push_back(std::make_pair(it1, it2));
    }
  }

  if (swap_pairs.empty()) {
    return std::make_shared<NopInstruction>();
  }

  int index = static_cast<int>(xor64() % swap_pairs.size());
  const auto& block_id1 = swap_pairs[index].first->first;
  const auto& block_id2 = swap_pairs[index].second->first;
  return std::make_shared<SwapInstruction>(block_id1, block_id2);
}

std::shared_ptr<Instruction> RandomInstructionGenerator::GenerateMergeInstruction(const std::shared_ptr<Canvas>& state)
{
  std::vector<std::pair<std::map<std::string, std::shared_ptr<Block>>::iterator, std::map<std::string, std::shared_ptr<Block>>::iterator>> merge_pairs;
  for (auto it1 = state->blocks.begin(); it1 != state->blocks.end(); ++it1) {
    for (auto it2 = it1; it2 != state->blocks.end(); ++it2) {
      if (it1 == it2) {
        continue;
      }

      if (it1->second->size != it2->second->size) {
        continue;
      }

      const auto& block1 = it1->second;
      const auto& block2 = it2->second;

      auto bottomToTop = (block1->bottomLeft.py == block2->topRight.py ||
        block1->topRight.py == block2->bottomLeft.py) &&
        block1->bottomLeft.px == block2->bottomLeft.px &&
        block1->topRight.px == block2->topRight.px;

      auto leftToRight = (block1->bottomLeft.px == block2->topRight.px ||
        block1->topRight.px == block2->bottomLeft.px) &&
        block1->bottomLeft.py == block2->bottomLeft.py &&
        block1->topRight.py == block2->topRight.py;

      auto mergable = bottomToTop || leftToRight;

      if (!mergable) {
        continue;
      }

      merge_pairs.push_back(std::make_pair(it1, it2));
    }
  }

  if (merge_pairs.empty()) {
    return std::make_shared<NopInstruction>();
  }

  int index = static_cast<int>(xor64() % merge_pairs.size());
  const auto& block_id1 = merge_pairs[index].first->first;
  const auto& block_id2 = merge_pairs[index].second->first;
  return std::make_shared<MergeInstruction>(block_id1, block_id2);
}

RGBA RandomInstructionGenerator::GenerateRandomColor() {
  int min = 0;
  int max = 255;
  int randomR = xor64() % (max - min + 1) + min;
  int randomG = xor64() % (max - min + 1) + min;
  int randomB = xor64() % (max - min + 1) + min;
  int randomA = xor64() % (max - min + 1) + min;
  return RGBA(randomR, randomG, randomB, randomA);
}
