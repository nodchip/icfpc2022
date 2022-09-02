#include "stdafx.h"
#include "instruction.h"
#include "instruction_cost_calculator.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

TEST(TestInstruction, ToString) {
  EXPECT_EQ(ColorInstruction("0.1", Color(1, 2, 3, 4)).toString(), "color [0.1] [1, 2, 3, 4]");
  EXPECT_EQ(PointCutInstruction("0.1", Point(1, 2)).toString(), "cut [0.1] [1, 2]");
  EXPECT_EQ(VerticalCutInstruction("0.1", 1).toString(), "cut [0.1] [x] [1]");
  EXPECT_EQ(HorizontalCutInstruction("0.1", 2).toString(), "cut [0.1] [y] [2]");
  EXPECT_EQ(SwapInstruction("0.1", "0.2").toString(), "swap [0.1] [0.2]");
  EXPECT_EQ(MergeInstruction("1.1", "0.2").toString(), "merge [1.1] [0.2]");
}

TEST(TestInstruction, Cost) {
  EXPECT_EQ(getCost(ColorInstruction("0.1", Color(1, 2, 3, 4)), 7,  13), 9);
  EXPECT_EQ(getCost(PointCutInstruction("0.1", Point(1, 2)),    1,  10), 100);
  EXPECT_EQ(getCost(VerticalCutInstruction("0.1", 1),           1,  1),  7);
  EXPECT_EQ(getCost(HorizontalCutInstruction("0.1", 2),         7,  15), 15);
  EXPECT_EQ(getCost(SwapInstruction("0.1", "0.2"),              14, 15), 3);
  EXPECT_EQ(getCost(MergeInstruction("1.1", "0.2"),             1,  1),  1);
}

// vim:ts=2 sw=2 sts=2 et ci
