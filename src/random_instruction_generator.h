#pragma once

struct Canvas;
struct Instruction;
struct RGBA;

struct RandomInstructionGenerator
{
  std::shared_ptr<Instruction> GenerateRandomInstruction(const std::shared_ptr<Canvas>& state);
  RGBA GenerateRandomColor();

  std::shared_ptr<Instruction> GenerateColorInstruction(const std::shared_ptr<Canvas>& state);
  std::shared_ptr<Instruction> GenerateHorizontalCutInstruction(const std::shared_ptr<Canvas>& state);
  std::shared_ptr<Instruction> GenerateVerticalCutInstruction(const std::shared_ptr<Canvas>& state);
  std::shared_ptr<Instruction> GeneratePointCutInstruction(const std::shared_ptr<Canvas>& state);
  std::shared_ptr<Instruction> GenerateSwapInstruction(const std::shared_ptr<Canvas>& state);
  std::shared_ptr<Instruction> GenerateMergeInstruction(const std::shared_ptr<Canvas>& state);
};
