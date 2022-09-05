#pragma once
#include <vector>
#include <optional>

#include "aligned_allocator.h"
#include "color.h"
#include "canvas.h"
#include "point.h"

// AVX2対応のため、32バイトにアラインする。
using Frame = std::vector<RGBA, AlignedAllocator<RGBA, 32>>;

struct Painter {

  // canvasOriginAtBottomLeftOfFrame: trueのとき、tsコードと同様にcanvasの(x=0, y=0)はframeの左下に、canvasの(x=0, y=H-1)はframe[0]になる。
  // canvasOriginAtBottomLeftOfFrame: false、canvasの(x=0, y=0)はframe[0]になる。
  static Frame draw(const Canvas& canvas, bool canvasOriginAtBottomLeftOfFrame = true);

};

struct Painting {
  int width = 0;
  int height = 0;
  Frame frame;

  const RGBA& operator()(int x, int y) const;
  RGBA& operator()(int x, int y);
};
using PaintingPtr = std::shared_ptr<Painting>;

PaintingPtr loadPaintingFromFile(std::string file_path);
PaintingPtr loadPaintingFromPNGFile(std::string file_path);

// 半開区間
std::optional<RGBA> meanColor(const Painting& painting, Point bottomLeft, Point topRight);
// finalAdjustment = true
//   roundは必ずしもスコアが最適ではないので、2^4=16パターンのうちコスト最小のものを選ぶ
//   iter+=16する程度の計算コストが掛かる
std::optional<RGBA> geometricMedianColor(const Painting& painting, Point bottomLeft, Point topRight, bool finalAdjustment, int maxIter=100);
