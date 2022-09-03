#include "stdafx.h"
#include <glog/logging.h>
#include <gtest/gtest.h>

#include "parser.h"

TEST(TestParser, ParseLine_Nop) {
  auto line = "    ";

  Parser parser;
  auto result = parser.ParseLine(42, line);
  EXPECT_TRUE(std::holds_alternative<std::shared_ptr<Instruction>>(result));
  auto instruction = std::dynamic_pointer_cast<NopInstruction>(std::get<std::shared_ptr<Instruction>>(result));
  EXPECT_TRUE(instruction);
}

TEST(TestParser, ParseLine_Comment) {
  auto line = "#This is a comment.";

  Parser parser;
  auto result = parser.ParseLine(42, line);
  EXPECT_TRUE(std::holds_alternative<std::shared_ptr<Instruction>>(result));
  auto instruction = std::dynamic_pointer_cast<CommentInstruction>(std::get<std::shared_ptr<Instruction>>(result));
  EXPECT_TRUE(instruction);
  EXPECT_EQ("This is a comment.", instruction->comment);
}

TEST(TestParser, ParseLine_Color) {
  auto line = "color [0.1] [105, 16, 44, 132]";

  Parser parser;
  auto result = parser.ParseLine(42, line);
  EXPECT_TRUE(std::holds_alternative<std::shared_ptr<Instruction>>(result));
  auto instruction = std::dynamic_pointer_cast<ColorInstruction>(std::get<std::shared_ptr<Instruction>>(result));
  EXPECT_TRUE(instruction);
  EXPECT_EQ("0.1", instruction->block_id);
  EXPECT_EQ(105, instruction->color.r);
  EXPECT_EQ(16, instruction->color.g);
  EXPECT_EQ(44, instruction->color.b);
  EXPECT_EQ(132, instruction->color.a);
}

TEST(TestParser, ParseLine_VerticalCut) {
  auto line = "cut [0] [x] [68]";

  Parser parser;
  auto result = parser.ParseLine(42, line);
  EXPECT_TRUE(std::holds_alternative<std::shared_ptr<Instruction>>(result));
  auto instruction = std::dynamic_pointer_cast<VerticalCutInstruction>(std::get<std::shared_ptr<Instruction>>(result));
  EXPECT_TRUE(instruction);
  EXPECT_EQ("0", instruction->block_id);
  EXPECT_EQ(68, instruction->lineNumber);
}

TEST(TestParser, ParseLine_HorizontalCut) {
  auto line = "cut [0] [y] [68]";

  Parser parser;
  auto result = parser.ParseLine(42, line);
  EXPECT_TRUE(std::holds_alternative<std::shared_ptr<Instruction>>(result));
  auto instruction = std::dynamic_pointer_cast<HorizontalCutInstruction>(std::get<std::shared_ptr<Instruction>>(result));
  EXPECT_TRUE(instruction);
  EXPECT_EQ("0", instruction->block_id);
  EXPECT_EQ(68, instruction->lineNumber);
}

TEST(TestParser, ParseLine_PointCut) {
  auto line = "cut [0.0] [35, 198]";

  Parser parser;
  auto result = parser.ParseLine(42, line);
  EXPECT_TRUE(std::holds_alternative<std::shared_ptr<Instruction>>(result));
  auto instruction = std::dynamic_pointer_cast<PointCutInstruction>(std::get<std::shared_ptr<Instruction>>(result));
  EXPECT_TRUE(instruction);
  EXPECT_EQ("0.0", instruction->block_id);
  EXPECT_EQ(35, instruction->point.px);
  EXPECT_EQ(198, instruction->point.py);
}

TEST(TestParser, ParseLine_Merge) {
  auto line = "merge [0.0.2] [0.0.3]";

  Parser parser;
  auto result = parser.ParseLine(42, line);
  EXPECT_TRUE(std::holds_alternative<std::shared_ptr<Instruction>>(result));
  auto instruction = std::dynamic_pointer_cast<MergeInstruction>(std::get<std::shared_ptr<Instruction>>(result));
  EXPECT_TRUE(instruction);
  EXPECT_EQ("0.0.2", instruction->block_id1);
  EXPECT_EQ("0.0.3", instruction->block_id2);
}

TEST(TestParser, ParseLine_Swap) {
  auto line = "swap [0.0.0] [0.0.1]";

  Parser parser;
  auto result = parser.ParseLine(42, line);
  EXPECT_TRUE(std::holds_alternative<std::shared_ptr<Instruction>>(result));
  auto instruction = std::dynamic_pointer_cast<SwapInstruction>(std::get<std::shared_ptr<Instruction>>(result));
  EXPECT_TRUE(instruction);
  EXPECT_EQ("0.0.0", instruction->block_id1);
  EXPECT_EQ("0.0.1", instruction->block_id2);
}

// vim:ts=2 sw=2 sts=2 et ci
