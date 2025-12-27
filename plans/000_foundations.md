# 000_foundations.md

## Goal and scope

Establish the foundational infrastructure for Chrona: build system, CLI structure, error handling, repo discovery, and test harness. This milestone enables all subsequent development by providing a reliable, testable base.

**Deliverables:**

- Working CMake build system (✓ done)
- Minimal dependency policy documented
- CLI with subcommand structure and help output
- Repository discovery mechanism
- Consistent error model with exit codes
- Test harness integrated into build

**Out of scope:**

- Actual VCS functionality (that's Milestone 1+)
- Advanced CLI features (colours, progress bars, etc.)
- Cross-platform edge cases beyond basic Windows/Linux support

---

## Context and justification

Before implementing version control primitives, we need:

1. A way to invoke the tool (`chrona <command>`)
2. A way to detect if we're in a repository
3. A way to handle errors consistently
4. A way to verify correctness (tests)

This milestone is prerequisite for Milestone 1 (object store), which will need repo initialisation and CLI commands.

**Rejected alternatives:**

- Using a heavy CLI library (e.g., CLI11, cxxopts) — prefer minimal/stdlib-first approach
- Skipping tests until later — too risky for correctness-critical code

**Assumptions:**

- C++20 standard library is sufficient for argument parsing (or one lightweight header-only library if needed)
- Tests can use a simple framework (Catch2, Google Test, or manual test runner)

---

## Interfaces and contracts

### CLI interface

- `chrona` (no args) → prints usage/help, exits 0
- `chrona --help` → prints usage/help, exits 0
- `chrona <command> [args...]` → executes command
- `chrona init` → initialises a repository in current directory
- Unknown command → error message, exits 1

### Repository structure

- `.chrona/` directory at repo root
- `.chrona/objects/` (for future object store)
- `.chrona/refs/` (for future references)
- `.chrona/config` (optional, for future config)

### Error model

- Exit codes: 0 = success, 1 = general error, 2 = usage error
- Error messages go to stderr
- Consistent error formatting (optional: error codes)

### Repo discovery

- Walk up from current directory looking for `.chrona/`
- Stop at filesystem root
- Return path to repo root or "not found"

**Must not break:**

- Build system continues to work
- No breaking changes to existing `main.cpp` structure (can refactor incrementally)

---

## Impacted areas

**Files to create:**

- `plans/000_foundations.md` (this file)
- `plans/README.md` (index)
- `src/cli.h` / `src/cli.cpp` (CLI parsing)
- `src/repo.h` / `src/repo.cpp` (repo discovery + init)
- `src/error.h` / `src/error.cpp` (error types/codes)
- `tests/` directory
- `tests/test_main.cpp` (test harness entry point)
- `CMakeLists.txt` (add test target)

**Files to modify:**

- `src/main.cpp` (route to CLI handler)
- `CMakeLists.txt` (add test configuration)
- `.gitignore` (add `build/`, test artifacts)

**Dependencies:**

- None external (or one lightweight header-only library if argument parsing becomes unwieldy)

---

## Incremental implementation

### Step 1: Update .gitignore

- [x] Add `build/` directory
- [x] Add common C++ build artifacts (`*.obj`, `*.o`, `*.exe`, etc.)
- **Verification:** `.gitignore` excludes build outputs

### Step 2: Document dependency policy

- [ ] Add section to README or create `DEPENDENCIES.md`
- [ ] State: prefer stdlib, allow one lightweight header-only library if needed
- [ ] Decision: which library (if any) for argument parsing, or manual `argc/argv`?
- **Verification:** Policy is clear and committed

### Step 3: Basic error model

- [x] Create `src/error.h` with error types/codes enum
- [x] Create `src/error.cpp` with error message formatting
- [x] Define exit code constants (0, 1, 2)
- **Verification:** Can create and format errors, exit codes are consistent

### Step 4: Repo discovery (no init yet)

- [x] Create `src/repo.h` with `find_repo(std::filesystem::path)` function
- [x] Create `src/repo.cpp` implementing directory walk-up logic
- [x] Return optional path or "not found" state
- **Verification:** Unit test: finds repo when `.chrona/` exists in parent, returns not-found otherwise

### Step 5: CLI scaffolding (argument parsing)

- [x] Create `src/cli.h` with command enum and argument structure
- [x] Create `src/cli.cpp` with `parse_args(int argc, char* argv[])` function
- [x] Handle: no args, `--help`, `<command>` cases
- [x] Return parsed command or error
- **Verification:** Unit test: parses known commands, rejects unknown, handles `--help`

### Step 6: Help/usage output

- [x] Add `print_usage()` function to `src/cli.cpp`
- [x] Show available commands and basic usage
- [x] Integrate into main: no args → help, `--help` → help
- **Verification:** Running `chrona` or `chrona --help` shows usage, exits 0

### Step 7: Repository initialisation

- [ ] Add `init_repo(std::filesystem::path)` to `src/repo.cpp`
- [ ] Create `.chrona/` directory structure
- [ ] Add `chrona init` command handler
- [ ] Error if repo already exists
- **Verification:** `chrona init` creates `.chrona/` structure, fails if already exists, `find_repo()` finds it after init

### Step 8: Test harness setup

- [ ] Choose test framework (Catch2, Google Test, or simple manual runner)
- [ ] Add test target to `CMakeLists.txt`
- [ ] Create `tests/test_main.cpp` with one example test
- [ ] Create `tests/test_repo.cpp` with repo discovery tests
- [ ] Create `tests/test_cli.cpp` with CLI parsing tests
- **Verification:** `cmake --build build --target test` runs tests successfully

### Step 9: Integration

- [ ] Wire CLI into `main.cpp`: parse args, route to command handlers
- [ ] Wire error handling: catch errors, print to stderr, exit with correct code
- [ ] Wire repo discovery into commands that need it
- **Verification:** End-to-end: `chrona init` works, `chrona` shows help, unknown command errors correctly

---

## Testing and validation

**Unit tests:**

- Repo discovery: finds `.chrona/` in current dir, parent dir, fails at root
- CLI parsing: all command forms, `--help`, unknown commands
- Error formatting: messages go to stderr, exit codes correct

**Integration tests:**

- `chrona` (no args) → help output, exit 0
- `chrona --help` → help output, exit 0
- `chrona init` → creates repo, exit 0
- `chrona init` (in existing repo) → error, exit 1
- `chrona unknown` → error message, exit 1

**Commands:**

# Build

cmake -B build
cmake --build build

# Run tests

cmake --build build --target test

# or

cd build && ctest

# Manual verification

./build/Debug/chrona
./build/Debug/chrona --help
./build/Debug/chrona init
./build/Debug/chrona unknown---

## Risks and failure modes

**Edge cases:**

- Repo discovery at filesystem root (should stop, not infinite loop)
- Permissions: can't create `.chrona/` (should error clearly)
- Unicode in paths (C++20 `std::filesystem` should handle, but test)

**Detection:**

- Tests fail on edge cases
- Manual testing on Windows and Linux if possible

**Mitigations:**

- Use `std::filesystem` for path handling (C++17/20 standard)
- Check return values from filesystem operations
- Clear error messages for permission issues

**Concurrency:**

- Not a concern yet (single-threaded CLI)

---

## Exit criteria

- [x] Project skeleton and build system working
- [ ] Minimal dependency policy documented
- [ ] `chrona` (no args) prints help/usage, exits 0
- [ ] `chrona --help` prints help/usage, exits 0
- [ ] `chrona init` creates `.chrona/` structure reliably
- [ ] `chrona unknown` shows error, exits 1
- [ ] Unit tests run via CMake test target
- [ ] All tests pass
- [ ] `.gitignore` excludes build artifacts

**Measurable gates:**

- Help output is readable and lists available commands
- Repo init creates expected directory structure (verifiable by `find_repo()`)
- Test suite has at least 5 tests covering core functionality
- Zero compiler warnings (with current warning flags)

---

## Future considerations

**Follow-ons:**

- Milestone 1 will add object store commands (`chrona hash-object`, etc.)
- CLI will need to grow subcommands incrementally
- Error model may need richer types later (but keep it simple for now)

**Known limits:**

- Manual argument parsing may become unwieldy; consider lightweight library later
- Test framework choice may need revisiting if we need advanced features

**Tech debt:**

- None anticipated if we keep it simple and incremental
