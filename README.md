# C++ Agentic Development Frame
[![Actions Status](https://github.com/n-n-code/cpp-agentic-development-frame/workflows/CI/badge.svg)](https://github.com/n-n-code/cpp-agentic-development-frame/actions)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

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

- portable C++20 library + CLI example
- no Qt requirement by default
- optional Qt/Clazy support as the example stack for projects that add a UI layer
- MIT license

## Quick Start

```bash
BUILD_DIR="$(mktemp -d /tmp/cpp-frame-build-XXXXXX)"
cmake -S . -B "$BUILD_DIR" -G Ninja
cmake --build "$BUILD_DIR" -j"$(nproc)"
ctest --test-dir "$BUILD_DIR" --output-on-failure
"$BUILD_DIR/frame_cli" --help
```

If you do not have `ninja-build`, omit `-G Ninja`.

## What This Frame Includes

- `AGENTS.md`: canonical repo contract for build/test/tooling/release rules
- `.agents/skills/`: generic project-local skills for coding, documenting,
  planning, security, release work, vendor-boundary work, and diagnostics
- `src/`: small example library + CLI
- `tests/`: deterministic example tests and `CTest` registration
- `docs/`: API-focused Doxygen setup
- `scripts/`: hygiene, commentary, release, and Valgrind helpers
- `.github/workflows/`: CI and release workflow templates
- `contrib/`: optional system integration examples
- `upcoming_features/`: tracked plan folder

## Project Layout

```text
.agents/skills/        Project-local agent skills
.github/workflows/     Generic CI and release workflow templates
cmake/                 Analyzer helper scripts
contrib/               Optional service/desktop integration examples
docs/                  Doxygen config and API-focused docs
scripts/               Hygiene, release, and diagnostics helpers
src/                   Example library and CLI
tests/                 Deterministic example tests
upcoming_features/     Forward-looking implementation plans
```

## Tooling

Optional local tooling:

- `clang-tidy`
- `clazy-standalone` when using the example Qt-based UI stack
- `doxygen`
- `valgrind`
- `ninja-build`

Useful targets when the tools are installed:

```bash
cmake --build "$BUILD_DIR" --target clang-tidy
cmake --build "$BUILD_DIR" --target clazy
cmake --build "$BUILD_DIR" --target docs
cmake --build "$BUILD_DIR" --target lint
cmake --build "$BUILD_DIR" --target valgrind
```

## Example Install Validation

```bash
INSTALL_DIR="$(mktemp -d /tmp/cpp-frame-install-XXXXXX)"
cmake --install "$BUILD_DIR" --prefix "$INSTALL_DIR"
test -x "$INSTALL_DIR/bin/frame_cli"
```

## Customizing This Frame

Typical first steps:

1. Rename the example targets, namespaces, and install paths.
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
