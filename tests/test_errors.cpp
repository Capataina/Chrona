#include "errors/error.hpp"
#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("create_error", "[error]") {
  SECTION("create_error with valid arguments") {
    auto error =
        chrona::create_error(chrona::ExitCode::GeneralError,
                             chrona::ErrorCode::NotFound, "Test error");
    REQUIRE(error.has_value());
    REQUIRE(error->exit_code == chrona::ExitCode::GeneralError);
    REQUIRE(error->error_code == chrona::ErrorCode::NotFound);
    REQUIRE(error->message == "Test error");
  }

  SECTION("create_error with default exit code") {
    auto error = chrona::create_error(chrona::ErrorCode::InvalidArgument,
                                      "Bad argument");
    REQUIRE(error.has_value());
    REQUIRE(error->exit_code == chrona::ExitCode::UsageError);
  }
}

TEST_CASE("print_error", "[error]") {
  auto error = chrona::create_error(chrona::ExitCode::GeneralError,
                                    chrona::ErrorCode::NotFound, "Test message")
                   .value();

  std::ostringstream oss;
  chrona::print_error(error, oss);
  REQUIRE(oss.str() == "Error: Test message\n");
}