# AGENTS.md

## Overview

This repository is a generic CMake-first C++ project frame optimized for
agentic development.

Baseline shape:

- CMake is the only supported build system
- Out-of-tree builds are the default workflow
- Repo-owned code should stay portable, local-first, and easy to validate
- One small library target plus one CLI target form the default example shape
- Deterministic `CTest`, `clang-tidy`, Doxygen, release-hygiene, and Valgrind
  lanes are part of the maintained contract
- Qt/Clazy provide the example UI stack, not the baseline assumption
- Public docs are generated from repo-owned headers and `docs/mainpage.md`
- Feature plans live under `upcoming_features/` as tracked Markdown files only

Repository principles:

- Keep generated artifacts out of the repository
- Keep publication-facing files free of machine-specific paths and broken local
  links
- Prefer small explicit abstractions, clear ownership, and incremental changes
- Keep CI, docs, tests, and release scripts aligned with shipped behavior
- Treat vendored third-party code as subtree/vendor content, not normal project
  code

## Key Paths

- `src/`: repo-owned library and CLI code
- `tests/`: deterministic example tests and test registration
- `docs/`: Doxygen config and API-focused main page
- `scripts/`: hygiene, release, and diagnostics helpers
- `contrib/`: optional service/desktop integration examples
- `cmake/`: reusable analyzer helper scripts
- `.agents/skills/`: project-local agent overlays and merged skills
- `.github/workflows/`: generic CI and release workflow templates
- `upcoming_features/`: forward-looking implementation plans

## Build And Validation

Read `README.md` first before changing build, setup, or release behavior.

Use an out-of-tree build:

```bash
BUILD_DIR="$(mktemp -d /tmp/cpp-frame-build-XXXXXX)"
cmake -S . -B "$BUILD_DIR" -G Ninja
cmake --build "$BUILD_DIR" -j"$(nproc)"
```

If `ninja-build` is unavailable, omit `-G Ninja`.

Optional sanitizer lane:

```bash
cmake -S . -B "$BUILD_DIR" -G Ninja \
  -DFRAME_ENABLE_ASAN=ON \
  -DFRAME_ENABLE_UBSAN=ON
```

Lightweight default validation:

```bash
"$BUILD_DIR/frame_cli" --help
```

Use the smallest validation set that proves the change, then extend as needed:

- `ctest --test-dir "$BUILD_DIR" --output-on-failure`
- `cmake --build "$BUILD_DIR" --target clang-tidy`
- `cmake --build "$BUILD_DIR" --target clazy` when the project uses the
  example Qt-based UI stack and the tool is available
- `cmake --build "$BUILD_DIR" --target lint`
- `cmake --build "$BUILD_DIR" --target docs`
- `bash scripts/run-valgrind.sh "$BUILD_DIR"`
- `bash scripts/check-release-hygiene.sh`

If install layout or shipped assets change, validate a temporary install prefix:

```bash
INSTALL_DIR="$(mktemp -d /tmp/cpp-frame-install-XXXXXX)"
cmake --install "$BUILD_DIR" --prefix "$INSTALL_DIR"
```

## Testing Rules

- Keep repo-owned tests deterministic and headless under `CTest`
- Keep `WHAT/HOW/WHY` commentary near the start of real test bodies; the repo
  scripts enforce that contract
- Prefer pure helper seams and injected fakes over environment-heavy tests
- Prefer parser, config, algorithm, and boundary-helper coverage before device,
  network, UI, or platform integration coverage
- If Qt GUI or Widgets tests are added, make `CTest` registration itself set
  `QT_QPA_PLATFORM=offscreen`

## Coding Guidance

- Keep changes targeted; do not reformat unrelated code
- Prefer small direct classes and explicit ownership/lifetime over broad
  abstraction layers
- Favor `const`, `override`, `explicit`, `[[nodiscard]]`, narrow enums, RAII,
  rule-of-zero types, and clear invariants
- Treat raw pointers and references as non-owning observers unless ownership
  transfer is explicit
- Prefer standard library value types over raw arrays and manual bounds
  handling
- When helper functions take multiple adjacent same-shaped parameters, prefer a
  small request struct if it improves clarity and avoids
  `bugprone-easily-swappable-parameters`
- Keep product/project terminology app-owned and consistent; reserve vendor-
  specific wording for vendor-boundary code

## Tooling Guidance

- Treat `clang-tidy`, Doxygen, hygiene scripts, and CI as maintained checks,
  not optional extras
- Prefer the repo-owned `clang-tidy` target from a configured build tree so the
  same compile database drives local and CI analysis
- Keep analyzer fixes targeted to repo-owned code; do not churn vendored or
  generated output
- Prefer fixing code over weakening analyzer configuration
- In Qt-heavy variants, treat `misc-include-cleaner` and
  `readability-redundant-access-specifiers` as low-value noise unless the tool
  behavior improves
- Prefer the repo-owned Valgrind runner over ad hoc Memcheck commands so leak
  policy and target selection stay aligned

## Dependencies And Release Rules

- Treat third-party code as vendored/subtree-managed source
- Prefer app-side integration changes before touching vendored code
- If vendored code changes, document why and update provenance/notice files
- Do not commit generated binaries, model artifacts, or large release payloads
- Keep README, release checklist, workflows, install rules, and shipped assets
  aligned with each other

## Agent Workflow

- Read `README.md` first, then the touched files before editing
- Prefer targeted changes over speculative cleanup
- Keep `README.md`, `AGENTS.md`, `.agents/skills/`, scripts, and workflows
  aligned when the repo workflow changes
- Store forward-looking feature plans under `upcoming_features/` as tracked
  Markdown files; update the existing plan rather than scattering notes
- Do not leave generated artifacts in the repo tree
- Do not assume the workspace is a valid Git repo; if Git commands fail,
  continue with file-based validation and note the limitation

## When Unsure

- Optimize for the current implementation, not hypothetical future platforms
- Prefer portable, buildable, testable defaults
- Ask before making architectural expansions that widen the frame far beyond a
  generic C++ project template
