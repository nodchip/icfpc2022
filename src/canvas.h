#pragma once

#include <map>
#include <memory>

#include "block.h"

struct Canvas
{
    std::map<std::string, std::shared_ptr<Block>> blocks;
};

struct Color
{
    int r;
    int g;
    int b;
    int a;
};
