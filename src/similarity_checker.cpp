#include "stdafx.h"
#include "similarity_checker.h"

double SimilarityChecker::pixelDiff(const RGBA& p1, const RGBA& p2) {
  const int rDist = (p1.r - p2.r) * (p1.r - p2.r);
  const int gDist = (p1.g - p2.g) * (p1.g - p2.g);
  const int bDist = (p1.b - p2.b) * (p1.b - p2.b);
  const int aDist = (p1.a - p2.a) * (p1.a - p2.a);
  return sqrt(rDist + gDist + bDist + aDist);
}

int SimilarityChecker::imageDiff(const Frame& f1, const Frame& f2) {
  double diff = 0;
  for (int i = 0; i < f1.size(); i++) {
    diff += pixelDiff(f1[i], f2[i]);
  }
  return (int)round(diff * alpha);
}
