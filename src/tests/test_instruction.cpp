#include "stdafx.h"
#include "instruction.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

TEST(TestInstruction, Simple) {
  EXPECT_EQ(ColorInstruction("0.1", Color(1, 2, 3, 4)).toString(), "color [0.1] [1, 2, 3, 4]");
  EXPECT_EQ(PointCutInstruction("0.1", Point(1, 2)).toString(), "cut [0.1] [1, 2]");
  EXPECT_EQ(VerticalCutInstruction("0.1", 1).toString(), "cut [0.1] [x] [1]");
  EXPECT_EQ(HorizontalCutInstruction("0.1", 2).toString(), "cut [0.1] [y] [2]");
  EXPECT_EQ(SwapInstruction("0.1", "0.2").toString(), "swap [0.1] [0.2]");
  EXPECT_EQ(MergeInstruction("1.1", "0.2").toString(), "merge [1.1] [0.2]");
}

// vim:ts=2 sw=2 sts=2 et ci
