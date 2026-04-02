# Feature: Example Done Feature

This example record demonstrates how a completed item should look in
`feature_records/done/`.

## Motivation

Repo browsers should be able to distinguish completed work from active work
without opening every record individually.

## Proposed Behavior

Track an example feature that has already completed and passed its expected
documentation-level checks.

## Lifecycle

- State: done
- Supersedes: none
- Superseded by: none

## Contract

- Must remain true: Done records should remain in the repository as historical evidence rather than being deleted.
- Must become true: The repository should include a valid `done` example for browsing and onboarding.
- Success signals: Browsers can open `feature_records/done/` and see a finished record with completed implementation and verification notes.

## Uncertainty And Cost

- Product uncertainty: low
- Technical uncertainty: low
- Implementation cost: low
- Validation cost: low
- Notes: This is a stable example that illustrates the completed lifecycle state.

## Responsibilities

- Implementer: example-author
- Verifier: example-reviewer
- Approver: none

## Evidence Matrix

- Tests | impact=no | status=not_applicable | rationale=The example does not represent a real executable feature. | verifier_note=Example-only record.
- Docs | impact=yes | status=passed | rationale=The purpose of the record is to document the `done` lifecycle shape. | verifier_note=Reviewed for template consistency and completed-state readability.
- Analyzers | impact=no | status=not_applicable | rationale=No analyzer-relevant source changes are represented by this example. | verifier_note=Example-only record.
- Install validation | impact=no | status=not_applicable | rationale=Install behavior is out of scope for this example. | verifier_note=Example-only record.
- Release hygiene | impact=no | status=not_applicable | rationale=The example does not change release behavior. | verifier_note=Example-only record.

## Implementation Notes

- Owner: example-author
- Status: completed
- Notes: This example is complete and intentionally remains in the repository as historical reference.

## Verification Notes

- Owner: example-reviewer
- Status: completed
- Commands: example record review
- Observed result: The record demonstrates a completed lifecycle state with finished notes and no pending work.
- Contract mismatches: none

## Waivers

- Self-validation rationale: none

## Files to Add/Modify

- `feature_records/...` — example lifecycle documentation only

## Testing Strategy

Keep the checker passing and leave the record in `done/` as a permanent
historical example.

## Open Questions

None.
