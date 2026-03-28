---
name: project-core-dev
description: Generic core-dev overlay for repo-owned C++ code, tests, and top-level CMake changes.
---

# Project Core Dev

Read `AGENTS.md` first. Use this skill for normal feature work and bug fixes in
repo-owned C++ code.

## Focus

- preserve buildability, tests, docs, and analyzer cleanliness
- keep changes narrow and behavior-oriented
- prefer app-side fixes before widening platform or vendor boundaries

## Validation

- build out of tree
- run `ctest --output-on-failure` for covered changes
- run `frame_cli --help` as a lightweight smoke test
- add docs/analyzer/Valgrind validation when the change surface justifies it
