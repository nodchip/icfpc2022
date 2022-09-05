#include "stdafx.h"
#include "painter.h"
#include <array>
#include <cassert>
#include <immintrin.h>
#include <fmt/format.h>
#include "similarity_checker.h"
#include "lodepng.h"

Frame Painter::draw(const Canvas& canvas, bool canvasOriginAtBottomLeftOfFrame) {
  const auto& blocks = canvas.simplify();
  Frame frame(canvas.width * canvas.height);
  int size = 0;
  for (const auto& block : blocks) {
    const Point frameTopLeft(block->bottomLeft.px  , canvasOriginAtBottomLeftOfFrame ? canvas.height - block->topRight.py   : block->bottomLeft.py);
    const Point frameBottomRight(block->topRight.px, canvasOriginAtBottomLeftOfFrame ? canvas.height - block->bottomLeft.py : block->topRight.py  );
    size += (frameBottomRight.px - frameTopLeft.px) * (frameBottomRight.py - frameTopLeft.py);
    for (int y = frameTopLeft.py; y < frameBottomRight.py; y++) {
      for (int x = frameTopLeft.px; x < frameBottomRight.px; x++) {
        frame[y * canvas.width + x] = block->color;
      }
    }
  }
  return frame;
}

const RGBA& Painting::operator()(int x, int y) const {
  return const_cast<Painting*>(this)->operator()(x, y);
}

RGBA& Painting::operator()(int x, int y) {
  assert(0 <= x && x < width);
  assert(0 <= y && y < height);
  assert(x + y * width < frame.size());
  return frame[x + y * width];
}

PaintingPtr loadPaintingFromFile(std::string file_path) {
  // WIDTH HEIGHT
  // R G B A    R G B A    R G B A  .... -> WIDTH
  // R G B A    R G B A    R G B A  ....
  // |
  // v
  // HEIGHT
  //
  // 最初の要素(画像の左上)はx=0, y=H-1
  // 次はx=1, y=H-1, ..
  auto result = std::make_shared<Painting>();

  std::ifstream ifs(file_path);
  if (ifs.bad() || ifs.eof()) return nullptr;
  ifs >> result->width >> result->height;
  if (result->width <= 0) return nullptr;
  if (result->height <= 0) return nullptr;
  result->frame.resize(result->width * result->height);
  for (int y = 0; y < result->height; ++y) {
    for (int x = 0; x < result->width; ++x) {
      int r = 0, g = 0, b = 0, a = 0;
      ifs >> r >> g >> b >> a;
      (*result)(x, result->height - 1 - y) = RGBA(r, g, b, a);
    }
  }
  if (ifs.bad()) return nullptr;

  return result;
}

PaintingPtr loadPaintingFromPNGFile(std::string file_path) {
  std::vector<uint8_t> buf;
  unsigned int w = 0, h = 0;
  if (lodepng::decode(buf, w, h, file_path) != 0) {
    LOG(ERROR) << "failed to open PNG : " << file_path;
    return nullptr;
  }

  auto result = std::make_shared<Painting>();
  result->width = w;
  result->height = h;
  if (result->width <= 0) return nullptr;
  if (result->height <= 0) return nullptr;
  result->frame.resize(result->width * result->height);
  for (int y = 0; y < result->height; ++y) {
    for (int x = 0; x < result->width; ++x) {
      (*result)(x, result->height - 1 - y) = RGBA(
        buf[4 * (w * y + x) + 0],
        buf[4 * (w * y + x) + 1],
        buf[4 * (w * y + x) + 2],
        buf[4 * (w * y + x) + 3]);
    }
  }
  return result;
}


// 半開区間
std::optional<RGBA> meanColor(const Painting& painting, Point bottomLeft, Point topRight) {
  int64_t rgba[4] = {0, 0, 0, 0};
  int64_t count = 0;
  for (int y = bottomLeft.py; y < topRight.py; ++y) {
    for (int x = bottomLeft.px; x < topRight.px; ++x) {
      ++count;
      for (int c = 0; c < 4; ++c) {
        rgba[c] += painting(x, y)[c];
      }
    }
  }
  if (count > 0) {
    return RGBA(rgba[0] / count, rgba[1] / count, rgba[2] / count, rgba[3] / count);
  }
  return std::nullopt;
}

std::optional<RGBA> geometricMedianColor(const Painting& painting, Point bottomLeft, Point topRight, bool finalAdjustment, int maxIter) {
  alignas(32) std::array<double, 4> estimate;
  auto mean = meanColor(painting, bottomLeft, topRight);
  if (!mean) return std::nullopt;
  estimate[0] = (*mean)[0];
  estimate[1] = (*mean)[1];
  estimate[2] = (*mean)[2];
  estimate[3] = (*mean)[3];

  const size_t N = topRight.subtract(bottomLeft).getScalarSize();
  std::vector<RGBA, AlignedAllocator<RGBA, 32>> colors(N);
  size_t i = 0;
  for (int y = bottomLeft.py; y < topRight.py; ++y) {
    for (int x = bottomLeft.px; x < topRight.px; ++x) {
      colors[i++] = painting(x, y);
    }
  }

  for (int iter = 0; iter < maxIter; ++iter) {
    std::array<double, 4> result = { 0, 0, 0, 0 };
    size_t i = 0;

    __m256d estimate_pd = _mm256_load_pd(&estimate[0]);
    __m256d eps_pd = _mm256_set_pd(1e-6, 1e-6, 1e-6, 1e-6);
    __m256d one_pd = _mm256_set_pd(1.0, 1.0, 1.0, 1.0);
    __m256d result_pd = _mm256_setzero_pd();
    __m256d sum_weights_pd = _mm256_setzero_pd();
    for (; i + 8 <= N; i += 8) {
      __m256i colors_u8 = _mm256_load_si256((__m256i*)&colors[i]);

      // uint8_t -> int32_t
      // __m256i _mm256_cvtepu8_epi32(__m128i s1);
      __m256i colors_0_i32 = _mm256_cvtepu8_epi32(_mm256_castsi256_si128(colors_u8));
      __m256i colors_1_i32 = _mm256_cvtepu8_epi32(_mm256_castsi256_si128(_mm256_srli_si256(colors_u8, 8)));
      __m256i colors_2_i32 = _mm256_cvtepu8_epi32(_mm256_extracti128_si256(colors_u8, 1));
      __m256i colors_3_i32 = _mm256_cvtepu8_epi32(_mm256_extracti128_si256(_mm256_srli_si256(colors_u8, 8), 1));

      // int32_t -> double
      // __m256 _mm256_cvtepi32_pd(__m128i m1);
      __m256d colors_0_pd = _mm256_cvtepi32_pd(_mm256_extracti128_si256(colors_0_i32, 0));
      __m256d colors_1_pd = _mm256_cvtepi32_pd(_mm256_extracti128_si256(colors_0_i32, 1));
      __m256d colors_2_pd = _mm256_cvtepi32_pd(_mm256_extracti128_si256(colors_1_i32, 0));
      __m256d colors_3_pd = _mm256_cvtepi32_pd(_mm256_extracti128_si256(colors_1_i32, 1));
      __m256d colors_4_pd = _mm256_cvtepi32_pd(_mm256_extracti128_si256(colors_2_i32, 0));
      __m256d colors_5_pd = _mm256_cvtepi32_pd(_mm256_extracti128_si256(colors_2_i32, 1));
      __m256d colors_6_pd = _mm256_cvtepi32_pd(_mm256_extracti128_si256(colors_3_i32, 0));
      __m256d colors_7_pd = _mm256_cvtepi32_pd(_mm256_extracti128_si256(colors_3_i32, 1));

      // colors[i] - estimate;
      __m256d sub_0_pd = _mm256_sub_pd(colors_0_pd, estimate_pd);
      __m256d sub_1_pd = _mm256_sub_pd(colors_1_pd, estimate_pd);
      __m256d sub_2_pd = _mm256_sub_pd(colors_2_pd, estimate_pd);
      __m256d sub_3_pd = _mm256_sub_pd(colors_3_pd, estimate_pd);
      __m256d sub_4_pd = _mm256_sub_pd(colors_4_pd, estimate_pd);
      __m256d sub_5_pd = _mm256_sub_pd(colors_5_pd, estimate_pd);
      __m256d sub_6_pd = _mm256_sub_pd(colors_6_pd, estimate_pd);
      __m256d sub_7_pd = _mm256_sub_pd(colors_7_pd, estimate_pd);

      // (colors[i] - estimate) * (colors[i] - estimate)
      __m256d square_0_pd = _mm256_mul_pd(sub_0_pd, sub_0_pd);
      __m256d square_1_pd = _mm256_mul_pd(sub_1_pd, sub_1_pd);
      __m256d square_2_pd = _mm256_mul_pd(sub_2_pd, sub_2_pd);
      __m256d square_3_pd = _mm256_mul_pd(sub_3_pd, sub_3_pd);
      __m256d square_4_pd = _mm256_mul_pd(sub_4_pd, sub_4_pd);
      __m256d square_5_pd = _mm256_mul_pd(sub_5_pd, sub_5_pd);
      __m256d square_6_pd = _mm256_mul_pd(sub_6_pd, sub_6_pd);
      __m256d square_7_pd = _mm256_mul_pd(sub_7_pd, sub_7_pd);

      // hadd
      // a0 a1 a2 b3
      // b0 b1 b2 b3
      //
      // a0+a1 b0+b1 a2+a3 b2+b3
      __m256d hadd_0_pd = _mm256_hadd_pd(square_0_pd, square_1_pd);
      __m256d hadd_1_pd = _mm256_hadd_pd(square_2_pd, square_3_pd);
      __m256d hadd_2_pd = _mm256_hadd_pd(square_4_pd, square_5_pd);
      __m256d hadd_3_pd = _mm256_hadd_pd(square_6_pd, square_7_pd);

      // a0+a1+a2+a3 b0+b1+b2+b3
      __m128d sum_2_0_pd = _mm_add_pd(_mm256_extractf128_pd(hadd_0_pd, 0), _mm256_extractf128_pd(hadd_0_pd, 1));
      __m128d sum_2_1_pd = _mm_add_pd(_mm256_extractf128_pd(hadd_1_pd, 0), _mm256_extractf128_pd(hadd_1_pd, 1));
      __m128d sum_2_2_pd = _mm_add_pd(_mm256_extractf128_pd(hadd_2_pd, 0), _mm256_extractf128_pd(hadd_2_pd, 1));
      __m128d sum_2_3_pd = _mm_add_pd(_mm256_extractf128_pd(hadd_3_pd, 0), _mm256_extractf128_pd(hadd_3_pd, 1));

      // a0+a1+a2+a3 b0+b1+b2+b3 c0+c1+c2+c3 d0+d1+d2+d3
      __m256d sum_4_0_pd = _mm256_castpd128_pd256(sum_2_0_pd);
      sum_4_0_pd = _mm256_insertf128_pd(sum_4_0_pd, sum_2_1_pd, 1);
      __m256d sum_4_1_pd = _mm256_castpd128_pd256(sum_2_2_pd);
      sum_4_1_pd = _mm256_insertf128_pd(sum_4_1_pd, sum_2_3_pd, 1);

      // a0+a1+a2+a3+eps b0+b1+b2+b3+eps c0+c1+c2+c3+eps d0+d1+d2+d3+eps
      sum_4_0_pd = _mm256_add_pd(sum_4_0_pd, eps_pd);
      sum_4_1_pd = _mm256_add_pd(sum_4_1_pd, eps_pd);

      // sqrt
      __m256d sqrt_0_pd = _mm256_sqrt_pd(sum_4_0_pd);
      __m256d sqrt_1_pd = _mm256_sqrt_pd(sum_4_1_pd);

      // weight
      __m256d weight_0_pd = _mm256_div_pd(one_pd, sqrt_0_pd);
      __m256d weight_1_pd = _mm256_div_pd(one_pd, sqrt_1_pd);

      // shuffle
      // a0 a1 a2 a3

      // a0 a0 a2 a2
      // a1 a1 a3 a3
      __m256d shuffle_0_pd = _mm256_shuffle_pd(weight_0_pd, weight_0_pd, 0b0000);
      __m256d shuffle_1_pd = _mm256_shuffle_pd(weight_0_pd, weight_0_pd, 0b1111);
      __m256d shuffle_2_pd = _mm256_shuffle_pd(weight_1_pd, weight_1_pd, 0b0000);
      __m256d shuffle_3_pd = _mm256_shuffle_pd(weight_1_pd, weight_1_pd, 0b1111);

      // weight
      // a0 a0 a2 a2
      // a1 a1 a3 a3

      // a0 a0 a0 a0
      // a1 a1 a1 a1
      // a2 a2 a2 a2
      // a3 a3 a3 a3
      __m256d weight_4_0_pd = _mm256_insertf128_pd(shuffle_0_pd, _mm256_extractf128_pd(shuffle_0_pd, 0), 1);
      __m256d weight_4_1_pd = _mm256_insertf128_pd(shuffle_1_pd, _mm256_extractf128_pd(shuffle_1_pd, 0), 1);
      __m256d weight_4_2_pd = _mm256_insertf128_pd(shuffle_0_pd, _mm256_extractf128_pd(shuffle_0_pd, 1), 0);
      __m256d weight_4_3_pd = _mm256_insertf128_pd(shuffle_1_pd, _mm256_extractf128_pd(shuffle_1_pd, 1), 0);
      __m256d weight_4_4_pd = _mm256_insertf128_pd(shuffle_2_pd, _mm256_extractf128_pd(shuffle_2_pd, 0), 1);
      __m256d weight_4_5_pd = _mm256_insertf128_pd(shuffle_3_pd, _mm256_extractf128_pd(shuffle_3_pd, 0), 1);
      __m256d weight_4_6_pd = _mm256_insertf128_pd(shuffle_2_pd, _mm256_extractf128_pd(shuffle_2_pd, 1), 0);
      __m256d weight_4_7_pd = _mm256_insertf128_pd(shuffle_3_pd, _mm256_extractf128_pd(shuffle_3_pd, 1), 0);

      // colors[i] * weight
      __m256d colors_weight_0_pd = _mm256_mul_pd(colors_0_pd, weight_4_0_pd);
      __m256d colors_weight_1_pd = _mm256_mul_pd(colors_1_pd, weight_4_1_pd);
      __m256d colors_weight_2_pd = _mm256_mul_pd(colors_2_pd, weight_4_2_pd);
      __m256d colors_weight_3_pd = _mm256_mul_pd(colors_3_pd, weight_4_3_pd);
      __m256d colors_weight_4_pd = _mm256_mul_pd(colors_4_pd, weight_4_4_pd);
      __m256d colors_weight_5_pd = _mm256_mul_pd(colors_5_pd, weight_4_5_pd);
      __m256d colors_weight_6_pd = _mm256_mul_pd(colors_6_pd, weight_4_6_pd);
      __m256d colors_weight_7_pd = _mm256_mul_pd(colors_7_pd, weight_4_7_pd);

      result_pd = _mm256_add_pd(result_pd, colors_weight_0_pd);
      result_pd = _mm256_add_pd(result_pd, colors_weight_1_pd);
      result_pd = _mm256_add_pd(result_pd, colors_weight_2_pd);
      result_pd = _mm256_add_pd(result_pd, colors_weight_3_pd);
      result_pd = _mm256_add_pd(result_pd, colors_weight_4_pd);
      result_pd = _mm256_add_pd(result_pd, colors_weight_5_pd);
      result_pd = _mm256_add_pd(result_pd, colors_weight_6_pd);
      result_pd = _mm256_add_pd(result_pd, colors_weight_7_pd);

      sum_weights_pd = _mm256_add_pd(sum_weights_pd, weight_0_pd);
      sum_weights_pd = _mm256_add_pd(sum_weights_pd, weight_1_pd);
    }

    // hadd
    // a0 a1 a2 a3

    // a0+a1 a0+a1 a2+a3 a2+a3
    __m256d sum_weights_hadd_pd = _mm256_hadd_pd(sum_weights_pd, sum_weights_pd);
    __m128d sum_weights_2_pd = _mm_add_pd(_mm256_castpd256_pd128(sum_weights_hadd_pd), _mm256_extractf128_pd(sum_weights_hadd_pd, 1));
    double sum_weights = _mm_cvtsd_f64(sum_weights_2_pd);

    _mm256_store_pd(&result[0], result_pd);

    //double sum_weights = 0.0;

    for (; i < N; ++i) {
      auto rDist = (colors[i][0] - estimate[0]) * (colors[i][0] - estimate[0]);
      auto gDist = (colors[i][1] - estimate[1]) * (colors[i][1] - estimate[1]);
      auto bDist = (colors[i][2] - estimate[2]) * (colors[i][2] - estimate[2]);
      auto aDist = (colors[i][3] - estimate[3]) * (colors[i][3] - estimate[3]);
      auto weight = 1.0 / std::sqrt(rDist + gDist + bDist + aDist + 1e-6);
      result[0] += colors[i][0] * weight;
      result[1] += colors[i][1] * weight;
      result[2] += colors[i][2] * weight;
      result[3] += colors[i][3] * weight;
      sum_weights += weight;
    }
    result[0] /= sum_weights;
    result[1] /= sum_weights;
    result[2] /= sum_weights;
    result[3] /= sum_weights;
    estimate = result;
  }

  const RGBA round_color(int(std::round(estimate[0])), int(std::round(estimate[1])), int(std::round(estimate[2])), int(std::round(estimate[3])));
  if (finalAdjustment) {
    const RGBA corner(int(std::floor(estimate[0])), int(std::floor(estimate[1])), int(std::floor(estimate[2])), int(std::floor(estimate[3])));
    RGBA best_color = corner;
    int best_score = std::numeric_limits<int>::max();
    auto calc_score = [&](RGBA color) {
      double dscore = 0;
      for (int y = bottomLeft.py; y < topRight.py; ++y) {
        for (int x = bottomLeft.px; x < topRight.px; ++x) {
          dscore += SimilarityChecker::pixelDiff(color, painting(x, y));
        }
      }
      constexpr double alpha = 0.005;
      const int score = std::round(alpha * dscore);
      return score;
    };
    for (int i = 0; i < 16; ++i) {
      const RGBA candidate(
        std::clamp(corner[0] + i % 2    , 0, 255),
        std::clamp(corner[1] + i / 2 % 2, 0, 255),
        std::clamp(corner[2] + i / 4 % 2, 0, 255),
        std::clamp(corner[3] + i / 8 % 2, 0, 255));
      const int score = calc_score(candidate);
      if (score < best_score) {
        best_score = score;
        best_color = candidate;
      }
    }
    assert (best_score <= calc_score(round_color));
    if (false && best_score < calc_score(round_color)) { // 煩雑すぎるのでオフ
      LOG(INFO) << fmt::format("improvement {}({}) -> {}({})", std::to_string(round_color), calc_score(round_color), std::to_string(best_color), best_score);
    }
    return best_color;
  } else {
    return round_color;
  }
}

GeometricMedianColorCache::GeometricMedianColorCache(const Painting& painting) : painting(painting) {} 
GeometricMedianColorCache::~GeometricMedianColorCache() {
  LOG(INFO) << fmt::format("color cache size={}, hit={}({:.1f}%), miss={}({:.1f}%)",
    cache.size(),
    hit_count, 100.0 * hit_count / (hit_count + miss_count),
    miss_count, 100.0 * miss_count / (hit_count + miss_count));
}
std::optional<RGBA> GeometricMedianColorCache::getColor(Point bottomLeft, Point topRight) {
  assert(0 <= bottomLeft.px && bottomLeft.px <= 0xffff);
  assert(0 <= bottomLeft.py && bottomLeft.py <= 0xffff);
  assert(0 <= topRight.px && topRight.px <= 0xffff);
  assert(0 <= topRight.py && topRight.py <= 0xffff);
  const auto key = (uint64_t(bottomLeft.px & 0xffff) << 24) | (uint64_t(bottomLeft.py & 0xffff) << 16) | (uint64_t(topRight.px & 0xffff) << 8) | uint64_t(topRight.py & 0xffff);
  auto it = cache.find(key);
  std::optional<RGBA> color;
  if (it == cache.end()) {
    ++miss_count;
    color = geometricMedianColor(painting, bottomLeft, topRight, true);
    cache.insert({key, color});
  } else {
    ++hit_count;
    color = it->second;
  }
  return color;
}
