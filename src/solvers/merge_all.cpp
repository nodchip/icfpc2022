#include "stdafx.h"
#include <thread>
#include <fmt/format.h>
#include "solver_registry.h"
#include "instruction.h"
#include "timer.h"

class MergeAll : public SolverBase {
public:
  MergeAll() { }
  SolverOutputs solve(const SolverArguments &args) override {

    SolverOutputs ret;

    // 前段がある場合はスルー
    if (!args.optional_initial_solution.empty()) {
      return ret;
    }

    auto canvas = args.previous_canvas->Clone();

    // block id は 左下->右上, 列スキャン (problem 26~35 限定？)
    std::vector<std::shared_ptr<Instruction>> instructions;

    for (int board_size : {10, 16, 20}) {
      if (canvas->blocks.size() != board_size * board_size) continue;
      int top_id = board_size * board_size - 1;
      std::vector<int> col_ids;
      for (int col_begin_id = 0; col_begin_id < board_size * board_size; col_begin_id += board_size) {
        std::vector<int> row_ids(board_size);
        std::iota(row_ids.begin(), row_ids.end(), col_begin_id);
        int dst_id = row_ids.front();
        for (int i = 1; i < row_ids.size(); i++) {
          int src_id = row_ids[i];
          auto inst = std::make_shared<MergeInstruction>(std::to_string(dst_id), std::to_string(src_id));
          instructions.push_back(inst);
          dst_id = ++top_id;
        }
        col_ids.push_back(dst_id);
      }
      int dst_id = col_ids.front();
      for (int i = 1; i < col_ids.size(); i++) {
        int src_id = col_ids[i];
        auto inst = std::make_shared<MergeInstruction>(std::to_string(dst_id), std::to_string(src_id));
        instructions.push_back(inst);
        dst_id = ++top_id;
      }
      instructions.push_back(std::make_shared<ColorInstruction>(std::to_string(top_id), RGBA(255, 255, 255, 255)));
    }

    ret.solution = instructions;
    return ret;

  }
};
REGISTER_SOLVER("MergeAll", MergeAll);
// vim:ts=2 sw=2 sts=2 et ci
