cmake_minimum_required(VERSION 3.20)

project(chrona
  VERSION 0.1.0
  LANGUAGES CXX
)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.0
)
FetchContent_MakeAvailable(Catch2)

# Main executable
add_executable(chrona
  src/main.cpp
  src/errors/error.cpp
)

target_compile_features(chrona PRIVATE cxx_std_20)

# Test executable - compile source files directly, don't link the main executable
add_executable(chrona_tests
  tests/test_errors.cpp
  src/errors/error.cpp  # Include source files you want to test
)

target_compile_features(chrona_tests PRIVATE cxx_std_20)
target_include_directories(chrona_tests PRIVATE src)
target_link_libraries(chrona_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
enable_testing()
add_test(NAME chrona_tests COMMAND chrona_tests)

# Good defaults: warnings (kept minimal and cross-compiler friendly)
if (MSVC)
  target_compile_options(chrona PRIVATE /W4)
  target_compile_options(chrona_tests PRIVATE /W4)
else()
  target_compile_options(chrona PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_options(chrona_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()