# Release Checklist

Use this checklist before publishing the repository or cutting a release.

## Repository Hygiene

- Confirm the working tree does not contain generated build artifacts.
- Run:

```bash
bash scripts/check-release-hygiene.sh
bash scripts/check-change-contracts.sh
```

- Verify there are no machine-specific home-directory paths or absolute local
  Markdown links in tracked files.
- Confirm no generated binaries or large release payloads are tracked in Git.
- Confirm tracked feature plans keep explicit contract, uncertainty/cost,
  lifecycle state, responsibilities, evidence matrices, implementation notes,
  verification command/result notes, and waiver rationale.
- If any record changed lifecycle, prefer `bash scripts/set-feature-record-lifecycle.sh`
  over manual moves so the state field and directory stay aligned.
- Confirm `config/change-contract-policy.sh`, `feature_records/README.md`,
  `feature_records/TEMPLATE.md`, and
  `scripts/check-change-contracts.sh` still describe the same contract.

## Licensing And Provenance

- Confirm the root [LICENSE](LICENSE) file matches the intended project
  license.
- Review any third-party notice/provenance files for accuracy.
- Confirm no release-only binaries, models, datasets, or large artifacts are
  tracked in the repository source tree.

## Build And Test

- For the automated pre-install portion of this section, you can run:

```bash
bash scripts/run-release-checklist.sh
```

- Confirm the build configuration still matches the repo baseline language
  standard, which is C++23.

- Configure a fresh out-of-tree build (prefer presets when available):

```bash
cmake --preset ci
```

Or manually:

```bash
BUILD_DIR="$(mktemp -d /tmp/cpp-frame-build-XXXXXX)"
cmake -S . -B "$BUILD_DIR" -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

- Build:

```bash
cmake --build "$BUILD_DIR" -j"$(nproc)"
```

- Verify formatting compliance:

```bash
cmake --build "$BUILD_DIR" --target format-check
```

- Run tests:

```bash
ctest --test-dir "$BUILD_DIR" --output-on-failure
```

- Run the release-memory diagnostics lane:

```bash
bash scripts/run-valgrind.sh "$BUILD_DIR"
```

- Run analyzers when available:

```bash
cmake --build "$BUILD_DIR" --target clang-tidy
cmake --build "$BUILD_DIR" --target cppcheck
cmake --build "$BUILD_DIR" --target clazy
```

- Treat `clazy` as part of the Qt-based UI variant rather than a baseline lane
  for CLI-only projects. Treat `cppcheck` as optional when not installed.

- If repo-owned public headers or docs changed, also run:

```bash
cmake --build "$BUILD_DIR" --target docs
```

- Validate CLI startup:

```bash
"$BUILD_DIR/frame_cli" --help
```

## Install Validation

- Install into a temporary prefix:

```bash
INSTALL_DIR="$(mktemp -d /tmp/cpp-frame-install-XXXXXX)"
cmake --install "$BUILD_DIR" --prefix "$INSTALL_DIR"
```

- Confirm the installed tree contains:
  - `bin/frame_cli`
  - license files under `share/licenses/<project-name>`
  - any expected docs, assets, or service/desktop files if the project ships
    them

## Documentation And User Flow

- Review [README.md](README.md) for consistency with current behavior.
- Review [AGENTS.md](AGENTS.md) for alignment with current tooling and
  workflows.
- Review [docs/mainpage.md](docs/mainpage.md) and [docs/Doxyfile.in](docs/Doxyfile.in)
  if the release touched repo-owned API docs or docs/CI wiring.
- Confirm `CMakePresets.json` presets still match the documented workflows.
- Confirm docs still describe the intended build, install, test, and release
  flows.

## Final Sanity Pass

- Re-read the planned release notes or repository description.
- Confirm the current state is suitable for a clean public snapshot.
- If publishing to GitHub, verify the workflow files exist under
  `.github/workflows/`.
