#include "stdafx.h"
#include "painter.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

#ifdef _WIN32
#define PROJECT_DIR "../../"
#else
#define PROJECT_DIR "../"
#endif

TEST(TestPainter, loadPaintingFromFile) {
  PaintingPtr painting = loadPaintingFromFile(PROJECT_DIR "data/problems/8.txt");
  ASSERT_TRUE(painting);
  EXPECT_EQ(painting->width, 400);
  EXPECT_EQ(painting->height, 400);
  EXPECT_EQ((*painting)(73, 345), RGBA(92, 225, 230, 255)); // "F" of ICFCP
}


// vim:ts=2 sw=2 sts=2 et ci
