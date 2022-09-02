#pragma once

#include <string>

#include "canvas.h"
#include "point.h"

struct Instruction
{
    virtual ~Instruction() = default;
};

struct ColorInstruction : public Instruction
{
    std::string block_id;
    Color color;
};

struct PointCutInstruction : public Instruction
{
    std::string block_id;
    Point point;
};

struct VerticalCutInstruction : public Instruction
{
    std::string block_id;
    int lineNumber;
};

struct HorizontalCutInstruction : public Instruction
{
    std::string block_id;
    int lineNumber;
};

struct SwapInstruction : public Instruction
{
    std::string block_id1;
    std::string block_id2;
};

struct MergeInstruction : public Instruction
{
    std::string block_id1;
    std::string block_id2;
};
