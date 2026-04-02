#!/usr/bin/env bash

# Repo-local change-contract policy consumed by the checker and overlay skill.

FRAME_CONTRACT_PLAN_DIR="feature_records"
FRAME_CONTRACT_TEMPLATE_BASENAME="TEMPLATE.md"

FRAME_CONTRACT_SUBSTANTIVE_PATH_PATTERNS=(
    'src/*'
    'tests/*'
    'scripts/*'
    'cmake/*'
    'docs/*'
    'benchmarks/*'
    'contrib/*'
    '.github/workflows/*'
)

FRAME_CONTRACT_SUBSTANTIVE_TOP_LEVEL_FILES=(
    'CMakeLists.txt'
    'CMakePresets.json'
    'README.md'
    'AGENTS.md'
    'RELEASE_CHECKLIST.md'
)

FRAME_CONTRACT_REQUIRED_SECTIONS=(
    '## Motivation'
    '## Proposed Behavior'
    '## Lifecycle'
    '## Contract'
    '## Uncertainty And Cost'
    '## Responsibilities'
    '## Evidence Matrix'
    '## Implementation Notes'
    '## Verification Notes'
    '## Files to Add/Modify'
    '## Testing Strategy'
    '## Waivers'
)

FRAME_CONTRACT_LIFECYCLE_VALUES=(
    'planned'
    'active'
    'superseded'
    'done'
)

FRAME_CONTRACT_UNCERTAINTY_VALUES=(
    'low'
    'medium'
    'high'
)

FRAME_CONTRACT_EVIDENCE_STATUS_VALUES=(
    'passed'
    'waived'
    'not_applicable'
    'missing'
)

FRAME_CONTRACT_YES_NO_VALUES=(
    'yes'
    'no'
)

FRAME_CONTRACT_IMPLEMENTATION_STATUS_VALUES=(
    'planned'
    'in_progress'
    'completed'
)

FRAME_CONTRACT_VERIFICATION_STATUS_VALUES=(
    'pending'
    'in_progress'
    'completed'
)

FRAME_CONTRACT_EVIDENCE_LANES=(
    'Tests'
    'Docs'
    'Analyzers'
    'Install validation'
    'Release hygiene'
)

FRAME_CONTRACT_CHECKER_COMMAND='bash scripts/check-change-contracts.sh'

FRAME_CONTRACT_VALIDATION_PROFILE_DOCS=(
    'bash scripts/check-change-contracts.sh'
    'bash scripts/check-release-hygiene.sh'
)

FRAME_CONTRACT_VALIDATION_PROFILE_CODE=(
    'bash scripts/check-change-contracts.sh'
    'cmake --preset dev'
    'cmake --build --preset dev'
    'ctest --preset dev'
    './build/dev/frame_cli --help'
    'cmake --build build/dev --target format-check'
    'cmake --build build/dev --target lint'
)

FRAME_CONTRACT_VALIDATION_PROFILE_RELEASE=(
    'bash scripts/check-change-contracts.sh'
    'bash scripts/check-release-hygiene.sh'
    'cmake --build build/dev --target docs'
    'bash scripts/run-valgrind.sh build/dev'
)
