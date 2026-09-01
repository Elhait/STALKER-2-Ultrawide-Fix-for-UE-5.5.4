# Cinematic FOV `FUN_146880C06` Runtime Correlation 2.0.4 — Task Plan

## Objective

Run one isolated, read-only runtime Gate A for `FUN_146880C06` (RVA
`0x6880C06`). Test whether its scalar evaluator inputs or derived raw values
correlate with the validated cinematic FOV convergence and recovery.

## Established evidence and current state

- Current executable: `Stalker2-Win64-Shipping.exe`, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- `FUN_14027A5E4` was runtime-rejected as a persistent unrelated state update.
- Static review describes this candidate as a scalar evaluator with two scalar
  fields, an interpolation input and clamp-like result handling; ownership is
  unproven.

## Approved scope

- One ordinary hook at `FUN_146880C06` plus validated ENTER/EXIT observation
  anchors.
- Log candidate object, scalar fields at `+0x200/+0x204`, interpolation input at
  `+0x108`, derived raw interpolation value, selected XMM/GPR context, thread,
  timing and authoritative inner FOV/aspect/flags as external reference.
- Produce one isolated read-only runtime artifact and one user-run lifecycle
  test.

## Explicit non-goals

- No game-state writes, FOV/aspect correction or post-call bridge.
- No PAGE_GUARD, VEH, random sampling, broad hooks or watchpoints.
- No stable gameplay/global experimental ASI changes.
- No automatic promotion of `FUN_14569...` family or any other candidate.

## Expected files or areas

- New research source and build helper in experimental source/build areas.
- New runtime log in the game binary directory during the authorized test.
- This plan and a concise evidence report after the run.

## Batches and validation

### Batch 1 — read-only artifact — COMPLETED

- Validate the exact 2.0.4 executable identity and candidate prologue.
- Install one ordinary candidate hook and validated lifecycle anchors.
- Log scalar evaluator inputs and a derived raw interpolation value only; do
  not infer the function return from volatile post-call registers.

Validation: successful build, version-specific byte validation and an explicit
read-only/no-writes implementation review.

Evidence: current executable prologue at RVA `0x6880C06` matched
`56 57 53 48 83 EC 40 0F 29 74`; the artifact built successfully and contains
only lifecycle observation and scalar reads/derivation.

### Batch 2 — isolated runtime correlation — COMPLETED

- Run only this ASI with all other ASIs disabled.
- Use the same tested cutscene without manual correction.

Validation: classify strong pass, partial or reject using scalar-flow and
lifecycle correlation.

Evidence: the probe installed and observed a same-thread ENTER→EXIT lifecycle
of approximately `12.470 s` on the same authoritative `inner`, but produced
zero candidate hits.

### Batch 3 — verdict — COMPLETED

- Record the evidence-backed verdict and close this candidate phase.
- Do not expand the artifact automatically after a negative result.

Verdict: `FUN_146880C06` rejected for the tested cinematic lifecycle;
write-test not justified. Full evidence is recorded in
`02-Research/Ghidra/reports/cinematic-fov146880c06-runtime-correlation-204-review.md`.

## Acceptance gate

Strong pass requires a stable candidate object or lineage and scalar inputs or
derived values that track the ENTER curve `90.65574 → 90.0`, change or stop at
steady state, and ideally reappear on EXIT recovery.

Partial is allowed when ENTER correlation is convincing but EXIT or return-value
semantics remain unproven.

Reject if the function is silent, produces generic unrelated curves, remains
unchanged through the lifecycle, or has no meaningful alignment with the
authoritative FOV reference.

## Risks and safe failure

- Prologue/identity mismatch: refuse installation without game-state writes.
- Invalid object/field read: skip the record safely.
- Hook/logging failure: fail closed; never modify camera state.
- Build/load success is not ownership evidence.

## Stop conditions and phase gates

- One candidate hook only; no write-test in this phase.
- One clean runtime run is sufficient for the first verdict.
- If rejected, close `FUN_146880C06`; any next candidate requires a separate
  plan and artifact.
- Stable/global ASI and held candidates remain untouched.

## Final review

Compare changed paths with this plan, inspect build/load/runtime evidence,
record executable identity and test conditions, review Git status/diff, and
separate completed, remaining, deferred, blocked and not-runtime-validated
items.
