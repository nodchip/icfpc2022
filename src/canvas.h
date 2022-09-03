#pragma once

#include <map>
#include <memory>

#include "block.h"
#include "color.h"

using Color = RGBA;

struct Canvas
{

  int width;
  int height;
  Color backgroundColor;
  std::map<std::string, std::shared_ptr<Block>> blocks;

  Canvas(int width, int height, const Color& backgroundColor);

  Size size() const;

  std::vector<std::shared_ptr<SimpleBlock>> simplify() const;

};

std::shared_ptr<Canvas> loadCanvasFromJSONFile(const std::string& file_path);
