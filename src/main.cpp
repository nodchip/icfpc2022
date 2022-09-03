#include "stdafx.h"

#include <chrono>
#include <filesystem>
#include <iostream>

#include <CLI/CLI.hpp>
#include <fmt/core.h>

#include "painter.h"
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

  auto sub_solve = app.add_subcommand("solve");
  std::string solver_names;
  std::string problem_file;
  std::string initial_solution_isl;
  std::string output_solution_isl = "output.isl";
  double timeout_s = -1.0;
  bool visualize = false;
  sub_solve->add_option("solver_name", solver_names, "solver name or comma-separated list of solver names");
  sub_solve->add_option("problem_file", problem_file, "problem file path");
  sub_solve->add_option("output_solution_isl", output_solution_isl, "output solution ISL file path (optional. default=output.isl)");
  sub_solve->add_option("initial_solution_isl", initial_solution_isl, "input solution ISL file path (optional)");
  sub_solve->add_option("--timeout", timeout_s, "timeout (s). it is up to each solver to follow the timeout or not");
  sub_solve->add_flag("--visualize", visualize, "realtime visualize");

  SolverRegistry::setOptionParser(sub_solve);

  CLI11_PARSE(app, argc, argv);

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

    std::shared_ptr<Painting> problem = loadPaintingFromFile(problem_file);
    if (!problem) {
      LOG(ERROR) << fmt::format("failed to load problem {}", problem_file);
    }
    LOG(INFO) << fmt::format("Problem  : {} ({}x{})", problem_file, problem->width, problem->height);

    std::vector<std::shared_ptr<Instruction>> initial_solution;
    if (std::filesystem::exists(initial_solution_isl)) {
      // TODO: load ISL
      LOG(INFO) << fmt::format("Initial Solution  : {}", initial_solution_isl);
    }

    SolverArguments arg(problem);
    arg.optional_initial_solution = initial_solution;
    arg.visualize = visualize;
    if (timeout_s > 0) {
      arg.timeout_s = timeout_s;
    }

    SolverOutputs out;
    for (auto solver_name : solver_name_list) {
      auto solver = SolverRegistry::getSolver(solver_name);
      if (!solver) {
        LOG(ERROR) << fmt::format("solver [{0}] not found!", solver_name);
        return -1;
      }
      LOG(ERROR) << fmt::format("Solver   : {} (starting with {} instructions)", solver_name, arg.optional_initial_solution.size());

      const auto t0 = std::chrono::system_clock::now();
      out = solver->solve(arg);
      const auto t1 = std::chrono::system_clock::now();
      const double solve_s = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
      LOG(INFO) << fmt::format("Elapsed  : {:.2f} s", solve_s);

      // successive processing.
      arg.optional_initial_solution = out.solution;
    }

    // TODO: evaluate the solution.

    dumpInstructions(output_solution_isl, out.solution);
    LOG(INFO) << fmt::format("Dumped {} instructions to : {}", out.solution.size(), output_solution_isl);

    return 0;
  }

  return 0;
}
