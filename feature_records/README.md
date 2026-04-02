# Feature Records

This directory stores tracked change contracts for substantive repo-owned work.

Lifecycle folders:

- `planned/` contains approved or drafted work that has not started yet.
- `active/` contains work currently in flight.
- `done/` contains completed records that remain part of the repo history.
- `superseded/` contains historical records replaced by a newer record.

Rules:

- A record's folder must match its `## Lifecycle` `State`.
- `TEMPLATE.md` stays at the root and is used for all new records.
- `superseded/` records must still point at their replacement via `Superseded by`.
- Substantive repo changes must update a non-template record under one of the lifecycle folders.
- Use `bash scripts/set-feature-record-lifecycle.sh <record> <state>` to move a
  record between lifecycle folders and update its `Lifecycle` state field.
- When moving to `superseded`, pass `--superseded-by feature_records/<state>/<replacement>.md`.

Examples included:

- `planned/2026-04-01-example-planned-feature.md`
- `active/2026-04-01-example-active-feature.md`
- `done/2026-03-20-example-done-feature.md`
- `superseded/2026-03-10-example-superseded-feature.md`
