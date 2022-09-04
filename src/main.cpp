#include "stdafx.h"

#include <chrono>
#include <filesystem>
#include <iostream>

#include <CLI/CLI.hpp>
#include <fmt/core.h>

#include "parser.h"
#include "painter.h"
#include "instruction_cost_calculator.h"
#include "similarity_checker.h"
#include "interpreter.h"
#include "solver_registry.h"

std::vector<std::string> split(std::string s, char delim) {
  std::vector<std::string> result;
  std::istringstream iss(s);
  std::string token;
  while (std::getline(iss, token, delim)) {
    if (!token.empty()) {
      result.push_back(token);
    }
  }
  return result;
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  google::SetStderrLogging(google::INFO);
  google::SetLogDestination(google::INFO, "main.log.");

  std::ios::sync_with_stdio(false);
  std::cin.tie(NULL);

  CLI::App app{ "main module" };
  app.require_subcommand(0, 1);
  bool list_solvers = false;
  app.add_flag("--list-solvers", list_solvers);

  auto sub_solve = app.add_subcommand("solve");
  std::string solver_names;
  std::string problem_file;
  std::string initial_solution_isl;
  std::string output_solution_isl = "output.isl";
  double timeout_s = -1.0;
  bool visualize = false;
  bool output_phase_isl = true;
  bool output_meta = true;
  sub_solve->add_option("solver_name", solver_names, "solver name or comma-separated list of solver names");
  sub_solve->add_option("problem_file", problem_file, "problem file path");
  sub_solve->add_option("output_solution_isl", output_solution_isl, "output solution ISL file path (optional. default=output.isl)");
  sub_solve->add_option("initial_solution_isl", initial_solution_isl, "input solution ISL file path (optional)");
  sub_solve->add_option("--timeout", timeout_s, "timeout (s). it is up to each solver to follow the timeout or not");
  sub_solve->add_flag("--visualize", visualize, "realtime visualize");
  sub_solve->add_flag("--output-phase,!--no-output-phase", output_phase_isl, "output phase ISL files (outfile.1.isl, outfile.2.isl, ..)");
  sub_solve->add_flag("--output-meta,!--no-output-meta", output_meta, "output meta file");
  SolverRegistry::setOptionParser(sub_solve);

  auto sub_eval = app.add_subcommand("eval");
  sub_eval->add_option("problem_file", problem_file, "problem file path");
  sub_eval->add_option("solution_isl", initial_solution_isl, "input solution ISL file path");

  CLI11_PARSE(app, argc, argv);

  auto loadInitialConfiguration = [&](PaintingPtr problem) {
    std::string config_path = problem_file;
    config_path.replace(config_path.find(".txt"), 4, ".initial.json");
    std::shared_ptr<Canvas> canvas;
    if (std::filesystem::exists(config_path)) {
      canvas = loadCanvasFromJSONFile(config_path);
      if (!canvas) {
        LOG(ERROR) << fmt::format("failed to load config {}", config_path);
        assert(false);
      }
      LOG(INFO) << fmt::format("Config  : {} ({} blocks)", config_path, canvas->blocks.size());
    } else {
      canvas = createLightningCanvas(problem->width, problem->height);
    }
    return canvas;
  };

  auto loadProblem = [&] {
    std::shared_ptr<Painting> problem = loadPaintingFromFile(problem_file);
    if (!problem) {
      LOG(ERROR) << fmt::format("failed to load problem {}", problem_file);
      assert(false);
    }
    LOG(INFO) << fmt::format("Problem  : {} ({}x{})", problem_file, problem->width, problem->height);
    return problem;
  };

  auto loadSolution = [&](const Painting& problem) {
    std::vector<std::shared_ptr<Instruction>> initial_solution;
    if (!initial_solution_isl.empty()) {
      if (!std::filesystem::exists(initial_solution_isl)) {
        LOG(ERROR) << fmt::format("initial solution not found {}", initial_solution_isl);
        assert(false);
      }
      initial_solution = Parser().ParseFile(initial_solution_isl);
      LOG(INFO) << fmt::format("Initial Solution  : {} ({} instructions)", initial_solution_isl, initial_solution.size());
    }
    return initial_solution;
  };

  if (list_solvers) {
    SolverRegistry::displaySolvers();
    return 0;
  }

  if (sub_solve->parsed()) {
    std::vector<std::string> solver_name_list;
    for (auto solver_name : split(solver_names, ',')) {
      solver_name = SolverRegistry::getCanonicalSolverName(solver_name);
      solver_name_list.push_back(solver_name);
    }
    if (solver_name_list.empty()) {
      LOG(ERROR) << fmt::format("no solvers");
      return -1;
    }

    std::shared_ptr<Painting> problem = loadProblem();
    std::shared_ptr<Canvas> initial_canvas = loadInitialConfiguration(problem);
    std::vector<std::shared_ptr<Instruction>> initial_solution = loadSolution(*problem);
    if (!initial_solution.empty()) {
      auto cost = computeCost(*problem, initial_canvas, initial_solution);
      if (!cost) {
        LOG(ERROR) << fmt::format("failed to run the solution! terminating.");
        return -1;
      }
      LOG(INFO) << fmt::format("-----------[ {} ]-----------", initial_solution_isl);
      LOG(INFO) << fmt::format("Inst. Cost : {} ({:.2f} %)", cost->instruction, 100.0 * cost->instruction / cost->total);
      LOG(INFO) << fmt::format(" Sim. Cost : {} ({:.2f} %)", cost->similarity, 100.0 * cost->similarity / cost->total);
      LOG(INFO) << fmt::format("Total Cost : {}", cost->total);
    }

    SolverArguments arg(problem, initial_canvas, initial_canvas);
    arg.optional_initial_solution = initial_solution;
    arg.visualize = visualize;
    if (timeout_s > 0) {
      arg.timeout_s = timeout_s;
    }

    int phase = 1;
    auto output_phase_file_path = [output_solution_isl](int phase) {
      auto ext = output_solution_isl.find(".isl");
      if (ext != std::string::npos) {
        return fmt::format("{}.phase{}.isl", output_solution_isl.substr(0, ext), phase);
      } else {
        return fmt::format("{}.phase{}", output_solution_isl, phase);
      }
    };

    SolverOutputs out;
    for (auto solver_name : solver_name_list) {
      LOG(INFO) << fmt::format("-----------[ {}: {} ]-----------", phase, solver_name);
      auto solver = SolverRegistry::getSolver(solver_name);
      if (!solver) {
        LOG(ERROR) << fmt::format("solver [{0}] not found!", solver_name);
        return -1;
      }
      LOG(ERROR) << fmt::format("Solver   : {} (starting with {} instructions and {} initial / {} previous blocks)", solver_name, arg.optional_initial_solution.size(), arg.initial_canvas->blocks.size(), arg.previous_canvas->blocks.size());

      const auto t0 = std::chrono::system_clock::now();
      out = solver->solve(arg);
      const auto t1 = std::chrono::system_clock::now();
      const double solve_s = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
      LOG(INFO) << fmt::format("Elapsed  : {:.2f} s", solve_s);

      auto cost = computeCost(*problem, initial_canvas, out.solution);
      if (!cost) {
        LOG(ERROR) << fmt::format("failed to run the solution! terminating.");
        return -1;
      }
      LOG(INFO) << fmt::format("Inst. Cost : {} ({:.2f} %)", cost->instruction, 100.0 * cost->instruction / cost->total);
      LOG(INFO) << fmt::format(" Sim. Cost : {} ({:.2f} %)", cost->similarity, 100.0 * cost->similarity / cost->total);
      LOG(INFO) << fmt::format("Total Cost : {}", cost->total);
      out.solution.push_back(std::make_shared<CommentInstruction>(fmt::format("Solver     : {}", solver_name)));
      out.solution.push_back(std::make_shared<CommentInstruction>(fmt::format("Inst. Cost : {} ({:.2f} %)", cost->instruction, 100.0 * cost->instruction / cost->total)));
      out.solution.push_back(std::make_shared<CommentInstruction>(fmt::format(" Sim. Cost : {} ({:.2f} %)", cost->similarity, 100.0 * cost->similarity / cost->total)));
      out.solution.push_back(std::make_shared<CommentInstruction>(fmt::format("Total Cost : {}", cost->total)));
      out.solution.push_back(std::make_shared<CommentInstruction>(fmt::format("Elapsed    : {} s", solve_s)));

      if (output_phase_isl && phase < solver_name_list.size()) {
        auto file_path = output_phase_file_path(phase);
        dumpInstructions(file_path, out.solution);
        LOG(INFO) << fmt::format("Dumped {} instructions to : {}", out.solution.size(), file_path);
      }

      // successive processing.
      arg.optional_initial_solution = out.solution;
      arg.previous_canvas = cost->canvas->Clone();
      ++phase;
    }

    dumpInstructions(output_solution_isl, out.solution);
    LOG(INFO) << fmt::format("Dumped final {} instructions to : {}", out.solution.size(), output_solution_isl);

    return 0;
  }

  if (sub_eval->parsed()) {
    std::shared_ptr<Painting> problem = loadProblem();

    std::vector<std::shared_ptr<Instruction>> solution = loadSolution(*problem);
    std::shared_ptr<Canvas> initial_canvas = loadInitialConfiguration(problem);
    auto cost = computeCost(*problem, initial_canvas, solution);
    if (!cost) {
      LOG(ERROR) << fmt::format("failed to run the solution! terminating.");
      return -1;
    }
    LOG(INFO) << fmt::format("Inst. Cost : {} ({:.2f} %)", cost->instruction, 100.0 * cost->instruction / cost->total);
    LOG(INFO) << fmt::format(" Sim. Cost : {} ({:.2f} %)", cost->similarity, 100.0 * cost->similarity / cost->total);
    LOG(INFO) << fmt::format("Total Cost : {}", cost->total);
    return 0;
  }

  return 0;
}
