# Cinematic FOV `FUN_1422FC35A` Runtime Correlation 2.0.4 — Task Plan

## Objective

Run one isolated, read-only runtime correlation pass for current-build
`FUN_1422FC35A` (`0x1422FC35A`) against the validated cinematic ENTER/EXIT
lifecycle. Determine whether its execution, object lineage, and scalar state
correlate with native FOV convergence.

## Established evidence and current state

- Current executable: `Stalker2-Win64-Shipping.exe`, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- The broad read-only static pass found `7368` structural candidates and
  ranked the top `500`.
- `FUN_1422FC35A` is the strongest unresolved semantic neighbor: it contains
  current/target-like scalar state, interpolation-shaped arithmetic,
  completion/clamp behavior and repeated state updates. Its cinematic
  ownership is not established.
- Manual/current runtime evidence confirms native ENTER convergence
  `90.65574 → 90.0` and EXIT recovery from the corrected cinematic state.

## Approved scope

- Create one research-only ASI that observes only `FUN_1422FC35A`.
- Correlate hits with the validated cinematic ENTER/EXIT window.
- Log candidate object identity/lineage, scalar arguments/state, phase and
  timing; use `inner` only as a correlation reference, not as a mandatory
  candidate-object identity.
- Produce one isolated runtime artifact and one user-run ENTER/EXIT test.

## Explicit non-goals

- No game-state writes or FOV/aspect correction.
- No broad hooks, executable-wide sampling, PAGE_GUARD, VEH, debugger-style
  watchpoints, random register snapshots or generic tick tracing.
- No changes to `STALKER2GameplayAspectFix`, the global experimental ASI, stable
  source, release artifacts or legacy offsets.
- Do not automatically promote secondary static candidates in this task.

## Expected files or areas

- New research source and build helper under the experimental source/build
  areas only.
- New runtime log in the game binary directory during the authorized test.
- This plan; no stable product files expected to change.

## Batches and validation

### Batch 1 — read-only artifact — COMPLETED

- Validate the exact 2.0.4 executable identity.
- Install one ordinary hook only at the current-build candidate function.
- Capture validated ENTER context and close at EXIT.
- Log candidate hits with timestamp, thread, candidate object/arguments,
  scalar values, and phase. Fail closed on invalid pointers or identity.

Validation: build success, unique candidate validation, load/install log, and
no write path in the artifact. Candidate hits are not rejected by thread ID;
the log records `enterThread` and `crossThread` so thread affinity can be
evaluated from evidence rather than assumed.

Result: created and built the isolated `FUN_1422FC35A` correlation ASI. It
validates the known 2.0.4 ENTER/EXIT anchors and candidate prologue, captures
candidate context during the lifecycle window without rejecting other threads,
and performs no game-state writes. Build succeeded with MSVC C++23.

### Batch 2 — isolated runtime correlation — COMPLETED

- Run with only this ASI enabled; all gameplay/global/cinematic ASIs disabled.
- Use the same tested cutscene and capture native ENTER convergence and EXIT
  recovery without manual correction.

Validation: classify PASS/partial/reject using the acceptance gate below.

Result: one isolated run produced `2782` hits across three candidate objects,
all on the ENTER thread. The stable main object and two auxiliary objects did
not expose scalar behavior tracking the authoritative FOV convergence; hits
continued after the FOV reached `90.0`. No cross-thread activity was observed.

### Batch 3 — verdict — COMPLETED / REJECTED

- Record whether the candidate is runtime-promoted, held, or rejected.
- Do not expand the artifact automatically after a negative result.

Result: `FUN_1422FC35A` rejected as a generic state update rather than the
native cinematic FOV owner. No write-test or secondary candidate was started.

## Acceptance gate

PASS/strong correlation requires activity during ENTER convergence, a
repeatable object or lineage, scalar behavior compatible with current/target/
alpha/time semantics, and a phase change at steady state. Re-activation during
EXIT is strong supporting evidence.

PARTIAL is allowed when ENTER correlation is clear but EXIT or semantic
interpretation remains incomplete.

REJECT if the candidate is absent during convergence, is generic high-frequency
noise, has unstable unrelated objects/scalars, or has no ENTER/EXIT lifecycle
correlation.

## Risks and safe failure

- Candidate ABI or signature mismatch: refuse installation without touching
  process state.
- Invalid candidate object or indirect argument: log and skip the hit.
- Hook/reentrancy/logging failure: disable the research hook and fail closed;
  never write camera state.
- A successful load/build is not runtime ownership evidence.

## Stop conditions and phase gates

- No writes or second candidate in Batch 1.
- One clean runtime run is sufficient for the first verdict.
- If rejected, close `FUN_1422FC35A` and return to the static ranking before
  considering a separate secondary-candidate plan.
- Do not modify stable/global ASI or broaden tracing from this artifact.

Current gate: met. This candidate-specific runtime pass is closed with a
negative verdict; any secondary candidate requires a separately scoped plan.

## Final review

Compare changed paths to this plan, inspect build/load/runtime evidence, record
the exact executable identity and test conditions, review Git status/diff, and
separate completed, remaining, deferred, blocked and not-runtime-validated
items.
