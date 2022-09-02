#pragma once
#include <vector>

#include "color.h"
#include "canvas.h"
#include "point.h"

using Frame = std::vector<RGBA>;

struct Painter {

  static Frame draw(const Canvas& canvas);

};
