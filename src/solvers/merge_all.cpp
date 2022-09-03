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

    // problem 26~30 限定かも

    auto canvas = args.canvas->Clone();

    // block id は 左下->右上, 列スキャン
    std::vector<std::shared_ptr<Instruction>> instructions;

    if (canvas->blocks.size() == 100) {
      // 40x40 のブロックが 100 個
      // merge 100=[0,1],101=[100,2],102=[101,3],...
      int top_id = 99;
      std::vector<int> col_ids;
      for (int col_begin_id = 0; col_begin_id < 100; col_begin_id += 10) {
        std::vector<int> row_ids(10);
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
    else if (canvas->blocks.size() == 400) {
      // 20x20 のブロックが 400 個
      int top_id = 399;
      std::vector<int> col_ids;
      for (int col_begin_id = 0; col_begin_id < 400; col_begin_id += 20) {
        std::vector<int> row_ids(20);
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
      instructions.push_back(
        std::make_shared<ColorInstruction>(
          std::to_string(top_id),
          //*meanColor(*args.painting, Point(0, 0), Point(args.painting->width, args.painting->height))
          //*geometricMedianColor(*args.painting, Point(0, 0), Point(args.painting->width, args.painting->height))
          RGBA(255, 255, 255, 255)
          )
      );
    }

    // dummy.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    ret.solution = instructions;
    return ret;

  }
};
REGISTER_SOLVER("MergeAll", MergeAll);
// vim:ts=2 sw=2 sts=2 et ci

