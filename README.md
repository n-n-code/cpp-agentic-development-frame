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
- `.agents/skills/`: generic project-local skills for coding, documenting,
  planning, security, release work, vendor-boundary work, and diagnostics
- `src/`: small example library + CLI
- `tests/`: deterministic example tests, `CTest` registration, and
  `frame_test.h` micro-framework
- `docs/`: API-focused Doxygen setup
- `scripts/`: hygiene, commentary, release, and Valgrind helpers
- `.github/workflows/`: CI and release workflow templates
- `contrib/`: optional system integration examples
- `benchmarks/`: optional chrono-based micro-benchmark harness
- `CMakePresets.json`: named build configurations for dev/release/ci/coverage
- `upcoming_features/`: tracked plan folder

## Project Layout

```text
.agents/skills/        Project-local agent skills
.github/workflows/     Generic CI and release workflow templates
benchmarks/            Optional micro-benchmark harness
cmake/                 Analyzer helper scripts
contrib/               Optional service/desktop integration examples
docs/                  Doxygen config and API-focused docs
scripts/               Hygiene, release, diagnostics, and init helpers
src/                   Example library and CLI
tests/                 Deterministic example tests and frame_test.h
upcoming_features/     Forward-looking implementation plans
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

## Release And Docs

- Use [RELEASE_CHECKLIST.md](RELEASE_CHECKLIST.md) before publishing.
- The Doxygen main page lives in [docs/mainpage.md](docs/mainpage.md).
- Keep publication-facing files free of machine-specific paths, broken local
  links, and generated artifacts.

## License

MIT. See [LICENSE](LICENSE).
