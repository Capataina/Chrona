#include "error.hpp"
#include <cstdlib>
#include <iostream>

namespace chrona {

std::optional<Error> create_error(ExitCode exit_code, ErrorCode error_code,
                                  const std::string &message) {
  return Error{exit_code, error_code, message};
}

std::optional<Error> create_error(ErrorCode error_code,
                                  const std::string &message) {
  // Default exit codes based on error code
  ExitCode code = ExitCode::GeneralError;
  if (error_code == ErrorCode::InvalidArgument) {
    code = ExitCode::UsageError;
  }
  return Error{code, error_code, message};
}

void print_error(const Error &error, std::ostream &os) {
  os << "Error: " << error.message << std::endl;
}

void exit_with_error(const Error &error, int exit_code) {
  print_error(error, std::cerr);
  std::exit(exit_code);
}

} // namespace chrona
