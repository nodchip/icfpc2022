#include "stdafx.h"
#include "painter.h"

#include <fmt/format.h>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include "similarity_checker.h"

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

TEST(TestPainter, geometricMedianColorSimple) {
  Painting painting;
  painting.width = 2;
  painting.height = 2;
  painting.frame = {
    RGBA(128, 128, 128, 128),
    RGBA(128, 128, 128, 128),
    RGBA(64, 64, 64, 64),
    RGBA(64, 64, 64, 64),
  };
  auto color = geometricMedianColor(painting, Point(0, 0), Point(2, 2), true);
  LOG(INFO) << *color;
  ASSERT_TRUE(color);
  EXPECT_EQ(*color, RGBA(96, 96, 96, 96));
}

TEST(TestPainter, geometricMedianColorBetterThanMean) {
  PaintingPtr painting = loadPaintingFromFile(PROJECT_DIR "data/problems/8.txt");
  ASSERT_TRUE(painting);
  EXPECT_EQ(painting->width, 400);
  EXPECT_EQ(painting->height, 400);
  auto mean_color = meanColor(*painting, Point(0, 0), Point(200, 200));
  auto median_color = geometricMedianColor(*painting, Point(0, 0), Point(200, 200), true);
  Painting mean_painting = *painting;
  mean_painting.frame.assign(mean_painting.frame.size(), *mean_color);
  Painting median_painting = *painting;
  median_painting.frame.assign(median_painting.frame.size(), *median_color);
  auto mean_diff = SimilarityChecker::imageDiffinROI(*painting, mean_painting, Point(0, 0), Point(200, 200));
  auto median_diff = SimilarityChecker::imageDiffinROI(*painting, median_painting, Point(0, 0), Point(200, 200));
  LOG(INFO) << "mean " << *mean_color << " " << mean_diff;
  LOG(INFO) << "median " << *median_color << " " << median_diff;
  EXPECT_LE(median_diff, mean_diff);
}

// vim:ts=2 sw=2 sts=2 et ci
