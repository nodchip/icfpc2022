#include "stdafx.h"
#include "painter.h"
#include <cassert>

Frame Painter::draw(const Canvas& canvas) {
  const auto& blocks = canvas.simplify();
  Frame frame(canvas.width * canvas.height);
  int size = 0;
  for (const auto& block : blocks) {
    const Point frameTopLeft(block->bottomLeft.px, canvas.height - block->topRight.py);
    const Point frameBottomRight(block->topRight.px, canvas.height - block->bottomLeft.py);
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
  // 最初の要素はx=0, y=0次はx=1, y=0, ..
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
      (*result)(x, y) = RGBA(r, g, b, a);
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
