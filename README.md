# C++ Agentic Development Frame
[![Actions Status](https://github.com/n-n-code/cpp-agentic-development-frame/workflows/CI/badge.svg)](https://github.com/n-n-code/cpp-agentic-development-frame/actions)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

<img width="2048" height="1024" alt="tiredrobotmaster" src="https://github.com/user-attachments/assets/477996ef-bdb9-4090-a7f6-7e310e200eb4" />

A generic CMake-first C++ project frame for agentic development.

It packages a buildable example library and CLI together with repo-local agent
skills, deterministic tests, docs, analyzers, release scripts, and GitHub
workflows that are meant to be copied and adapted into a real project.

## Overview

This frame is designed for projects that want:

- a clear `AGENTS.md` contract for coding agents
- project-local skills under `.agents/skills/`
- CMake-only build and install flow
- deterministic `CTest` coverage with enforced WHAT/HOW/WHY commentary
- `clang-tidy`, Doxygen, and Valgrind lanes, plus an example Qt/Clazy path for UI projects
- release hygiene and release checklist automation
- generic CI and release GitHub workflows

Defaults:

- portable C++23 library + CLI example
- no Qt requirement by default
- optional Qt/Clazy support as the example stack for projects that add a UI layer
- MIT license

## Quick Start

Using CMake presets (recommended):

```bash
cmake --preset dev
cmake --build build/dev -j"$(nproc)"
ctest --test-dir build/dev --output-on-failure
./build/dev/frame_cli --help
```

Or manually:

```bash
BUILD_DIR="$(mktemp -d /tmp/cpp-frame-build-XXXXXX)"
cmake -S . -B "$BUILD_DIR" -G Ninja
cmake --build "$BUILD_DIR" -j"$(nproc)"
ctest --test-dir "$BUILD_DIR" --output-on-failure
"$BUILD_DIR/frame_cli" --help
```

Available presets: `dev` (debug + sanitizers), `release` (optimized + LTO),
`ci` (matches GitHub Actions), `coverage` (gcov instrumentation).

## What This Frame Includes

- `AGENTS.md`: canonical repo contract for build/test/tooling/release rules
- `config/change-contract-policy.sh`: repo-local change-contract policy shared by the checker and overlay skill
- `.agents/skills/`: generic project-local skills for coding, documenting,
  planning, security, release work, vendor-boundary work, diagnostics, a
  portable development-contract core skill, and the repo-local overlay
- `src/`: small example library + CLI
- `tests/`: deterministic example tests, `CTest` registration, and
  `frame_test.h` micro-framework
- `docs/`: API-focused Doxygen setup
- `scripts/`: hygiene, commentary, release, and Valgrind helpers
- `.github/workflows/`: CI and release workflow templates
- `contrib/`: optional system integration examples
- `benchmarks/`: optional chrono-based micro-benchmark harness
- `CMakePresets.json`: named build configurations for dev/release/ci/coverage
- `feature_records/`: tracked feature-record folder with lifecycle subdirectories
  with explicit lifecycle state, change contracts, uncertainty/cost bands,
  responsibilities, and evidence matrices

## Feature Record Lifecycle

Human-readable state flow:

```text
                 start work                 complete + verify
  +-----------+  ---------->  +----------+  ----------------->  +--------+
  | planned/  |               | active/  |                     | done/  |
  +-----------+               +----------+                     +--------+
                                     |
                                     | replace with newer record
                                     v
                               +---------------+
                               | superseded/   |
                               +---------------+

  rule: records in `superseded/` must set `Superseded by` to the replacement record
```

How to read it:

- `planned/` means approved or drafted work that has not started.
- `active/` means work is currently in progress.
- `done/` means work completed and remains as project history.
- `superseded/` means the old record was replaced by a newer one.

Use the helper to move records safely:

```bash
bash scripts/set-feature-record-lifecycle.sh feature_records/planned/<record>.md active
bash scripts/set-feature-record-lifecycle.sh feature_records/active/<record>.md done
bash scripts/set-feature-record-lifecycle.sh \
  --superseded-by feature_records/done/<replacement>.md \
  feature_records/active/<record>.md superseded
```

## Project Layout

```text
.agents/skills/        Project-local agent skills
config/change-contract-policy.sh
                       Repo-local change-contract policy
.github/workflows/     Generic CI and release workflow templates
benchmarks/            Optional micro-benchmark harness
cmake/                 Analyzer helper scripts
contrib/               Optional service/desktop integration examples
docs/                  Doxygen config and API-focused docs
scripts/               Hygiene, release, diagnostics, and init helpers
src/                   Example library and CLI
tests/                 Deterministic example tests and frame_test.h
feature_records/       Root template, README, and lifecycle subdirectories
```

## Tooling

Optional local tooling:

- `clang-format`
- `clang-tidy`
- `clazy-standalone` when using the example Qt-based UI stack
- `cppcheck`
- `doxygen`
- `gcovr` (for coverage HTML reports)
- `valgrind`
- `ninja-build`

Useful targets when the tools are installed:

```bash
cmake --build "$BUILD_DIR" --target format         # auto-format sources
cmake --build "$BUILD_DIR" --target format-check   # CI formatting check
cmake --build "$BUILD_DIR" --target clang-tidy
cmake --build "$BUILD_DIR" --target cppcheck
cmake --build "$BUILD_DIR" --target clazy
cmake --build "$BUILD_DIR" --target docs
cmake --build "$BUILD_DIR" --target lint            # aggregates analyzers
cmake --build "$BUILD_DIR" --target valgrind
cmake --build "$BUILD_DIR" --target coverage        # tests + gcov report
```

## Example Install Validation

```bash
INSTALL_DIR="$(mktemp -d /tmp/cpp-frame-install-XXXXXX)"
cmake --install "$BUILD_DIR" --prefix "$INSTALL_DIR"
test -x "$INSTALL_DIR/bin/frame_cli"
```

## Customizing This Frame

Typical first steps:

1. Run the init script to rename all placeholders:
   ```bash
   ./scripts/init-project.sh --name "Your Project Name"    # dry-run
   ./scripts/init-project.sh --name "Your Project Name" --apply
   ```
2. Replace the example library/CLI code in `src/`.
3. Update `AGENTS.md` with project-specific constraints.
4. Trim or extend `.agents/skills/` to match the real project.
5. Adapt workflows, scripts, and `contrib/` assets to the real runtime shape.
6. Add or replace the example tests with project-specific deterministic tests.
7. Treat `feature_records/` plans as tracked change contracts for substantive
   repo-owned work. Changes to repo-owned code, tests, scripts, docs, workflows,
   or top-level build/release files should update a non-template plan file.
   Place each record under the lifecycle directory that matches its `State`.
8. Keep `config/change-contract-policy.sh` as the source of truth for substantive
   path detection, required plan sections, evidence lanes, and default
   validation profiles.
9. Keep verifier evidence concrete: plans should record verifier commands,
   observed results, and any contract mismatches rather than only a prose note.
10. Use `bash scripts/set-feature-record-lifecycle.sh <record> <state>` to
    move records between lifecycle folders deterministically.

## Release And Docs

- Use [RELEASE_CHECKLIST.md](RELEASE_CHECKLIST.md) before publishing.
- The Doxygen main page lives in [docs/mainpage.md](docs/mainpage.md).
- Keep `feature_records/README.md`, `feature_records/TEMPLATE.md`, and lifecycle
  records under `feature_records/*/*.md` aligned with the enforced change-contract
  template and `config/change-contract-policy.sh` so lifecycle state, uncertainty,
  cost, ownership, the evidence matrix, and verifier command/result notes
  remain explicit.
- Keep publication-facing files free of machine-specific paths, broken local
  links, and generated artifacts.

## License

MIT. See [LICENSE](LICENSE).
