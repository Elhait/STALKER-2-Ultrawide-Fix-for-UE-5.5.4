# Release Preparation v0.5.0 — A8 Git Pre-commit Review

Status: PASS — review complete; no commit performed.

## Repository state

- Branch: `main`
- HEAD: `21c884f chore: organize reverse-engineering research archive`
- Working tree contains 51 staged paths, 4 unstaged tracked paths and 8
  untracked A0–A7 release-preparation reports before this A8 report.
- The staged index predates some final documentation/report edits. This is a
  stale-index condition to resolve during B0 staging, not an authorization to
  edit the index during A8.
- `git diff --check` and `git diff --cached --check` completed without
  whitespace errors; Git reported only existing LF/CRLF conversion warnings.

## Proposed commit scope

### Include — v0.5.0 production/release changes

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `README.md`
- `RELEASE_NOTES.md`
- `TESTING_AND_RESEARCH.md`
- `GITHUB_RELEASE_BODY.md`
- `NEXUS_DESCRIPTION.md`
- `backlog/RELEASE_PREPARATION_TASK_PLAN.md`
- `backlog/TASKLOG.md`

The final working-tree content of documentation files must be staged during
B0; the older staged versions must not silently override the final A4 state.

### Include — completed research/provenance for this development cycle

- Completed plan moves under `research/completed/`.
- Deferred plan move under `research/deferred/`.
- Active plan additions/updates under `backlog/active/`.
- `research/reports/RELEASE_PREPARATION_v0.5.0.md` as the superseded-run
  provenance record.
- The untracked A0–A7 preparation reports and this A8 report, if the project
  wants the release-gate audit trail committed with the development cycle.

These paths preserve completed evidence and plan provenance; they do not
promote active/deferred research into the product scope.

## Exclude from commit

- Diagnostic/test source files under `src/` such as dialogue boundary traces,
  feasibility tracers and weapon/viewmodel observers.
- Diagnostic/test build scripts under `build-artifacts/test-scripts/`.
- Generated `.obj` files under `build-artifacts/obj/`.
- `build-artifacts/test-asi/STALKER2UltrawideFix.ini`, a generated test/config
  artifact shown as a staged deletion.
- Ignored `release-assets/` payload files and historical ZIPs. The verified A6
  archive is a release artifact, not a repository source change, and is already
  tracked separately by its A6 SHA.

## Unrelated or ambiguous changes

- No unrelated source or documentation change was identified in the inspected
  paths.
- The generated-object and diagnostic-source groups are intentionally excluded
  even though they are currently staged; B0 must resolve those stale index
  entries explicitly.
- The choice to commit the A0–A8 release reports is a repository-history choice,
  not a product correctness blocker. The proposed scope includes them so the
  release evidence is reproducible; omit them only by explicit user decision.

## Proposed commit title

`release: add dialogue zoom controls and unified v0.5.0 release preparation`

This title reflects the actual production and release-preparation delta rather
than only the version number.

## A8 gate

The complete repository state was inspected in staged, unstaged and untracked
forms. The proposed include/exclude scope is separated, stale index entries are
identified, the release binary/archive are not changed, and no commit, staging,
tag or push was performed.

Git pre-commit review: PASS.

Next step: Final Phase-A report, then await explicit user approval before B0.
