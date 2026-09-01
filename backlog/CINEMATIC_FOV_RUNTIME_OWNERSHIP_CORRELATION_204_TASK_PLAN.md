# Cinematic FOV Runtime Ownership Correlation 2.0.4 — Task Plan

## Objective

Identify, from bounded existing 2.0.4 runtime evidence, an execution owner or
small owner neighborhood that is active during both native cinematic FOV
transitions and is correlated with the validated cinematic lifecycle.

## Established evidence and current state

- Current executable SHA-256:
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Native ENTER FOV converges `90.65574 -> 90.0` in approximately one second.
- Native EXIT FOV converges the modified cinematic value back to the ENTER
  baseline and returns to gameplay flags `0x04`.
- Manual and global experimental evidence proves that aspect plus corrected
  FOV gives the desired cinematic framing.
- Layout-based writer candidates, random register sampling, unsafe guard-page
  tracing, and the tested dispatcher/candidate branches were already rejected
  or closed.
- No current-build execution owner has yet been accepted.

## Approved scope

- Inspect existing 2.0.4 runtime logs and their recorded caller/target/object,
  timestamp, thread and lifecycle fields.
- Correlate only execution that is bounded by validated ENTER/EXIT and can be
  associated with the same camera/context lineage.
- Rank a small number of owner candidates by lifecycle timing and recurrence.
- Use existing current-build static reports only to identify the containing
  function of a runtime-correlated candidate.

## Explicit non-goals

- No new ASI, hook, write test, or game launch in this phase.
- No PAGE_GUARD, VEH, random thread-context sampling, or broad execution trace.
- No direct `+0x230` writer enumeration, FOV-like value scan, legacy offset
  reuse, or setter-local FOV search.
- Do not modify the stable gameplay ASI, global experimental ASI, production
  source, release files, or Ghidra project state.

## Expected files or areas

- Existing 2.0.4 runtime logs under the game Win64 directory and workspace
  evidence copies, if present.
- Existing current-build Ghidra reports under `02-Research/Ghidra/reports`.
- This task plan only; no runtime artifact is expected in this phase.

## Batches and validation

### Batch 1 — runtime evidence inventory

- Locate existing 2.0.4 lifecycle and discovery logs.
- Extract only bounded ENTER-to-EXIT execution records with identity and timing
  metadata.
- Separate valid context-correlated records from dispatcher noise, stale
  register snapshots and unrelated objects.

Validation: every retained record must identify executable location, lifecycle
phase and usable context/object identity, or be marked incomplete.

Result:

- The available Win64 runtime directory contains only
  `STALKER2CinematicFovTransitionCandidateCorrelation204.log` and
  `STALKER2GameplayAspectFix.log`.
- The candidate-correlation log records a valid ENTER/EXIT `inner`, but its
  runtime candidates are the already rejected layout-based functions. It has
  no accepted owner edge, no owner target/caller inventory, and no ENTER/EXIT
  execution neighborhood suitable for promotion.
- The gameplay log records the known transition state machine and FOV values,
  but not the native execution owner of the convergence.
- No new ownership-discovery artifact or runtime log is present.

### Batch 2 — owner correlation

- Compare candidate activity against ENTER convergence, steady cinematic state,
  EXIT recovery and post-EXIT completion.
- Require recurrence or a meaningful transition event, not generic frequency.
- Keep at most three owner neighborhoods for review.

Validation: a candidate must correlate with both transition directions or be
explicitly marked partial/unresolved.

Result:

- No candidate execution owner can be correlated from the available logs with
  both `90.65574 -> 90.0` ENTER convergence and reverse EXIT recovery.
- The existing records do not expose a bounded caller/target/object lineage
  for a different execution path; promoting any function from them would be
  inference from absence or from layout similarity.
- Candidate inventory: empty. No owner neighborhood is retained.

### Batch 3 — bounded decision

- Accept, reject or leave unresolved each retained owner candidate.
- Inspect existing static material only for accepted or strongest unresolved
  runtime-correlated candidates.
- Stop if no candidate satisfies lifecycle ownership correlation.

Validation: no runtime artifact follows without an accepted owner boundary.

Result:

- This phase is closed as `bounded-unresolved: insufficient runtime owner
  evidence`.
- No candidate was accepted, rejected on new runtime grounds, or promoted to
  static semantic inspection.
- No new ASI, hook, write test, or game launch was performed.
- A new runtime artifact would require a separately approved discovery scope;
  it is not justified by the existing logs alone.

## Risks and safe failure

- Existing logs may contain monitor-thread IDs, stale register state or invalid
  pointers; these are not owner evidence.
- High-frequency generic ticks can appear correlated by time alone; require
  context lineage and transition-specific behavior.
- If evidence is insufficient, close this phase as bounded-unresolved without
  widening the trace or changing instrumentation design.

## Stop conditions and phase gates

- Do not reopen closed writer, setter, FOV-field or unsafe instrumentation
  branches.
- Do not inspect more than three bounded runtime owner neighborhoods.
- Do not build or inject a new ASI in this phase.
- If no owner correlates with both ENTER and EXIT, mark the phase unresolved
  and stop.

## Final review

Record the exact logs and executable identity used, candidate verdicts,
untouched ASI/source areas, limitations, Git status and whether a separate
static semantic or runtime-correlation phase is justified.
