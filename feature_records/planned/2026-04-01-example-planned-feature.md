# Feature: Example Planned Feature

This example record demonstrates how a not-yet-started item should look in
`feature_records/planned/`.

## Motivation

Repo browsers should be able to see what a planned record looks like without
having to infer it from the template alone.

## Proposed Behavior

Add a small example feature record that stays in the `planned/` folder and
shows a not-yet-started change contract.

## Lifecycle

- State: planned
- Supersedes: none
- Superseded by: none

## Contract

- Must remain true: Planned records should be explicit about intent, scope, and expected proof before work starts.
- Must become true: The repository should include an example record that demonstrates the `planned` lifecycle state.
- Success signals: Browsers can open `feature_records/planned/` and see a valid example without needing to interpret placeholders.

## Uncertainty And Cost

- Product uncertainty: low
- Technical uncertainty: low
- Implementation cost: low
- Validation cost: low
- Notes: This is documentation-by-example rather than a live implementation slice.

## Responsibilities

- Implementer: example-author
- Verifier: example-reviewer
- Approver: none

## Evidence Matrix

- Tests | impact=no | status=not_applicable | rationale=This example documents lifecycle shape rather than executable behavior. | verifier_note=Example-only record.
- Docs | impact=yes | status=passed | rationale=The purpose of the record is to document the planned lifecycle shape. | verifier_note=Reviewed for template and lifecycle consistency.
- Analyzers | impact=no | status=not_applicable | rationale=No analyzer-relevant source changes are represented by this example. | verifier_note=Example-only record.
- Install validation | impact=no | status=not_applicable | rationale=Install behavior is out of scope for this example. | verifier_note=Example-only record.
- Release hygiene | impact=no | status=not_applicable | rationale=The record is illustrative and does not change release behavior. | verifier_note=Example-only record.

## Implementation Notes

- Owner: example-author
- Status: planned
- Notes: Example placeholder for a feature that has not started implementation.

## Verification Notes

- Owner: example-reviewer
- Status: pending
- Commands: pending
- Observed result: Pending because the example represents a record before work begins.
- Contract mismatches: none

## Waivers

- Self-validation rationale: none

## Files to Add/Modify

- `src/...` — example future implementation surface
- `tests/...` — example future validation surface
- `README.md` — optional future documentation update

## Testing Strategy

When the feature starts, validate it with the smallest proving set and update
this record from `planned/` to the appropriate lifecycle folder.

## Open Questions

Whether the eventual implementation should remain docs-only or grow into a
real workflow helper.
