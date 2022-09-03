#include "stdafx.h"
#include "painter.h"
#include <array>
#include <cassert>

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
  if (result->width < 0) return nullptr;
  if (result->height < 0) return nullptr;
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

// 半開区間
std::optional<RGBA> meanColor(const Painting& painting, Point bottomLeft, Point topRight) {
  int64_t rgba[4] = {0, 0, 0, 0};
  int64_t count = 0;
  for (int y = bottomLeft.py; y < topRight.py; ++y) {
    for (int x = bottomLeft.px; x < topRight.px; ++x) {
      ++count;
      rgba[0] += painting(x, y).r;
      rgba[1] += painting(x, y).g;
      rgba[2] += painting(x, y).b;
      rgba[3] += painting(x, y).a;
    }
  }
  if (count > 0) {
    return RGBA(rgba[0] / count, rgba[1] / count, rgba[2] / count, rgba[3] / count);
  }
  return std::nullopt;
}

std::optional<RGBA> geometricMedianColor(const Painting& painting, Point bottomLeft, Point topRight, int maxIter) {
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

  auto weighted_average = [&colors](auto estimate, const auto& weights) {
    std::array<double, 4> result = {0, 0, 0, 0};
    double sum = 0.0;
    for (size_t i = 0; i < colors.size(); ++i) {
      result[0] += colors[i][0] * weights[i];
      result[1] += colors[i][1] * weights[i];
      result[2] += colors[i][2] * weights[i];
      result[3] += colors[i][3] * weights[i];
      sum += weights[i];
    }
    result[0] /= sum;
    result[1] /= sum;
    result[2] /= sum;
    result[3] /= sum;
    return result;
  };
  auto SQ = [](double x) { return x * x; };
  auto dist_l2 = [SQ](const auto& x, const auto& y) {
    return std::sqrt(SQ(x[0] - y[0]) + SQ(x[1] - y[1]) + SQ(x[2] - y[2]) + SQ(x[3] - y[3]));
  };

  std::vector<double> weights(N, 0.0);
  for (int iter = 0; iter < maxIter; ++iter) {
    for (size_t i = 0; i < N; ++i) {
      weights[i] = 1.0 / (dist_l2(colors[i], estimate) + 1e-6);
    }
    estimate = weighted_average(estimate, weights);
  }
  return RGBA(int(std::round(estimate[0])), int(std::round(estimate[1])), int(std::round(estimate[2])), int(std::round(estimate[3])));
}
