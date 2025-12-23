cmake_minimum_required(VERSION 3.20)

project(chrona
  VERSION 0.1.0
  LANGUAGES CXX
)

add_executable(chrona
  src/main.cpp
)

target_compile_features(chrona PRIVATE cxx_std_20)

# Good defaults: warnings (kept minimal and cross-compiler friendly)
if (MSVC)
  target_compile_options(chrona PRIVATE /W4)
else()
  target_compile_options(chrona PRIVATE -Wall -Wextra -Wpedantic)
endif()
