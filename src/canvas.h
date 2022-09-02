#pragma once

#include <map>
#include <memory>

#include "block.h"
#include "color.h"

using Color = RGBA;

struct Canvas
{
  std::map<std::string, std::shared_ptr<Block>> blocks;
};
