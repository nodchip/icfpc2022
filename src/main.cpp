#include "stdafx.h"

#include <chrono>
#include <filesystem>
#include <iostream>

#include <CLI/CLI.hpp>
#include <fmt/core.h>

#include "solver_registry.h"

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
  std::string solver_name;
  std::string problem_file;
  std::string initial_solution_isl;
  std::string output_solution_isl = "output.isl";
  double timeout_s = -1.0;
  bool visualize = false;
  sub_solve->add_option("solver_name", solver_name, "solver name");
  sub_solve->add_option("problem_file", problem_file, "problem file path");
  sub_solve->add_option("output_solution_isl", output_solution_isl, "output solution ISL file path (optional. default=output.isl)");
  sub_solve->add_option("initial_solution_isl", initial_solution_isl, "input solution ISL file path (optional)");
  sub_solve->add_option("--timeout", timeout_s, "timeout (s). it is up to each solver to follow the timeout or not");
  sub_solve->add_flag("--visualize", visualize, "realtime visualize");

  CLI11_PARSE(app, argc, argv);

  if (sub_solve->parsed()) {
    solver_name = SolverRegistry::getCanonicalSolverName(solver_name);
    LOG(ERROR) << fmt::format("Solver   : {}", solver_name);

    auto solver = SolverRegistry::getSolver(solver_name);
    if (!solver) {
      LOG(ERROR) << fmt::format("solver [{0}] not found!", solver_name);
      return 0;
    }

    PaintingPtr problem = nullptr; // TODO: load Painting
    LOG(INFO) << fmt::format("Problem  : {}", problem_file);

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

    const auto t0 = std::chrono::system_clock::now();
    SolverOutputs out = solver->solve(arg);
    const auto t1 = std::chrono::system_clock::now();
    const double solve_s = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() * 1e-6;
    LOG(INFO) << fmt::format("Elapsed  : {:.2f} s", solve_s);

    // TODO: evaluate the solution.

    dumpInstructions(output_solution_isl, out.solution);
    LOG(INFO) << fmt::format("Dumped {} instructions to : {}", out.solution.size(), output_solution_isl);

    return 0;
  }

  return 0;
}
