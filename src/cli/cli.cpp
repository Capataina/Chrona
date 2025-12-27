#include "cli.hpp"
#include <iostream>
#include <optional>

namespace chrona {

ParseResult parse_args(int argc, char *argv[]) {

  if (argc > 2) {
    return ParseResult{
        ParseAction::Error, std::nullopt, {}, "Too many arguments provided"};
  }

  if (argc == 1) {
    return ParseResult{
        ParseAction::ShowHelp, std::nullopt, {}, "No arguments provided"};
  }

  if (std::string(argv[1]) == "--help") {
    return ParseResult{ParseAction::ShowHelp, std::nullopt, {}, std::nullopt};
  }

  if (std::string(argv[1]) == "init") {
    return ParseResult{
        ParseAction::RunCommand, Command::Init, {}, std::nullopt};
  }

  return ParseResult{ParseAction::Error,
                     std::nullopt,
                     {},
                     "Unknown command: " + std::string(argv[1])};
}

void print_usage() {
  std::cout
      << "Usage: chrona <command> [args...]" << std::endl
      << std::endl
      << "Commands:" << std::endl
      << "  init    Initialize a new repository" << std::endl
      << "  help    Show help" << std::endl
      << std::endl
      << "For more information, see the documentation at https://chrona.com"
      << std::endl;
}

} // namespace chrona