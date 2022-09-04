#pragma once
#include <vector>
#include <optional>

#include "color.h"
#include "canvas.h"
#include "point.h"

using Frame = std::vector<RGBA>;

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

// 半開区間
std::optional<RGBA> meanColor(const Painting& painting, Point bottomLeft, Point topRight);
std::optional<RGBA> geometricMedianColor(const Painting& painting, Point bottomLeft, Point topRight, int maxIter=100);
