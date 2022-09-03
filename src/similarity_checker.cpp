#include "stdafx.h"
#include "similarity_checker.h"

double SimilarityChecker::pixelDiff(const RGBA& p1, const RGBA& p2) {
  const int rDist = (p1[0] - p2[0]) * (p1[0] - p2[0]);
  const int gDist = (p1[1] - p2[1]) * (p1[1] - p2[1]);
  const int bDist = (p1[2] - p2[2]) * (p1[2] - p2[2]);
  const int aDist = (p1[3] - p2[3]) * (p1[3] - p2[3]);
  return sqrt(rDist + gDist + bDist + aDist);
}

int SimilarityChecker::imageDiff(const Frame& f1, const Frame& f2) {
  double diff = 0;
  for (int i = 0; i < f1.size(); i++) {
    diff += pixelDiff(f1[i], f2[i]);
  }
  return (int)round(diff * alpha);
}

int SimilarityChecker::imageDiffinROI(const Painting& p1, const Painting& p2, Point bottomLeft, Point topRight) {
  constexpr double alpha = 0.005;
  double diff = 0;
  for (int y = bottomLeft.py; y < topRight.py; ++y) {
    for (int x = bottomLeft.px; x < topRight.px; ++x) {
      diff += pixelDiff(p1(x, y), p2(x, y));
    }
  }
  return (int)round(diff * alpha);
}
