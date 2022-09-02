#pragma once
#include <vector>

#include "color.h"
#include "canvas.h"
#include "point.h"

using Frame = std::vector<RGBA>;

struct Painter {

  static Frame draw(const Canvas& canvas) {
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

};
