#include "stdafx.h"

#include <chrono>
#include <filesystem>
#include <iostream>

#include <CLI/CLI.hpp>
#include <fmt/core.h>

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    google::SetStderrLogging(google::INFO);
    google::SetLogDestination(google::INFO, "main.log.");

    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    CLI::App app { "main module" };
    app.require_subcommand(0, 1);

    auto sub_solve = app.add_subcommand("solve");
    std::string solver_name;
    sub_solve->add_option("solver_name", solver_name, "solver name");

    CLI11_PARSE(app, argc, argv);

    return 0;
}
