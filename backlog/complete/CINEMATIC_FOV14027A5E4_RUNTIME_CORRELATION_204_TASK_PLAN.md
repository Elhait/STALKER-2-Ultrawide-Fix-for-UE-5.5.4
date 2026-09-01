# Cinematic FOV `FUN_14027A5E4` Runtime Correlation 2.0.4 — Task Plan

## Objective

Run one isolated, read-only runtime Gate A for `FUN_14027A5E4` (RVA
`0x27A5E4`). Test its lifecycle/progression hypothesis against validated
cinematic ENTER/EXIT without modifying game state.

## Established evidence and current state

- Current executable: `Stalker2-Win64-Shipping.exe`, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Static review found a compact function with an explicit scalar parameter,
  time-like fields, threshold/accumulator checks and follow-up update calls.
- `FUN_1422FC35A`, `FUN_1405EDA3A` and `FUN_1431FA182` were rejected by
  previous runtime gates; this candidate is selected by semantic plausibility,
  not old static rank.

## Approved scope

- One ordinary hook at `FUN_14027A5E4` plus validated ENTER/EXIT observation
  anchors.
- Log candidate object, scalar argument, pre-update progression fields,
  threshold inputs, mode bytes, thread and lifecycle timing.
- Use authoritative `inner` FOV/aspect/flags only as an external reference.
- Produce one isolated runtime artifact and one user-run ENTER/EXIT test.

## Explicit non-goals

- No game-state writes, FOV/aspect correction or post-call bridge.
- No PAGE_GUARD, VEH, random sampling, broad hooks or watchpoints.
- No stable gameplay/global experimental ASI changes.
- No automatic promotion of `FUN_146880C06` or the `FUN_14569...` family.

## Expected files or areas

- New research source and build helper in experimental source/build areas.
- New runtime log in the game binary directory during the authorized test.
- This plan and a concise evidence report after the run.

## Batches and validation

### Batch 1 — read-only artifact — COMPLETED

- Validate the exact 2.0.4 executable identity and candidate prologue.
- Install one ordinary candidate hook and validated lifecycle anchors.
- Log PRE fields `+0x43C`, `+0x440`, `+0x460`, `+0x464`, `+0x46C`, `+0x470`,
  `+0x6A3`, `+0x6A4`, `+0x6F8`, plus `XMM1` and object pointer.
- Do not infer post-update state from volatile registers.

Validation: successful build, unique version-specific validation, load/install
log and no game-state write path.

Evidence: current executable prologue at RVA `0x27A5E4` matched
`41 56 56 57 53 48 81 EC 98 00`; the artifact built successfully and contains
only read-only candidate/lifecycle observation logic.

### Batch 2 — isolated runtime correlation — COMPLETED

- Run only this ASI with all other ASIs disabled.
- Use the same tested cutscene and capture native ENTER convergence and EXIT
  recovery without manual correction.

Validation: classify strong pass, partial or reject using the progression gate.

Evidence: the isolated run produced 588 same-thread hits on one stable
candidate object, but the progression/completion fields stayed constant and
the function continued for the full ~12.4-second lifecycle after authoritative
FOV had stabilized at `90.0`.

### Batch 3 — verdict — COMPLETED

- Record the evidence-backed verdict and close this candidate phase.
- Do not expand the artifact automatically after a negative result.

Verdict: `FUN_14027A5E4` rejected as the authoritative cinematic FOV owner;
write-test not justified. Full evidence is recorded in
`02-Research/Ghidra/reports/cinematic-fov14027a5e4-runtime-correlation-204-review.md`.

## Acceptance gate

Strong pass requires stable object/context, meaningful progression or threshold
state evolution during `90.65574 → 90.0`, a completion/mode change near the
endpoint, and ideally reset/reactivation during EXIT.

Partial is allowed when ENTER progression is convincing but EXIT or completion
semantics remain unproven.

Reject if the candidate is silent, unrelated, generic steady-state activity,
or its progression fields do not align with the lifecycle.

## Risks and safe failure

- Prologue/identity mismatch: refuse installation without process writes.
- Invalid object/field read: skip the record safely.
- Hook/logging failure: fail closed; never modify camera state.
- Build/load success is not ownership evidence.

## Stop conditions and phase gates

- One candidate hook only; no write-test in this phase.
- One clean runtime run is sufficient for the first verdict.
- If rejected, close `FUN_14027A5E4`; the next candidate requires a separate
  plan and artifact.
- Stable/global ASI and held candidates remain untouched.

## Final review

Compare changed paths with this plan, inspect build/load/runtime evidence,
record executable identity and test conditions, review Git status/diff, and
separate completed, remaining, deferred, blocked and not-runtime-validated
items.
