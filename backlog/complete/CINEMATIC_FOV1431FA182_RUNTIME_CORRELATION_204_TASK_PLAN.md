# Cinematic FOV `FUN_1431FA182` Runtime Correlation 2.0.4 — Task Plan

## Objective

Run one isolated, read-only runtime correlation pass for current-build
`FUN_1431FA182` (`0x1431FA182`, RVA `0x31FA182`) as a setup/smoothing
candidate. Determine whether its rare calls, scalar arguments and state stores
are phase-linked to cinematic ENTER/EXIT.

## Established evidence and current state

- Current executable: `Stalker2-Win64-Shipping.exe`, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Broad static ranking identified `FUN_1431FA182` as a compact function with
  explicit scalar parameters, state initialization, smoothing-like setup and
  downstream calls.
- `FUN_1422FC35A` was runtime-rejected as the FOV owner; `FUN_1405EDA3A` was
  silent during the validated lifecycle.
- Native cinematic behavior remains known from prior evidence: ENTER FOV
  converges `90.65574 → 90.0`; EXIT recovers the modified state.

## Approved scope

- Create one research-only ASI with one ordinary hook at `FUN_1431FA182` plus
  the already validated ENTER/EXIT observation anchors.
- Log candidate object, scalar arguments, PRE state stores and lifecycle phase.
- Accept rare setup hits as meaningful only when their timing and values show
  target/smoothing/duration semantics.
- Produce one isolated runtime artifact and one user-run ENTER/EXIT test.

## Explicit non-goals

- No game-state writes, FOV/aspect correction or POST-call bridge.
- No PAGE_GUARD, VEH, random sampling, broad hooks, watchpoints or generic
  executable-wide tracing.
- No stable gameplay/global experimental ASI changes.
- Do not automatically promote `FUN_1404A4CCE` or rerank the full reserve in
  this task.

## Expected files or areas

- New research source and build helper in the experimental source/build areas.
- New runtime log in the game binary directory during the authorized test.
- This plan and a concise evidence report after the run.

## Batches and validation

### Batch 1 — read-only artifact — IN PROGRESS

- Validate the exact 2.0.4 executable identity and candidate prologue.
- Install one ordinary candidate hook and the validated lifecycle observation
  anchors.
- Log PRE candidate state, scalar arguments, object identity, thread/
  cross-thread marker, QPC delta from ENTER and authoritative FOV/aspect/flags.
- Do not infer POST state or branch outcomes from volatile post-call registers.

Validation: successful build, unique version-specific validation, load/install
log and no game-state write path.

### Batch 2 — isolated runtime correlation — COMPLETED

- Run only this ASI with all other stable/global/cinematic ASIs disabled.
- Use the same tested cutscene and capture native ENTER convergence and EXIT
  recovery without manual correction.

Validation: classify strong pass, partial or reject using the setup-oriented
acceptance gate below.

Result: the artifact installed successfully; ENTER and EXIT captured the same
`inner=0x27B7E02A6C0` and thread `19768`. `FUN_1431FA182` produced zero hits
during the approximately `12.4 s` lifecycle window.

### Batch 3 — verdict — COMPLETED / REJECTED

- Record the evidence-backed verdict and close this candidate phase.
- Do not expand the artifact automatically after a negative result.

Result: candidate rejected for the tested cinematic lifecycle. No write-test was
performed. The initial three-candidate runtime shortlist is exhausted; further
work requires runtime-informed reranking and a separate plan.

## Acceptance gate

Strong pass requires a stable candidate object/context, a rare hit near ENTER
or EXIT, scalar arguments/stores compatible with target/smoothing/duration
setup, silence during steady state, and a meaningful repeat/reset pattern.

Partial is allowed when ENTER setup timing and scalar semantics are convincing
but EXIT or object lineage remains unproven.

Reject if there are no hits, only unrelated setup activity, no lifecycle-linked
scalar behavior, or unstable object/context without an explainable lineage.

## Risks and safe failure

- Candidate ABI/prologue mismatch: refuse installation without process writes.
- Invalid object/argument read: skip the record safely.
- Hook or logging failure: fail closed; never modify camera state.
- Build/load success is not ownership evidence.

## Stop conditions and phase gates

- One candidate hook only; no write-test in this phase.
- One clean runtime run is sufficient for the first verdict.
- If rejected, close `FUN_1431FA182`; any reranking or next candidate requires
  a separate plan.
- Stable/global ASI and reserve candidates remain untouched.

Current gate: met. `FUN_1431FA182` is closed; no automatic promotion of
`FUN_1404A4CCE` is performed by this task.

## Final review

Compare changed paths with this plan, inspect build/load/runtime evidence,
record executable identity and test conditions, review Git status/diff, and
separate completed, remaining, deferred, blocked and not-runtime-validated
items.
