#include <optional>
#include <string>
#include <vector>

namespace chrona {

enum class Command {
  Init,
};

enum class ParseAction { RunCommand, ShowHelp, Error };

struct ParseResult {
  ParseAction action;
  std::optional<Command> command;
  std::vector<std::string> args;
  std::optional<std::string> error_message;
};

ParseResult parse_args(int argc, char *argv[]);

void print_usage();

} // namespace chrona