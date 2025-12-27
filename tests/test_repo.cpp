#include "repo/repo.hpp"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>

TEST_CASE("find_repo - comprehensive debugging", "[repo]") {
  std::cout << "\n=== TEST SUITE START ===\n" << std::endl;

  SECTION("Test 1: Current working directory") {
    std::cout << "\n--- Test 1: Current working directory ---" << std::endl;
    auto cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;

    auto repo = chrona::find_repo(cwd);

    std::cout << "Result has_value(): " << repo.has_value() << std::endl;
    if (repo.has_value()) {
      std::cout << "Found repo at: " << repo.value() << std::endl;
      std::cout << "Repo absolute path: "
                << std::filesystem::absolute(repo.value()) << std::endl;
    } else {
      std::cout << "No repo found (optional is empty)" << std::endl;
    }

    // Check if .chrona actually exists in current dir
    auto chrona_path = cwd / ".chrona";
    std::cout << "Checking if .chrona exists in current dir: " << chrona_path
              << std::endl;
    std::cout << ".chrona exists: " << std::filesystem::exists(chrona_path)
              << std::endl;

    REQUIRE(true); // Just for debugging, no assertion yet
  }

  SECTION("Test 2: Current directory as string '.'") {
    std::cout << "\n--- Test 2: Current directory as '.' ---" << std::endl;
    std::filesystem::path start = ".";
    std::cout << "Start path: " << start << std::endl;
    std::cout << "Start path absolute: " << std::filesystem::absolute(start)
              << std::endl;

    auto repo = chrona::find_repo(start);

    std::cout << "Result has_value(): " << repo.has_value() << std::endl;
    if (repo.has_value()) {
      std::cout << "Found repo at: " << repo.value() << std::endl;
      std::cout << "Repo absolute: " << std::filesystem::absolute(repo.value())
                << std::endl;
    } else {
      std::cout << "No repo found" << std::endl;
    }

    REQUIRE(true);
  }

  SECTION("Test 3: Parent directory") {
    std::cout << "\n--- Test 3: Parent directory ---" << std::endl;
    auto cwd = std::filesystem::current_path();
    auto parent = cwd.parent_path();
    std::cout << "Current dir: " << cwd << std::endl;
    std::cout << "Parent dir: " << parent << std::endl;

    auto repo = chrona::find_repo(parent);

    std::cout << "Result has_value(): " << repo.has_value() << std::endl;
    if (repo.has_value()) {
      std::cout << "Found repo at: " << repo.value() << std::endl;
    } else {
      std::cout << "No repo found in parent" << std::endl;
    }

    // Check parent for .chrona
    auto chrona_in_parent = parent / ".chrona";
    std::cout << "Checking .chrona in parent: " << chrona_in_parent
              << std::endl;
    std::cout << "Exists: " << std::filesystem::exists(chrona_in_parent)
              << std::endl;

    REQUIRE(true);
  }

  SECTION("Test 4: Parent directory as string '..'") {
    std::cout << "\n--- Test 4: Parent directory as '..' ---" << std::endl;
    std::filesystem::path start = "..";
    std::cout << "Start path: " << start << std::endl;
    std::cout << "Start path absolute: " << std::filesystem::absolute(start)
              << std::endl;

    auto repo = chrona::find_repo(start);

    std::cout << "Result has_value(): " << repo.has_value() << std::endl;
    if (repo.has_value()) {
      std::cout << "Found repo at: " << repo.value() << std::endl;
      std::cout << "Repo absolute: " << std::filesystem::absolute(repo.value())
                << std::endl;
    } else {
      std::cout << "No repo found" << std::endl;
    }

    REQUIRE(true);
  }

  SECTION("Test 5: Root directory (Unix style)") {
    std::cout << "\n--- Test 5: Root directory '/' ---" << std::endl;
    std::filesystem::path start = "/";
    std::cout << "Start path: " << start << std::endl;

    // Check if this path exists on Windows
    std::cout << "Path exists: " << std::filesystem::exists(start) << std::endl;

    auto repo = chrona::find_repo(start);

    std::cout << "Result has_value(): " << repo.has_value() << std::endl;
    if (repo.has_value()) {
      std::cout << "ERROR: Found repo at root (unexpected): " << repo.value()
                << std::endl;
    } else {
      std::cout << "OK: No repo found at root (expected)" << std::endl;
    }

    REQUIRE(!repo.has_value());
  }

  SECTION("Test 6: Windows root (C:\\)") {
    std::cout << "\n--- Test 6: Windows root C:\\ ---" << std::endl;
    std::filesystem::path start = "C:\\";
    std::cout << "Start path: " << start << std::endl;
    std::cout << "Path exists: " << std::filesystem::exists(start) << std::endl;

    auto current = start;
    std::cout << "Testing parent_path() behavior:" << std::endl;
    for (int i = 0; i < 5; ++i) {
      auto parent = current.parent_path();
      std::cout << "  Iteration " << i << ": current=" << current
                << ", parent=" << parent
                << ", parent==current=" << (parent == current)
                << ", parent.empty()=" << parent.empty() << std::endl;
      if (parent == current || parent.empty()) {
        std::cout << "  -> Would break here" << std::endl;
        break;
      }
      current = parent;
    }

    auto repo = chrona::find_repo(start);

    std::cout << "Result has_value(): " << repo.has_value() << std::endl;
    if (repo.has_value()) {
      std::cout << "Found repo at: " << repo.value() << std::endl;
    } else {
      std::cout << "No repo found (expected)" << std::endl;
    }

    REQUIRE(!repo.has_value());
  }

  SECTION("Test 7: Simulate find_repo traversal manually") {
    std::cout << "\n--- Test 7: Manual traversal simulation ---" << std::endl;
    auto start = std::filesystem::current_path();
    std::cout << "Starting from: " << start << std::endl;
    std::cout << "Absolute path: " << std::filesystem::absolute(start)
              << std::endl;

    auto current = start;
    int iteration = 0;
    std::cout << "\nTraversing up the directory tree:" << std::endl;

    while (true) {
      std::cout << "\n  Iteration " << iteration << ":" << std::endl;
      std::cout << "    Current: " << current << std::endl;
      std::cout << "    Current absolute: "
                << std::filesystem::absolute(current) << std::endl;

      auto check_path = current / ".chrona";
      std::cout << "    Checking: " << check_path << std::endl;
      bool exists = std::filesystem::exists(check_path);
      std::cout << "    .chrona exists: " << exists << std::endl;

      if (exists) {
        std::cout << "    -> FOUND! Would return: " << current << std::endl;
        break;
      }

      auto parent = current.parent_path();
      std::cout << "    Parent: " << parent << std::endl;
      std::cout << "    Parent absolute: " << std::filesystem::absolute(parent)
                << std::endl;
      std::cout << "    parent == current: " << (parent == current)
                << std::endl;
      std::cout << "    parent.empty(): " << parent.empty() << std::endl;

      if (parent == current || parent.empty()) {
        std::cout << "    -> Would break (reached root)" << std::endl;
        break;
      }

      current = parent;
      iteration++;

      if (iteration > 20) {
        std::cout << "    -> STOPPING: Too many iterations (infinite loop?)"
                  << std::endl;
        break;
      }
    }

    REQUIRE(true);
  }

  SECTION("Test 8: Build directory") {
    std::cout << "\n--- Test 8: Build directory ---" << std::endl;
    std::filesystem::path build_dir = "build";
    std::cout << "Checking build directory: " << build_dir << std::endl;

    if (std::filesystem::exists(build_dir)) {
      std::cout << "Build dir exists" << std::endl;
      auto build_abs = std::filesystem::absolute(build_dir);
      std::cout << "Build dir absolute: " << build_abs << std::endl;

      auto repo = chrona::find_repo(build_dir);

      std::cout << "Result has_value(): " << repo.has_value() << std::endl;
      if (repo.has_value()) {
        std::cout << "Found repo: " << repo.value() << std::endl;
      } else {
        std::cout << "No repo found" << std::endl;
      }
    } else {
      std::cout << "Build dir does not exist" << std::endl;
    }

    REQUIRE(true);
  }

  SECTION("Test 9: Check if .chrona exists in repo root") {
    std::cout << "\n--- Test 9: Looking for .chrona in repo ---" << std::endl;
    auto cwd = std::filesystem::current_path();
    std::cout << "Current directory: " << cwd << std::endl;

    // Walk up and check each directory
    auto current = cwd;
    int depth = 0;
    while (depth < 10) {
      auto chrona_check = current / ".chrona";
      std::cout << "Depth " << depth << ": Checking " << chrona_check
                << std::endl;
      std::cout << "  Exists: " << std::filesystem::exists(chrona_check)
                << std::endl;

      if (std::filesystem::exists(chrona_check)) {
        std::cout << "  -> FOUND .chrona at: " << current << std::endl;
        if (std::filesystem::is_directory(chrona_check)) {
          std::cout << "  -> It's a directory" << std::endl;
        } else if (std::filesystem::is_regular_file(chrona_check)) {
          std::cout << "  -> It's a file" << std::endl;
        }
        break;
      }

      auto parent = current.parent_path();
      if (parent == current || parent.empty()) {
        std::cout << "  -> Reached root, stopping" << std::endl;
        break;
      }
      current = parent;
      depth++;
    }

    REQUIRE(true);
  }

  std::cout << "\n=== TEST SUITE END ===\n" << std::endl;
}