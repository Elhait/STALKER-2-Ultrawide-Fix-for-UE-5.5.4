# COMPLETE_BACKLOG_AND_TESTING_SUMMARY

## Objective

Reconcile the project backlog with the evidence established during the
2.0.4 gameplay/cinematic work and update `TESTING_AND_RESEARCH.md` with the
current confirmed, closed, deferred and unresolved states.

## Established evidence and current state

The unified `STALKER2UltrawideFix.asi` has runtime evidence on 2.0.4 for the
gameplay signature resolver, runtime-camera cinematic aspect handling,
signature-resolved cinematic FOV boundaries, configurable aspect policies and
SHA-256 startup identity logging. Several earlier research branches are
closed, while weapon/viewmodel and future-version compatibility work remain
open or deferred.

## Approved scope

- Read every task plan in `backlog`.
- Move only completed or explicitly closed plans to `backlog/complete`.
- Preserve active, pending, deferred and unresolved research plans in place.
- Update `TESTING_AND_RESEARCH.md` with evidence-backed 2.0.4 results,
  configuration behavior, release boundaries and remaining work.
- Update moved plan statuses only where needed to reflect supplied evidence.

## Explicit non-goals

- No source, build, ASI or INI changes.
- No deletion of task plans, logs, Ghidra projects or research evidence.
- No new runtime tests or compatibility claims.
- No Git staging, commit or history rewrite.

## Expected files or areas

- `backlog/*.md`
- `backlog/complete/*.md`
- `TESTING_AND_RESEARCH.md`

## Batches and validation

### Batch 1 — classification

Classify plans as complete/closed or still active from their contents and the
established runtime evidence.

### Batch 2 — documentation update

Update the testing summary with confirmed runtime behavior, configuration
policy, hash identity evidence, 2.0.4 limits and deferred research.

### Batch 3 — archival move

Move the exact completed/closed plan files into `backlog/complete` without
overwriting existing files.

### Batch 4 — final review

Run Markdown whitespace checks where available and perform a read-only Git
status/diff/path review against this plan.

## Risks and rollback or safe-failure behavior

Classification errors can hide active work. Keep unresolved or pending plans
in `backlog`; moving a plan is recoverable by moving it back to its original
directory. Never overwrite an existing file in `backlog/complete`.

## Stop conditions and phase gates

Stop if a plan has contradictory status/evidence that cannot be resolved from
the conversation or project files, if a destination filename already exists,
or if unrelated files appear changed. Do not claim future-patch support from
the 2.0.4 results.

## Final Git review

Confirm the summary changes and moved paths match this plan, distinguish
completed/remaining/deferred/not-runtime-validated items, and preserve all
intentional user changes.
