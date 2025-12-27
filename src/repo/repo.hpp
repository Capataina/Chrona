#include <filesystem>

namespace chrona {

std::optional<std::filesystem::path>
find_repo(const std::filesystem::path &start_path);

} // namespace chrona