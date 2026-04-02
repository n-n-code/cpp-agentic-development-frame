# Feature: Development Contract System Unification

## Motivation

The current worktree no longer represents a few isolated slices. It now adds a
complete repo-owned development contract system: policy, feature-record
lifecycle layout, checker, lifecycle helper, examples, docs, and a portable
combined skill. The contract record should match that reality instead of
pretending the changes are still separate active efforts.

## Proposed Behavior

Treat the current uncommitted repository changes as one active feature that
establishes the full development contract system. The system should provide a
repo-owned policy file, lifecycle-grouped `feature_records/`, a checker, a
lifecycle transition helper, seed examples, aligned docs, and a portable skill
that explains how to build the same system in another repository.

## Lifecycle

- State: done
- Supersedes: none
- Superseded by: none

## Contract

- Must remain true: The development contract system stays repo-owned, deterministic, Markdown-based, and enforceable through local scripts and CI.
- Must become true: The current worktree is represented by a single active record that covers policy, lifecycle layout, transition helper, docs, and portable skill guidance together.
- Success signals: One active record describes the in-flight development contract system, the example lifecycle records remain browseable, and the checker plus repo hygiene lanes still pass.

## Uncertainty And Cost

- Product uncertainty: low
- Technical uncertainty: medium
- Implementation cost: medium
- Validation cost: medium
- Notes: The system shape is now clear, but the repository still has one large uncommitted slice spanning scripts, docs, skills, and record layout.

## Responsibilities

- Implementer: codex
- Verifier: codex
- Approver: repo-maintainer

## Evidence Matrix

- Tests | impact=yes | status=passed | rationale=The checker and lifecycle helper are core workflow behavior and need direct shell coverage. | verifier_note=Validated with `bash scripts/test-feature-record-lifecycle.sh` and `bash scripts/test-change-contracts.sh`.
- Docs | impact=yes | status=passed | rationale=README, AGENTS, release checklist, feature-record docs, and skills all changed as part of this unified workflow slice. | verifier_note=Reviewed README, AGENTS, RELEASE_CHECKLIST, feature_records docs, and contract-related skills for alignment.
- Analyzers | impact=no | status=not_applicable | rationale=The current work is shell, Markdown, and repo workflow structure rather than C++ analyzer-sensitive source changes. | verifier_note=No analyzer lane was required for this slice.
- Install validation | impact=no | status=not_applicable | rationale=Install layout and shipped runtime assets are unchanged. | verifier_note=Install validation remained out of scope.
- Release hygiene | impact=yes | status=passed | rationale=This feature changes repo process and must remain aligned with hygiene enforcement. | verifier_note=Validated with `bash scripts/check-change-contracts.sh` and `bash scripts/check-release-hygiene.sh`.

## Implementation Notes

- Owner: codex
- Status: in_progress
- Notes: Consolidated the current worktree under one umbrella contract and removed intermediate superseded real records so the initial commit stays simpler.

## Verification Notes

- Owner: codex
- Status: completed
- Commands: `bash scripts/test-feature-record-lifecycle.sh`; `bash scripts/test-change-contracts.sh`; `bash scripts/check-change-contracts.sh`; `bash scripts/check-release-hygiene.sh`
- Observed result: The helper tests, checker fixtures, direct checker run, and release hygiene lane all pass with a single real active contract plus the example lifecycle records.
- Contract mismatches: none

## Waivers

- Self-validation rationale: This repo-owned workflow change is being implemented and verified in one session, with explicit verifier evidence recorded.

## Files to Add/Modify

- `config/...` — repo-owned contract policy
- `feature_records/...` — lifecycle tree, examples, and active contract records
- `scripts/...` — checker, lifecycle helper, release helpers, and shell tests
- `README.md`, `AGENTS.md`, `RELEASE_CHECKLIST.md` — maintainer-facing workflow guidance
- `.agents/skills/...` — repo-local overlays and the portable combined skill

## Testing Strategy

Keep the checker fixture suite, lifecycle-helper shell tests, direct checker
run, and release hygiene lane passing while the unified feature remains active.

## Open Questions

Whether the next slice should add a helper for creating new feature records from
the template, not just transitioning existing ones.
