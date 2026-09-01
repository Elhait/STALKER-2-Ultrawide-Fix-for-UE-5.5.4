# Cinematic FOV `FUN_1405EDA3A` Runtime Correlation 2.0.4 — Task Plan

## Objective

Run one isolated, read-only runtime correlation pass for current-build
`FUN_1405EDA3A` (`0x1405EDA3A`, RVA `0x5EDA3A`) against the validated
cinematic ENTER/EXIT lifecycle. Determine whether its time/progression and
completion-related state correlates with native FOV convergence.

## Established evidence and current state

- Current executable: `Stalker2-Win64-Shipping.exe`, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- The broad static pass found `FUN_1405EDA3A` as a plausible timing/state
  neighbor, but did not prove camera ownership.
- The preceding runtime test rejected `FUN_1422FC35A`; its rejection does not
  determine the role of this candidate.
- Native cinematic behavior is known: ENTER FOV converges `90.65574 → 90.0`
  and EXIT recovers the corrected state to the gameplay baseline.

## Approved scope

- Create one research-only ASI with one ordinary hook at `FUN_1405EDA3A`.
- Correlate candidate object, thread, timestamps, progression/time-like fields,
  state flags and completion-path indicators with the validated lifecycle.
- Keep authoritative `inner` and FOV/aspect/flags as external reference data;
  candidate object need not equal `inner`.
- Produce one isolated runtime artifact and one user-run ENTER/EXIT test.

## Explicit non-goals

- No game-state writes or FOV/aspect correction.
- No PAGE_GUARD, VEH, random sampling, broad hooks, watchpoints or generic
  executable-wide tracing.
- No stable gameplay/global experimental ASI changes.
- No automatic promotion of `FUN_1431FA182` or other reserve candidates.

## Expected files or areas

- New research source and build helper in the experimental source/build areas.
- New runtime log in the game binary directory during the authorized test.
- This plan and a concise evidence report after the run.

## Batches and validation

### Batch 1 — read-only artifact — IN PROGRESS

- Validate the exact 2.0.4 executable identity and candidate prologue.
- Install one ordinary hook at `FUN_1405EDA3A` plus the validated lifecycle
  observation anchors.
- Log candidate object identity, thread/cross-thread marker, QPC delta from
  ENTER, fields `+0x300`, `+0x301`, `+0x3E6`, `+0x4B8`, `+0x4C0`, `+0x4C8`,
  and authoritative FOV/aspect/flags.
- Record completion-related state conservatively; do not infer branch outcome
  when the hook position cannot prove it.

Validation: successful build, unique version-specific validation, load/install
log, and no game-state write path.

### Batch 2 — isolated runtime correlation — COMPLETED

- Run only this ASI with all stable/global/cinematic ASIs disabled.
- Use the same tested cutscene and capture native ENTER convergence and EXIT
  recovery without manual correction.

Validation: classify strong pass, partial, or reject using the candidate gate.

Result: the artifact installed successfully; ENTER and EXIT captured the same
`inner=0x2D701A50100` and thread `7188`. `FUN_1405EDA3A` produced zero hits
during the approximately `11.55 s` lifecycle window.

### Batch 3 — verdict — COMPLETED / REJECTED

- Record the evidence-backed verdict and close this candidate phase.
- Do not broaden the artifact automatically after a negative result.

Result: candidate rejected for the tested cinematic lifecycle. This is a clean
runtime negative, not a setup failure. No write-test was performed. The next
reserve candidate requires a separate task plan.

## Acceptance gate

Strong pass requires a stable candidate object or lineage, meaningful evolution
of progression/time-like fields during `90.65574 → 90.0`, and a completion or
mode change near the FOV endpoint. Re-activation/reset during EXIT is strong
supporting evidence.

Partial is allowed when ENTER timing/progression is convincing but EXIT or
completion semantics remain unproven.

Reject if activity persists as generic steady-state work, fields do not evolve
with FOV timing, objects are unrelated/unstable, or no ENTER/EXIT lifecycle
correlation exists.

## Risks and safe failure

- Candidate ABI/prologue mismatch: refuse installation without process writes.
- Invalid object/field read: skip the record and continue safely.
- Hook or logging failure: fail closed; never modify camera state.
- Build/load success is not ownership evidence.

## Stop conditions and phase gates

- One candidate hook only; no write-test in this phase.
- One clean runtime run is sufficient for the first verdict.
- If rejected, close `FUN_1405EDA3A`; any next candidate requires a separate
  plan and artifact.
- Stable/global ASI and reserve candidates remain untouched.

Current gate: met. `FUN_1405EDA3A` is closed for this lifecycle; no automatic
promotion of `FUN_1431FA182` is performed by this task.

## Final review

Compare changed paths with this plan, inspect build/load/runtime evidence,
record executable identity and test conditions, review Git status/diff, and
separate completed, remaining, deferred, blocked and not-runtime-validated
items.
