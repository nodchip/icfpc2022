#include "stdafx.h"
#include "painter.h"
#include <array>
#include <cassert>
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
  std::array<double, 4> estimate;
  auto mean = meanColor(painting, bottomLeft, topRight);
  if (!mean) return std::nullopt;
  estimate[0] = (*mean)[0];
  estimate[1] = (*mean)[1];
  estimate[2] = (*mean)[2];
  estimate[3] = (*mean)[3];

  const size_t N = topRight.subtract(bottomLeft).getScalarSize();
  std::vector<RGBA> colors(N);
  size_t i = 0;
  for (int y = bottomLeft.py; y < topRight.py; ++y) {
    for (int x = bottomLeft.px; x < topRight.px; ++x) {
      colors[i++] = painting(x, y);
    }
  }

  for (int iter = 0; iter < maxIter; ++iter) {
    double sum_weights = 0.0;
    std::array<double, 4> result = { 0, 0, 0, 0 };
    for (size_t i = 0; i < N; ++i) {
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
