#include "stdafx.h"
#include "similarity_checker.h"

#include <immintrin.h>

namespace {
  double SQRT_TABLE[255 * 255 * 4 + 1];
}

void SimilarityChecker::Initialize() {
  for (int i = 0; i < std::size(SQRT_TABLE); ++i) {
    SQRT_TABLE[i] = std::sqrt(i);
  }
}

double SimilarityChecker::pixelDiff(const RGBA& p1, const RGBA& p2) {
  const int rDist = (p1[0] - p2[0]) * (p1[0] - p2[0]);
  const int gDist = (p1[1] - p2[1]) * (p1[1] - p2[1]);
  const int bDist = (p1[2] - p2[2]) * (p1[2] - p2[2]);
  const int aDist = (p1[3] - p2[3]) * (p1[3] - p2[3]);
  return SQRT_TABLE[rDist + gDist + bDist + aDist];
}

int SimilarityChecker::imageDiff(const Frame& f1, const Frame& f2) {
  int i = 0;
  __m256d diff_pd = _mm256_setzero_pd();
  for (; i + 8 <= f1.size(); i += 8) {
    __m256i f1_u8 = _mm256_load_si256((__m256i*)&f1[i]);
    __m256i f2_u8 = _mm256_load_si256((__m256i*)&f2[i]);

    // uint8_t -> int16_t
    __m256i f1_lo_i16 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(f1_u8, 0));
    __m256i f1_hi_i16 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(f1_u8, 1));
    __m256i f2_lo_i16 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(f2_u8, 0));
    __m256i f2_hi_i16 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(f2_u8, 1));

    // f1 - f2
    __m256i sub_lo_i16 = _mm256_sub_epi16(f1_lo_i16, f2_lo_i16);
    __m256i sub_hi_i16 = _mm256_sub_epi16(f1_hi_i16, f2_hi_i16);

    // (f1 - f2) * (f1 - f2)
    __m256i mul_lo_0_i16 = _mm256_mullo_epi16(sub_lo_i16, sub_lo_i16);
    __m256i mul_hi_0_i16 = _mm256_mulhi_epi16(sub_lo_i16, sub_lo_i16);
    __m256i mul_lo_1_i16 = _mm256_mullo_epi16(sub_hi_i16, sub_hi_i16);
    __m256i mul_hi_1_i16 = _mm256_mulhi_epi16(sub_hi_i16, sub_hi_i16);

    // unpack
    __m256i mul_0_i32 = _mm256_unpacklo_epi16(mul_lo_0_i16, mul_hi_0_i16);
    __m256i mul_1_i32 = _mm256_unpackhi_epi16(mul_lo_0_i16, mul_hi_0_i16);
    __m256i mul_2_i32 = _mm256_unpacklo_epi16(mul_lo_1_i16, mul_hi_1_i16);
    __m256i mul_3_i32 = _mm256_unpackhi_epi16(mul_lo_1_i16, mul_hi_1_i16);

    // hadd
    // a0 a1 a2 a3 a4 a5 a6 a7
    // b0 b1 b2 b3 b4 b5 b6 b7
    // c0 c1 c2 c3 c4 c5 c6 c7
    // d0 d1 d2 d3 d4 d5 d6 d7

    // a0+a1 a2+a3 b0+b1 b2+b3 a4+a5 a6+a7 b4+b5 b6+b7
    // c0+c1 c2+c3 d0+d1 d2+d3 c4+c5 c6+c7 d4+d5 d6+d7
    __m256i hadd_0_1_i32 = _mm256_hadd_epi32(mul_0_i32, mul_1_i32);
    __m256i hadd_2_3_i32 = _mm256_hadd_epi32(mul_2_i32, mul_3_i32);

    // a0+a1+a2+a3 b0+b1+b2+b3 c0+c1+c2+c3 d0+d1+d2+d3 a4+a5+a6+a7 b4+b5+b6+b7 c4+c5+c6+c7 d4+d5+d6+d7
    __m256i hadd_0_1_2_3_i32 = _mm256_hadd_epi32(hadd_0_1_i32, hadd_2_3_i32);

    __m256d sqrt_0_pd = _mm256_i32gather_pd(SQRT_TABLE, _mm256_extracti128_si256(hadd_0_1_2_3_i32, 0), 8);
    __m256d sqrt_1_pd = _mm256_i32gather_pd(SQRT_TABLE, _mm256_extracti128_si256(hadd_0_1_2_3_i32, 1), 8);
    diff_pd = _mm256_add_pd(diff_pd, sqrt_0_pd);
    diff_pd = _mm256_add_pd(diff_pd, sqrt_1_pd);
  }
  diff_pd = _mm256_hadd_pd(diff_pd, diff_pd);
  double diff = ((double*)&diff_pd)[0] + ((double*)&diff_pd)[2];
  //double diff = 0;
  for (; i < f1.size(); ++i) {
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
