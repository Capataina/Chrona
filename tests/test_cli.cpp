#include "cli/cli.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

namespace chrona {

TEST_CASE("parse_args - no arguments", "[cli]") {
  const char *argv[] = {"chrona"};
  auto result = parse_args(1, const_cast<char **>(argv));

  REQUIRE(result.action == ParseAction::ShowHelp);
  REQUIRE_FALSE(result.command.has_value());
  REQUIRE(result.args.empty());
  REQUIRE(result.error_message.has_value());
  REQUIRE(result.error_message.value() == "No arguments provided");
}

TEST_CASE("parse_args - --help", "[cli]") {
  const char *argv[] = {"chrona", "--help"};
  auto result = parse_args(2, const_cast<char **>(argv));

  REQUIRE(result.action == ParseAction::ShowHelp);
  REQUIRE_FALSE(result.command.has_value());
  REQUIRE(result.args.empty());
  REQUIRE_FALSE(result.error_message.has_value());
}

TEST_CASE("parse_args - init command", "[cli]") {
  const char *argv[] = {"chrona", "init"};
  auto result = parse_args(2, const_cast<char **>(argv));

  REQUIRE(result.action == ParseAction::RunCommand);
  REQUIRE(result.command.has_value());
  REQUIRE(result.command.value() == Command::Init);
  REQUIRE(result.args.empty());
  REQUIRE_FALSE(result.error_message.has_value());
}

TEST_CASE("parse_args - init with extra arguments", "[cli]") {
  const char *argv[] = {"chrona", "init", "extra", "args"};
  auto result = parse_args(4, const_cast<char **>(argv));

  // Based on current implementation, this should return Error
  // (This might be a bug - you may want to handle extra args for init
  // differently)
  REQUIRE(result.action == ParseAction::Error);
  REQUIRE(result.error_message.has_value());
  REQUIRE(result.error_message.value() == "Too many arguments provided");
}

TEST_CASE("parse_args - unknown command", "[cli]") {
  const char *argv[] = {"chrona", "unknown"};
  auto result = parse_args(2, const_cast<char **>(argv));

  REQUIRE(result.action == ParseAction::Error);
  REQUIRE_FALSE(result.command.has_value());
  REQUIRE(result.args.empty());
  REQUIRE(result.error_message.has_value());
  REQUIRE(result.error_message.value() == "Unknown command: unknown");
}

TEST_CASE("parse_args - unknown command with extra args", "[cli]") {
  const char *argv[] = {"chrona", "unknown", "arg1", "arg2"};
  auto result = parse_args(4, const_cast<char **>(argv));

  REQUIRE(result.action == ParseAction::Error);
  REQUIRE(result.error_message.has_value());
  REQUIRE(result.error_message.value() == "Too many arguments provided");
}

TEST_CASE("parse_args - empty string as command", "[cli]") {
  const char *argv[] = {"chrona", ""};
  auto result = parse_args(2, const_cast<char **>(argv));

  REQUIRE(result.action == ParseAction::Error);
  REQUIRE(result.error_message.has_value());
  REQUIRE(result.error_message.value() == "Unknown command: ");
}

TEST_CASE("parse_args - case sensitivity", "[cli]") {
  SECTION("uppercase INIT should fail") {
    const char *argv[] = {"chrona", "INIT"};
    auto result = parse_args(2, const_cast<char **>(argv));

    REQUIRE(result.action == ParseAction::Error);
    REQUIRE(result.error_message.has_value());
    REQUIRE(result.error_message.value() == "Unknown command: INIT");
  }

  SECTION("mixed case Init should fail") {
    const char *argv[] = {"chrona", "Init"};
    auto result = parse_args(2, const_cast<char **>(argv));

    REQUIRE(result.action == ParseAction::Error);
    REQUIRE(result.error_message.has_value());
    REQUIRE(result.error_message.value() == "Unknown command: Init");
  }
}

TEST_CASE("parse_args - help variants", "[cli]") {
  SECTION("-h should fail (not implemented)") {
    const char *argv[] = {"chrona", "-h"};
    auto result = parse_args(2, const_cast<char **>(argv));

    REQUIRE(result.action == ParseAction::Error);
    REQUIRE(result.error_message.has_value());
    REQUIRE(result.error_message.value() == "Unknown command: -h");
  }

  SECTION("help (without dashes) should fail") {
    const char *argv[] = {"chrona", "help"};
    auto result = parse_args(2, const_cast<char **>(argv));

    REQUIRE(result.action == ParseAction::Error);
    REQUIRE(result.error_message.has_value());
    REQUIRE(result.error_message.value() == "Unknown command: help");
  }
}

TEST_CASE("ParseResult structure", "[cli]") {
  // Verify ParseResult can be constructed with all fields
  ParseResult help_result{
      ParseAction::ShowHelp, std::nullopt, {}, std::nullopt};

  REQUIRE(help_result.action == ParseAction::ShowHelp);
  REQUIRE_FALSE(help_result.command.has_value());
  REQUIRE(help_result.args.empty());
  REQUIRE_FALSE(help_result.error_message.has_value());

  ParseResult command_result{
      ParseAction::RunCommand, Command::Init, {}, std::nullopt};

  REQUIRE(command_result.action == ParseAction::RunCommand);
  REQUIRE(command_result.command.has_value());
  REQUIRE(command_result.command.value() == Command::Init);

  ParseResult error_result{
      ParseAction::Error, std::nullopt, {}, "Test error message"};

  REQUIRE(error_result.action == ParseAction::Error);
  REQUIRE(error_result.error_message.has_value());
  REQUIRE(error_result.error_message.value() == "Test error message");
}

} // namespace chrona