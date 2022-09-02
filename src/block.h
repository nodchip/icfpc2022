#pragma once

struct Block
{
    virtual ~Block() = default;
};

struct ComplexBlock : public Block
{
};

struct SimpleBlock : public Block
{
};
