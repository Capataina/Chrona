# Chrona

> A Git-inspired, local-first version control core in modern C++ — built to understand how real version control works under the hood: content-addressed storage, immutable snapshots, commit graphs, staging/index semantics, and diffs.

---

## Why Chrona exists

A lot of developers learn “Git” as *the thing you use to push to GitHub*.  
In reality, **Git is the version control system** (the local engine and data model), while **GitHub / GitLab / Codeberg are hosting platforms** that *store Git repositories and add collaboration features* (issues, PRs, CI, permissions, web UI).

Chrona exists to make that separation obvious in practice by rebuilding the core ideas from first principles — so “version control” becomes a system you actually understand, not a set of memorised commands.

---

## What Chrona focuses on

Chrona is a deliberately scoped learning project that targets the “inner engine”:

- A **content-addressed object database** (integrity + deduplication)
- **Snapshot-based versioning** (trees/blobs/commits as immutable objects)
- A **commit DAG** (history as a graph, not a linear list)
- An **index/staging model** (working tree vs staged vs committed)
- **Diff views** as derived information (not the primary storage truth)

The goal is clarity and correctness first, then performance and ergonomics where it makes sense.

---

## Scope

### In scope (v0.x)
Chrona aims to implement a useful **local** VCS core:

- **Object storage primitives**
  - file contents (“blobs” concept)
  - directory snapshots (“trees” concept)
  - history nodes (“commits” concept)
- **History and references**
  - naming pointers to history states (branches/HEAD-like concept)
  - commit graph traversal (history viewing, ancestry)
- **Staging/index model**
  - separating working changes from what will be committed
  - detecting changes across working/staged/committed states
- **Diff mechanics**
  - line-based diffs as a derived view for humans
- **Milestone-driven correctness**
  - deterministic encoding, stable hashing, testable invariants

### Explicitly out of scope (for now)
These are intentionally excluded early to prevent scope creep:

- **Networking / collaboration**
  - remote synchronization, server protocols, hosting integration
- **History rewriting**
  - rebase-style workflows, reflog parity, complex recovery semantics
- **Advanced storage systems**
  - packfiles/delta compression/GC as baseline requirements
- **Enterprise extras**
  - commit signing, hooks, submodules, LFS, sparse checkout

(These can become optional “advanced modules” later if Chrona earns it.)

---

## Design principles

- **Local-first**: everything works offline, instantly.
- **Immutable core**: stored history objects are treated as write-once, read-many.
- **Integrity by construction**: content addressing makes corruption/tampering detectable.
- **Determinism**: identical inputs should produce identical object identities.
- **Derived views**: “diff/status/log” are computed from snapshots and metadata, not stored as primary truth.
- **Small, verifiable steps**: milestones have concrete exit criteria and tests.

---

## Roadmap

Chrona is built in phases. Each milestone should be demo-able and backed by tests, with tight definitions of “done”.

### 0) Foundations
- [ ] Project skeleton and build system
- [ ] Minimal dependency policy (keep it lean)
- [ ] Basic CLI scaffolding (subcommand structure can evolve)
- [ ] Repo discovery (ability to operate from subdirectories)
- [ ] Consistent error model and exit codes
- [ ] Test harness baseline

**Exit criteria**
- A repository can be initialised reliably
- Running the binary with no arguments provides help/usage
- Unit tests run in CI locally

---

### 1) Content-addressed object store (file contents)
- [ ] Hashing + canonical object encoding
- [ ] Write/read “blob-like” objects
- [ ] Validation (type/size checks, corrupted object detection)
- [ ] Simple inspection tooling for debugging internal state

**Exit criteria**
- Storing the same file content twice yields the same object identity
- Stored objects can be read back and verified correctly

---

### 2) Directory snapshots (trees)
- [ ] Tree entry encoding (name + metadata + referenced object identity)
- [ ] Deterministic ordering and stable serialization
- [ ] Snapshot creation from a directory state
- [ ] Materialisation primitive (restore a snapshot into a directory)

**Exit criteria**
- Identical directory states produce identical snapshot identities
- Restoring a snapshot reproduces the directory structure and contents correctly

---

### 3) Commits + history graph
- [ ] Commit objects referencing a directory snapshot + parent commit(s)
- [ ] Metadata (author/time/message) in a stable format
- [ ] Graph traversal (walk history from a “current” pointer)
- [ ] Basic reference system (branch/HEAD-like concept)

**Exit criteria**
- You can create a chain of history states and traverse it reliably
- References correctly point to the intended history state across restarts

---

### 4) Index / staging model
- [ ] A versioned on-disk “index” format
- [ ] Stage selected paths into the index
- [ ] Build commits from the index state (not directly from working directory)
- [ ] Detect differences across working vs staged vs committed

**Exit criteria**
- You can stage a subset of changes and commit only those
- The system can report “what’s changed” in each state boundary accurately

---

### 5) Diff as a derived view
- [ ] Line-based diff for text files (start simple, then improve)
- [ ] Diff between:
  - working vs staged
  - staged vs committed
  - committed vs committed
- [ ] Reasonable handling for binary files (e.g., “binary differs”)

**Exit criteria**
- Diffs are correct on a curated suite of tricky cases (insert/delete/move blocks)
- Output is stable and test-validated

---

### 6) Branching workflow essentials
- [ ] Branch creation/movement semantics
- [ ] Switching between history states safely (checkout-like behaviour)
- [ ] Detached state support (optional, if aligned with learning goals)

**Exit criteria**
- You can maintain multiple lines of work and switch between them without corruption
- History pointers behave predictably under new commits

---

### 7) Merge fundamentals (optional “deep dive”)
- [ ] Merge-base discovery
- [ ] Basic 3-way merge for text
- [ ] Conflict representation strategy (minimal but explicit)

**Exit criteria**
- Simple merges succeed
- Conflicts are surfaced deterministically and recoverably

---

### 8) Storage and performance module (optional “systems flex”)
- [ ] Compaction strategy (packing many objects together)
- [ ] Optional delta compression exploration
- [ ] Reachability and cleanup (GC-like behaviour)

**Exit criteria**
- Large repos remain fast to traverse and store
- Compaction does not change logical history or integrity guarantees

---

### 9) Documentation as a feature
- [ ] Short “How Git works” explanation written from Chrona’s implementation
- [ ] Design notes per milestone: format decisions, invariants, tradeoffs
- [ ] Minimal reproducible demos for each milestone

**Exit criteria**
- A junior engineer can read the docs and accurately explain the Git-vs-host distinction
- A reviewer can validate correctness claims from tests + docs

---

## Summary

Chrona is a disciplined, milestone-driven C++ project that rebuilds the core ideas behind Git (not the hosting platforms around it). The aim is deep systems understanding: immutable snapshots, content addressing, commit graphs, staging semantics, and diff views — with scope control to avoid “endless Git parity”.
