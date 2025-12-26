# Chrona Architecture

Top-down overview of the repository structure. Updated when subsystems are added or responsibility boundaries change.

## Repository Layout

```
chrona/
├── src/                      # Production source code
│   ├── main.cpp              # Entry point
│   └── errors/               # Error handling subsystem
│       ├── error.hpp         # Error types and declarations
│       └── error.cpp         # Error creation and formatting
├── tests/                    # Test suite (Catch2)
│   └── test_errors.cpp       # Error module tests
├── plans/                    # Planning documents (this directory)
│   ├── README.md             # Plan index
│   ├── ARCHITECTURE.md       # This file
│   └── 000_foundations.md    # Current milestone
├── build/                    # Build artifacts (gitignored)
├── CMakeLists.txt            # Build configuration
└── README.md                 # Project documentation
```

## Subsystems

### Error Handling (`src/errors/`)

Provides consistent error representation and exit codes across the application.

**Key types:**
- `ExitCode` — Process exit codes (Success=0, GeneralError=1, UsageError=2)
- `ErrorCode` — Semantic error categories (NotFound, AlreadyExists, InvalidArgument, IOError, UnknownError)
- `Error` — Combines exit code, error code, and human-readable message

**Key functions:**
- `create_error()` — Constructs error with appropriate exit code
- `print_error()` — Formats error to output stream
- `exit_with_error()` — Prints and terminates process

## Build System

- **CMake 3.20+** with C++20 standard
- **Catch2 v3.5** fetched via FetchContent
- **Targets:**
  - `chrona` — Main executable
  - `chrona_tests` — Test executable
- **Warnings:** `/W4` (MSVC) or `-Wall -Wextra -Wpedantic` (GCC/Clang)

## Dependencies

Current policy: prefer standard library; allow lightweight header-only libraries if genuinely needed.

- **Runtime:** None (stdlib only)
- **Test:** Catch2 (fetched at configure time)

## Current State

Milestone 0 (Foundations) is **in progress**. See [000_foundations.md](000_foundations.md) for detailed checklist.
