#include "stdafx.h"
#include "canvas.h"

#include <fmt/format.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include "similarity_checker.h"

#ifdef _WIN32
#define PROJECT_DIR "../../"
#else
#define PROJECT_DIR "../"
#endif

TEST(TestCanvas, loadCanvasFromJSONFile) {
  auto canvas = loadCanvasFromJSONFile(PROJECT_DIR "data/problems/30.initial.json", false);
  ASSERT_TRUE(canvas);
  EXPECT_EQ(canvas->width, 400);
  EXPECT_EQ(canvas->height, 400);
  EXPECT_EQ(canvas->blocks["318"]->bottomLeft, Point(300, 360));
  EXPECT_EQ(canvas->blocks["317"]->topRight, Point(320, 360));
  auto block = std::dynamic_pointer_cast<SimpleBlock>(canvas->blocks["316"]);
  ASSERT_TRUE(block);
  EXPECT_EQ(block->color, Color(98, 46, 50, 255));
}


