# Backlog and Build-Artifact Organization Task Plan

Status: Closed — all approved artifact and task-plan moves completed and verified.

## Objective

Organize reusable root build artifacts and all project task plans without deleting needed evidence or changing source behavior.

## Established evidence and current state

- Eight `.obj` files are present directly in the project root.
- Four `*_TASK_PLAN.md` files are present directly in the project root.
- The project `.gitignore` ignores `*.obj`.
- Existing plans describe completed and ongoing research phases; their content must be preserved while paths and status headers are normalized.

## Approved scope

- Move only root-level `.obj` files into `build-artifacts/obj/`.
- Remove the project-local `*.obj` ignore rule so retained artifacts can be tracked when appropriate.
- Move all project `*_TASK_PLAN.md` files into `backlog/`; move completed plans into `backlog/complete/`.
- Add English status lines immediately after each plan title: `Planned`, `In Progress`, `Testing` or `Closed`, with a closure reason for `Closed` plans.
- Preserve plan contents and update only path references made stale by the moves.

## Explicit non-goals

- Do not delete `.obj` files, plans, source, build outputs or research evidence.
- Do not modify C++ behavior, build scripts, release assets or Git history.
- Do not reorganize nested research scripts or unrelated Markdown documents.
- Do not update `TESTING_AND_RESEARCH.md` in this cleanup task; use `backlog/complete` as the later source for that work.

## Expected files and areas

- `backlog/` and `backlog/complete/`.
- `build-artifacts/obj/`.
- Project `.gitignore`.
- Existing four task plans and references in project documentation.

## Batches and validation

### Batch 1 — Inventory and plan

- Confirm exact root `.obj` and task-plan paths.
- Create this plan before moving any files.

### Batch 2 — Artifact and plan organization

- Move the eight root `.obj` files to `build-artifacts/obj/`.
- Move active/future plans to `backlog/` and completed plans to `backlog/complete/`.
- Normalize English status headers and closure reasons.
- Update stale references caused by the moves.

### Batch 3 — Review

- Verify no root-level `.obj` or task plans remain.
- Verify every moved file exists at its intended destination and contents are preserved.
- Check `.gitignore`, relevant diffs and repository status.
- No build or runtime validation is required.

## Risks and rollback / safe failure

- File moves are reversible by restoring each exact path; no broad wildcard deletion is allowed.
- If a plan's completion state is ambiguous, keep it in `backlog/` with `In Progress` or `Planned` rather than misclassifying it.
- If a reference cannot be updated safely, stop before deleting or moving its source path.

## Stop conditions and phase gates

- Stop if any target path already contains a different file.
- Stop if a plan's status cannot be classified from its content without inventing evidence.
- Stop before touching files outside the approved artifact, plan and direct-reference scope.

## Expected final Git review

- Perform read-only Git status/diff review after the cleanup.
- Confirm changed paths match this plan and no Git state-changing command is used.
- Record the factual cleanup result in `backlog/TASKLOG.md`.
