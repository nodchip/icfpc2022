#include "stdafx.h"
#include "random_instruction_generator.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

#include "canvas.h"
#include "instruction.h"

TEST(TestRandomInstructionGenerator, GenerateColorInstruction) {
  auto canvas = std::make_shared<Canvas>(400, 400, RGBA(255, 255, 255, 255));
  RandomInstructionGenerator generator;

  auto instruction = generator.GenerateColorInstruction(canvas);
  EXPECT_TRUE(instruction);
  auto color_instruction = std::dynamic_pointer_cast<ColorInstruction>(instruction);
  EXPECT_TRUE(color_instruction);
  EXPECT_EQ("0", color_instruction->block_id);
  EXPECT_LE(0, color_instruction->color[0]);
  EXPECT_GE(255, color_instruction->color[0]);
  EXPECT_LE(0, color_instruction->color[1]);
  EXPECT_GE(255, color_instruction->color[1]);
  EXPECT_LE(0, color_instruction->color[2]);
  EXPECT_GE(255, color_instruction->color[2]);
  EXPECT_LE(0, color_instruction->color[3]);
  EXPECT_GE(255, color_instruction->color[3]);
}

TEST(TestRandomInstructionGenerator, GenerateHorizontalCutInstruction) {
  auto canvas = std::make_shared<Canvas>(400, 400, RGBA(255, 255, 255, 255));
  RandomInstructionGenerator generator;

  auto instruction = generator.GenerateHorizontalCutInstruction(canvas);
  EXPECT_TRUE(instruction);
  auto horizontal_cut_instruction = std::dynamic_pointer_cast<HorizontalCutInstruction>(instruction);
  EXPECT_TRUE(horizontal_cut_instruction);
  EXPECT_EQ("0", horizontal_cut_instruction->block_id);
  EXPECT_LT(0, horizontal_cut_instruction->lineNumber);
  EXPECT_GT(400, horizontal_cut_instruction->lineNumber);
}

TEST(TestRandomInstructionGenerator, GenerateVerticalCutInstruction) {
  auto canvas = std::make_shared<Canvas>(400, 400, RGBA(255, 255, 255, 255));
  RandomInstructionGenerator generator;

  auto instruction = generator.GenerateVerticalCutInstruction(canvas);
  EXPECT_TRUE(instruction);
  auto vertical_cut_instruction = std::dynamic_pointer_cast<VerticalCutInstruction>(instruction);
  EXPECT_TRUE(vertical_cut_instruction);
  EXPECT_EQ("0", vertical_cut_instruction->block_id);
  EXPECT_LT(0, vertical_cut_instruction->lineNumber);
  EXPECT_GT(400, vertical_cut_instruction->lineNumber);
}

TEST(TestRandomInstructionGenerator, GeneratePointCutInstruction) {
  auto canvas = std::make_shared<Canvas>(400, 400, RGBA(255, 255, 255, 255));
  RandomInstructionGenerator generator;

  auto instruction = generator.GeneratePointCutInstruction(canvas);
  EXPECT_TRUE(instruction);
  auto point_cut_instruction = std::dynamic_pointer_cast<PointCutInstruction>(instruction);
  EXPECT_TRUE(point_cut_instruction);
  EXPECT_EQ("0", point_cut_instruction->block_id);
  EXPECT_LT(0, point_cut_instruction->point.px);
  EXPECT_GT(400, point_cut_instruction->point.px);
  EXPECT_LT(0, point_cut_instruction->point.py);
  EXPECT_GT(400, point_cut_instruction->point.py);
}

TEST(TestRandomInstructionGenerator, GenerateSwapInstruction) {
  auto canvas = std::make_shared<Canvas>(400, 400, RGBA(255, 255, 255, 255));
  canvas->blocks["1"] = std::make_shared<SimpleBlock>("1", Point(400, 0), Point(800, 400), RGBA(255, 255, 255, 255));

  RandomInstructionGenerator generator;

  auto instruction = generator.GenerateSwapInstruction(canvas);
  EXPECT_TRUE(instruction);
  auto swap_instruction = std::dynamic_pointer_cast<SwapInstruction>(instruction);
  EXPECT_TRUE(swap_instruction);
  EXPECT_EQ("0", swap_instruction->block_id1);
  EXPECT_EQ("1", swap_instruction->block_id2);
}

TEST(TestRandomInstructionGenerator, GenerateMergeInstruction) {
  auto canvas = std::make_shared<Canvas>(400, 400, RGBA(255, 255, 255, 255));
  canvas->blocks["1"] = std::make_shared<SimpleBlock>("1", Point(400, 0), Point(800, 400), RGBA(255, 255, 255, 255));

  RandomInstructionGenerator generator;

  auto instruction = generator.GenerateMergeInstruction(canvas);
  EXPECT_TRUE(instruction);
  auto merge_instruction = std::dynamic_pointer_cast<MergeInstruction>(instruction);
  EXPECT_TRUE(merge_instruction);
  EXPECT_EQ("0", merge_instruction->block_id1);
  EXPECT_EQ("1", merge_instruction->block_id2);
}

// vim:ts=2 sw=2 sts=2 et ci
