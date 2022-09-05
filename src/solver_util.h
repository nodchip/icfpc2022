#pragma once
#include <vector>
#include <memory>

struct GeometricMedianColorCache;
struct Painting;
struct Canvas;
using CanvasPtr = std::shared_ptr<Canvas>;
struct Instruction;

std::vector<std::shared_ptr<Instruction>> removeAdverseInstruction(GeometricMedianColorCache& cache, Painting& painting, CanvasPtr initial_canvas, const std::vector<std::shared_ptr<Instruction>>& instructions);
std::vector<std::shared_ptr<Instruction>> replaceColorInstructionOptimal(GeometricMedianColorCache& cache, Painting& painting, CanvasPtr initial_canvas, const std::vector<std::shared_ptr<Instruction>>& instructions);

