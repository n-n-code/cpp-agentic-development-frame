# AGENTS.md

## Overview

This repository is a generic CMake-first C++ project frame optimized for
agentic development.

Baseline shape:

- CMake is the only supported build system
- C++23 is the baseline language standard
- Out-of-tree builds are the default workflow
- Repo-owned code should stay portable, local-first, and easy to validate
- One small library target plus one CLI target form the default example shape
- Deterministic `CTest`, `clang-tidy`, `clang-format`, `cppcheck`, Doxygen,
  release-hygiene, Valgrind, and coverage lanes are part of the maintained
  contract
- Qt/Clazy provide the example UI stack, not the baseline assumption
- Public docs are generated from repo-owned headers and `docs/mainpage.md`
- Feature plans live under `feature_records/` as tracked Markdown files
  using lifecycle subdirectories such as `active/`, `planned/`, `done/`, and
  `superseded/` (see `feature_records/TEMPLATE.md` for the expected format)
- `config/change-contract-policy.sh` is the repo-local source of truth for change-
  contract paths, required sections, evidence lanes, and default validation
  profiles
- Feature plans act as change contracts: they must declare maintained/new
  contract points, lifecycle state, uncertainty/cost bands, implementer and
  verifier ownership, an evidence matrix, implementation notes, verification
  command/result notes, and any waivers explicitly
- CMake presets provide named configurations for common workflows

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
- `tests/`: deterministic example tests, test registration, and `frame_test.h`
  micro-framework
- `docs/`: Doxygen config and API-focused main page
- `scripts/`: hygiene, release, and diagnostics helpers
- `contrib/`: optional service/desktop integration examples
- `cmake/`: reusable analyzer helper scripts
- `.agents/skills/`: project-local agent overlays and merged skills
- `config/change-contract-policy.sh`: repo-local change-contract policy shared by
  the checker and overlay skill
- `.github/workflows/`: generic CI and release workflow templates
- `benchmarks/`: optional chrono-based micro-benchmarks and `frame_bench.h`
  harness
- `feature_records/`: root template/README plus lifecycle-grouped feature records

## Build And Validation

Read `README.md` first before changing build, setup, or release behavior.

Prefer CMake presets for common workflows:

```bash
cmake --preset dev
cmake --build build/dev -j"$(nproc)"
```

Available presets: `dev` (debug + sanitizers), `release` (optimized + LTO),
`ci` (matches GitHub Actions), `coverage` (gcov instrumentation).

Manual out-of-tree build (when presets are unavailable):

```bash
BUILD_DIR="$(mktemp -d /tmp/cpp-frame-build-XXXXXX)"
cmake -S . -B "$BUILD_DIR" -G Ninja
cmake --build "$BUILD_DIR" -j"$(nproc)"
```

If `ninja-build` is unavailable, omit `-G Ninja`.

Optional sanitizer lane (included in the `dev` preset):

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
- `cmake --build "$BUILD_DIR" --target format-check`
- `cmake --build "$BUILD_DIR" --target lint` (includes clang-tidy and cppcheck
  when available)
- `cmake --build "$BUILD_DIR" --target docs`
- `bash scripts/run-valgrind.sh "$BUILD_DIR"`
- `bash scripts/check-release-hygiene.sh`
- `bash scripts/check-change-contracts.sh`

If install layout or shipped assets change, validate a temporary install prefix:

```bash
INSTALL_DIR="$(mktemp -d /tmp/cpp-frame-install-XXXXXX)"
cmake --install "$BUILD_DIR" --prefix "$INSTALL_DIR"
```

## Formatting

Run `cmake --build "$BUILD_DIR" --target format` before committing to keep
source files consistently formatted. CI enforces `format-check` and will reject
unformatted code. The `.clang-format` config at the repo root defines the
canonical style.

## Coverage

Build with the `coverage` preset and run the `coverage` target:

```bash
cmake --preset coverage
cmake --build build/coverage
cmake --build build/coverage --target coverage
```

This runs tests and collects coverage via `gcovr`. HTML output lands in
`build/coverage/coverage/`. New code should maintain or improve line coverage.

## Benchmarks

Build with `-DFRAME_ENABLE_BENCHMARKS=ON` and run manually:

```bash
cmake --build "$BUILD_DIR" --target example_bench
"$BUILD_DIR/benchmarks/example_bench"
```

Add benchmarks for hot paths, algorithms, and performance-sensitive code.
Use `FRAME_BENCHMARK(name, iterations)` from `benchmarks/frame_bench.h`.

## Project Setup

When adapting this frame for a new project, use the init script:

```bash
./scripts/init-project.sh --name "Your Project Name"    # dry-run
./scripts/init-project.sh --name "Your Project Name" --apply
```

This renames all placeholder targets, namespaces, prefixes, and filenames.

## Testing Rules

- Keep repo-owned tests deterministic and headless under `CTest`
- Use the `frame_test.h` micro-framework: `FRAME_TEST(name)` for registration,
  `FRAME_EXPECT_EQ`, `FRAME_EXPECT_TRUE`, `FRAME_EXPECT_FALSE`,
  `FRAME_EXPECT_THROWS` for assertions, `FRAME_RUN_TESTS()` in main
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
- Read `config/change-contract-policy.sh` when the work may trigger a substantive
  change contract
- Prefer targeted changes over speculative cleanup
- Keep `README.md`, `AGENTS.md`, `.agents/skills/`, scripts, and workflows
  aligned when the repo workflow changes
- Store tracked feature plans under `feature_records/` using the lifecycle
  subdirectory that matches the plan's `State`; update the existing plan rather
  than scattering notes
- Treat substantive repo-owned changes in `src/`, `tests/`, `scripts/`, `docs/`,
  `.github/workflows/`, `cmake/`, `benchmarks/`, `contrib/`, and top-level
  build/release docs as contract-bearing work that should update a non-template
  feature plan unless `config/change-contract-policy.sh` narrows or extends that set
- Keep change-contract fields explicit: `missing` evidence states are not
  acceptable, waived evidence requires rationale, and implementer/verifier
  identity matches require a self-validation waiver
- Keep lifecycle state current so active, done, and superseded work can be
  distinguished explicitly in `feature_records/`
- Prefer `bash scripts/set-feature-record-lifecycle.sh <record> <state>` when
  transitioning feature records between lifecycle folders
- When moving a record to `superseded`, provide a replacement path so
  `Superseded by` stays explicit
- Keep implementation notes owned by the implementer and verification notes
  owned by the verifier so the plan itself records the responsibility split
- Keep verifier notes concrete: record commands run, observed results, and any
  contract mismatches rather than only a summary sentence
- Do not leave generated artifacts in the repo tree
- Do not assume the workspace is a valid Git repo; if Git commands fail,
  continue with file-based validation and note the limitation

## When Unsure

- Optimize for the current implementation, not hypothetical future platforms
- Prefer portable, buildable, testable defaults
- Ask before making architectural expansions that widen the frame far beyond a
  generic C++ project template
