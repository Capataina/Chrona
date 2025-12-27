#include <iostream>
#include <optional>

namespace chrona {

enum class ExitCode {
  Success = 0,
  GeneralError = 1,
  UsageError = 2,
};

enum class ErrorCode {
  NotFound,
  AlreadyExists,
  InvalidArgument,
  IOError,
  UnknownError,
};

struct Error {
  ExitCode exit_code;
  ErrorCode error_code;
  std::string message;
};

std::optional<Error> create_error(ExitCode exit_code, ErrorCode error_code,
                                  const std::string &message);
std::optional<Error> create_error(ErrorCode error_code,
                                  const std::string &message);
void print_error(const Error &error, std::ostream &os = std::cerr);
void exit_with_error(const Error &error,
                     int exit_code = static_cast<int>(ExitCode::GeneralError));

} // namespace chrona