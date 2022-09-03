#pragma once
#include <vector>
#include <optional>

#include "color.h"
#include "canvas.h"
#include "point.h"

using Frame = std::vector<RGBA>;

struct Painter {

  static Frame draw(const Canvas& canvas);

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
