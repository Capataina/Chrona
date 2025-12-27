#include "cli/cli.hpp"
#include "errors/error.hpp"

int main(int argc, char *argv[]) {
  // First we need to parse the arguments
  auto result = chrona::parse_args(argc, argv);

  // Then we need to handle the result
  switch (result.action) {
    // Show help
  case chrona::ParseAction::ShowHelp:
    chrona::print_usage();
    return 0;

    // Error
  case chrona::ParseAction::Error:
    chrona::print_error(chrona::create_error(chrona::ErrorCode::InvalidArgument,
                                             result.error_message.value())
                            .value());
    return 1;

    // Run command
  case chrona::ParseAction::RunCommand:
    switch (result.command.value()) {
    case chrona::Command::Init:
      // TODO: Implement init command
      return 0;
    default:
      chrona::print_error(
          chrona::create_error(
              chrona::ErrorCode::InvalidArgument,
              "Unknown command: " +
                  std::to_string(static_cast<int>(result.command.value())))
              .value());
      return 1;
    }
  }
}
