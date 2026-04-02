# Feature: Example Active Feature

This example record demonstrates how an in-flight item should look in
`feature_records/active/`.

## Motivation

Repo browsers should be able to identify active work immediately from the
directory tree and from a concrete example record.

## Proposed Behavior

Track an example feature that is currently in progress and uses the `active`
lifecycle state.

## Lifecycle

- State: active
- Supersedes: none
- Superseded by: none

## Contract

- Must remain true: Active records should communicate that work is underway and still needs final verification or completion.
- Must become true: The repository should include a valid `active` example for browsing and onboarding.
- Success signals: Browsers can open `feature_records/active/` and see a record that reads like in-flight work rather than a finished historical note.

## Uncertainty And Cost

- Product uncertainty: low
- Technical uncertainty: low
- Implementation cost: low
- Validation cost: low
- Notes: This example exists to show folder semantics rather than to describe shipped behavior.

## Responsibilities

- Implementer: example-author
- Verifier: example-reviewer
- Approver: none

## Evidence Matrix

- Tests | impact=no | status=not_applicable | rationale=The example does not represent a real executable change. | verifier_note=Example-only record.
- Docs | impact=yes | status=passed | rationale=The example is itself documentation for the active lifecycle shape. | verifier_note=Reviewed for consistency with the template and folder naming.
- Analyzers | impact=no | status=not_applicable | rationale=No analyzer-relevant source changes are represented by this example. | verifier_note=Example-only record.
- Install validation | impact=no | status=not_applicable | rationale=Install behavior is out of scope for this example. | verifier_note=Example-only record.
- Release hygiene | impact=no | status=not_applicable | rationale=The example does not change release automation behavior. | verifier_note=Example-only record.

## Implementation Notes

- Owner: example-author
- Status: in_progress
- Notes: This example intentionally reads like work that has started but is not yet complete.

## Verification Notes

- Owner: example-reviewer
- Status: in_progress
- Commands: checker review pending final completion
- Observed result: The example currently illustrates the in-flight shape of an active record.
- Contract mismatches: none

## Waivers

- Self-validation rationale: none

## Files to Add/Modify

- `feature_records/...` — example lifecycle documentation only

## Testing Strategy

Keep the record checker passing while the example remains in the `active`
folder.

## Open Questions

None.
