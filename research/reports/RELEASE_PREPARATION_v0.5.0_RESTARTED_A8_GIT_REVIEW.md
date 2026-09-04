# v0.5.0 Release Preparation — A8 Git Scope Review

Date: 2026-09-04
Mode: read-only; no staging, unstaging, revert or commit performed.

## Repository state

- The index is stale relative to the working tree.
- `TESTING_AND_RESEARCH.md` is staged with the previous candidate hash, while
  the working tree contains the corrected renamed candidate hash.
- `git diff --check`: PASS.
- `git diff --cached --check`: PASS.
- Recent repository history includes an earlier release commit; this review
  does not rewrite or undo it.

## Proposed commit scope

Include after rebuilding the staging area from the approved scope:

- production dialogue/camera source change in
  `src/experimental_cinematic_21_9_combined_fix_204.cpp`;
- current v0.5.0 public and release documentation;
- permanent release playbook;
- relevant completed/deferred task plans and provenance records;
- A0–A8 release-preparation reports, including this report;
- intentional `build-artifacts` history, including the updated research object
  files and the renamed test build script, because this repository deliberately
  preserves the build/research path as demonstration provenance.

## Exclude from the release commit

- ignored `release-assets` binaries, INI files and ZIP archives;
- temporary logs, runtime captures, test INI files and research-only binaries;
- diagnostic-only source or temporary tracer not required by the stable build.

## Scope decisions recorded during A8 remediation

- The user confirmed that existing and updated `build-artifacts` files are
  intentional repository history. They are therefore INCLUDE, not accidental
  generated garbage.
- Completed `DIALOGUE_LIFETIME_INVALIDATION_TASK_PLAN.md` and
  `GAMEPLAY_FOV_SETTINGS_REBUILD_TRACE_TASK_PLAN.md` were moved to
  `research/completed`.
- `PRODUCT_IDENTITY_RENAME_TASK_PLAN.md` remains in `backlog/active` until the
  exact renamed-binary smoke is separately completed.

## Current staged-path risk

The current index contains the intentional `build-artifacts` changes and the
older staged content of `TESTING_AND_RESEARCH.md`. It must not be committed as
is. The final commit must stage the corrected working-tree documentation while
retaining the intentionally preserved build/research history.

## Proposed commit title

`release: add dialogue zoom controls for v0.5.0`

## Result

```yaml
A8: REVIEW REQUIRED / BLOCKED FOR COMMIT AS-IS
Reason: stale index; final staging must replace the old staged documentation
      while preserving intentional build-artifacts history
Commit performed: false
Tag/push performed: false
Next action: rebuild staging from the approved scope, then perform final
             staged-path review
```
