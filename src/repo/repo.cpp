#include "repo.hpp"
#include <filesystem>
#include <optional>

namespace chrona {

std::optional<std::filesystem::path>
find_repo(const std::filesystem::path &start_path) {
  auto current = start_path;

  while (true) {
    if (std::filesystem::exists(current / ".chrona")) {
      return current;
    }

    auto parent = current.parent_path();
    if (parent == current || parent.empty()) {
      break;
    }
    current = parent;
  }

  return {};
}

} // namespace chrona