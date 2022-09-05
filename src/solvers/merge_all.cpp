#include "stdafx.h"
#include <thread>
#include <fmt/format.h>
#include "solver_registry.h"
#include "instruction.h"
#include "timer.h"

class MergeAll : public SolverBase {
public:
  struct Option : public OptionBase {
    bool emit_color = true;
    void setOptionParser(CLI::App* app) override {
      app->add_flag("--merge-all-emit-color,!--merge-all-no-emit-color", emit_color);
    }
  };
  virtual OptionBase::Ptr createOption() { return std::make_shared<Option>(); }

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
      if (getOption<Option>()->emit_color) {
        LOG(INFO) << "emit_color";
        instructions.push_back(std::make_shared<ColorInstruction>(std::to_string(top_id), RGBA(255, 255, 255, 255)));
      }
    }

    ret.solution = instructions;
    return ret;

  }
};
REGISTER_SOLVER_WITH_OPTION("MergeAll", MergeAll, MergeAll::Option);

// 細かいものを併合していく。コスト的にMergeAllよりも不利。
// pid=32でinst cost Pyramid:43695 Merge:13641
class PyramidMergeAll : public SolverBase {
public:
  PyramidMergeAll() { }
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
        while (row_ids.size() > 1) {
          std::vector<int> next;
          for (int i = 0; i < row_ids.size(); i += 2) {
            auto inst = std::make_shared<MergeInstruction>(std::to_string(row_ids[i]), std::to_string(row_ids[i + 1]));
            instructions.push_back(inst);
            next.push_back(++top_id);
          }
          if (row_ids.size() % 2 == 1) {
            auto inst = std::make_shared<MergeInstruction>(std::to_string(top_id), std::to_string(row_ids.back()));
            instructions.push_back(inst);
            next.push_back(++top_id);
          }
          LOG(INFO) << row_ids.size() << " " << next.size();
          std::swap(next, row_ids);
        }
        assert(row_ids.size() == 1);
        col_ids.push_back(row_ids.front());
      }
      while (col_ids.size() > 1) {
        std::vector<int> next;
        for (int i = 0; i < col_ids.size(); i += 2) {
          auto inst = std::make_shared<MergeInstruction>(std::to_string(col_ids[i]), std::to_string(col_ids[i + 1]));
          instructions.push_back(inst);
          next.push_back(++top_id);
        }
        if (col_ids.size() % 2 == 1) {
          auto inst = std::make_shared<MergeInstruction>(std::to_string(top_id), std::to_string(col_ids.back()));
          instructions.push_back(inst);
          next.push_back(++top_id);
        }
        std::swap(next, col_ids);
      }
      instructions.push_back(std::make_shared<ColorInstruction>(std::to_string(top_id), RGBA(255, 255, 255, 255)));
    }

    ret.solution = instructions;
    return ret;

  }
};
REGISTER_SOLVER("PyramidMergeAll", PyramidMergeAll);

// vim:ts=2 sw=2 sts=2 et ci

