# Feature: Example Superseded Feature

This example record demonstrates how replaced work should look in
`feature_records/superseded/`.

## Motivation

Repo browsers should be able to see that some historical records were replaced
by newer ones, not merely completed.

## Proposed Behavior

Track an example record whose original plan was replaced by a newer record in
`feature_records/done/`.

## Lifecycle

- State: superseded
- Supersedes: none
- Superseded by: feature_records/done/2026-03-20-example-done-feature.md

## Contract

- Must remain true: Superseded records should remain readable as historical context and should clearly point at their replacement.
- Must become true: The repository should include a valid `superseded` example that demonstrates the replacement link.
- Success signals: Browsers can open `feature_records/superseded/` and immediately see which newer record replaced it.

## Uncertainty And Cost

- Product uncertainty: low
- Technical uncertainty: low
- Implementation cost: low
- Validation cost: low
- Notes: This is a documentation example showing the superseded lifecycle relationship.

## Responsibilities

- Implementer: example-author
- Verifier: example-reviewer
- Approver: none

## Evidence Matrix

- Tests | impact=no | status=not_applicable | rationale=The example does not represent executable behavior. | verifier_note=Example-only record.
- Docs | impact=yes | status=passed | rationale=The example exists to document how a superseded record should look. | verifier_note=Reviewed for template consistency and replacement-pointer clarity.
- Analyzers | impact=no | status=not_applicable | rationale=No analyzer-relevant source changes are represented by this example. | verifier_note=Example-only record.
- Install validation | impact=no | status=not_applicable | rationale=Install behavior is out of scope for this example. | verifier_note=Example-only record.
- Release hygiene | impact=no | status=not_applicable | rationale=The example does not change release behavior. | verifier_note=Example-only record.

## Implementation Notes

- Owner: example-author
- Status: completed
- Notes: This example remains in the repository only as historical context after being replaced.

## Verification Notes

- Owner: example-reviewer
- Status: completed
- Commands: example record review
- Observed result: The record clearly points to its replacement in `feature_records/done/2026-03-20-example-done-feature.md`.
- Contract mismatches: none

## Waivers

- Self-validation rationale: none

## Files to Add/Modify

- `feature_records/...` — example lifecycle documentation only

## Testing Strategy

Keep the checker passing and preserve the `Superseded by` pointer to the
replacement record.

## Open Questions

None.
