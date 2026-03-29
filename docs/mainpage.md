# C++ Agentic Development Frame Documentation

This documentation is generated from repo-owned C++ headers under `src/`.
It focuses on the public API, build/test contracts, and project-structure
expectations rather than end-user setup.

## Default Shape

- `project_core` is the example library target
- `frame_cli` is the example command-line target
- build, docs, tests, analyzers, and release helpers are first-class repo
  concerns
- Qt/Clazy form the example UI stack rather than the baseline project shape
- deterministic tests live under `tests/`
- future plans live under `upcoming_features/`

## Core API Surface Covered Here

- `frame::trimCopy()` provides a small string utility with clear ownership
  semantics
- `frame::normalizedProjectName()` shows a deterministic library helper that is
  easy to test and document

## Repo Expectations

- keep repo-owned public headers documented
- keep analyzer and release scripts aligned with current project behavior
- keep publication-facing files free of generated output and machine-specific
  paths
- use `README.md` and `RELEASE_CHECKLIST.md` for build, release, and publishing
  guidance
