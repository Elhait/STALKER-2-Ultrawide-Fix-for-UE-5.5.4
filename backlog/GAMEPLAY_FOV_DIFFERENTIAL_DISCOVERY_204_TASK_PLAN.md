# Gameplay FOV Differential Discovery 2.0.4 — Task Plan

## Objective

Identify the current 2.0.4 runtime data-flow for the player-configured FOV by observing controlled settings changes `90 → 77 → 79 → 90` at the validated gameplay camera-writer boundary.

## Established evidence and current state

- Executable: S.T.A.L.K.E.R. 2 version 2.0.4.
- Known executable SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Gameplay camera-writer signature was runtime-validated at RVA `0xAF41A1`.
- The writer receives the primary FOV in `XMM0` and uses the source/output camera objects already observed by the stable gameplay tracer.
- No current-build cinematic FOV target has been established; the earlier `+0x230` write is not promoted.

## Approved scope

- Create one read-only diagnostic ASI for the validated 2.0.4 gameplay camera-writer FOV store boundary.
- Log controlled FOV changes, source/output identity, selected fields, caller RVA and XMM lanes.
- Use the resulting sequence to select current-build FOV data-flow candidates for a later cinematic comparison.

## Explicit non-goals

- No writes, FOV modification, aspect modification or flag modification.
- No cinematic hooks, legacy RVA reuse, `state+0x54`, `+0x230` or broad float scanning.
- Do not enable or modify `STALKER2GameplayAspectFix.asi` during the isolated discovery run.
- No production source, stable release artifact or gameplay behavior changes.

## Expected files or areas

- `src/gameplay_fov_differential_discovery_204.cpp`
- `build-artifacts/test-scripts/build-gameplay-fov-differential-discovery-204.cmd`
- `build-artifacts/test-asi/STALKER2GameplayFovDifferentialDiscovery204.asi`
- Runtime log in the game binary directory.

## Batches and validation

### Batch 1 — Read-only tracer

- Resolve exactly one complete gameplay camera-writer signature.
- Hook only the validated FOV store instruction at signature offset `+25`.
- Log only materially changed FOV values, with source/output pointers, aspect/flags, caller RVA and XMM0–XMM3.
- Cap records and fail closed on ambiguity or validation failure.
- Build validation proves compilation and hook setup logic only.

Batch 1 implementation result: created `src/gameplay_fov_differential_discovery_204.cpp` and `build-artifacts/test-scripts/build-gameplay-fov-differential-discovery-204.cmd`. The tracer resolves the complete current gameplay-writer signature, hooks only its validated `MOVSS [RBX+0x30], XMM0` FOV store, logs only materially changed FOV values and related source/output state, and performs no writes or game calls. An initial resolver type mismatch was corrected to use the repository's text-pattern API; the final build succeeded and produced `build-artifacts/test-asi/STALKER2GameplayFovDifferentialDiscovery204.asi`.

Initial controlled runtime result: the tracer installed and captured one writer invocation with `XMM0=120`, but no subsequent records for the requested `90 → 77 → 79 → 90` settings changes. This validates the hook and shows that the current gameplay writer is reached during camera initialization/transition, but the live Settings changes did not traverse this boundary during the test. The writer is therefore not yet promoted as the Settings FOV source/consumer; no causal candidate or write-test is authorized.

### Batch 2 — Controlled settings capture

- Enable only the new differential tracer.
- Disable stable gameplay ASI and all cinematic/diagnostic ASI.
- Change the in-game FOV setting in one session: `90 → 77 → 79 → 90`, pausing briefly after each change.
- Promote only a reproducible sequence tied to the same writer/object path.

### Batch 3 — Candidate decision

- Compare the controlled sequence for register, store and object identity.
- Do not perform a write-test in this task; a candidate requires a separately approved causal experiment.

## Risks and safe-failure behavior

- Signature ambiguity, decode mismatch or unreadable state causes no hook installation.
- The tracer only reads memory and registers; it does not call game-owned functions or modify state.
- Repeated writer traffic is bounded by a fixed record limit and FOV-change epsilon.

## Stop conditions and phase gates

- Stop after the controlled `90 → 77 → 79 → 90` sequence is captured or clearly unavailable.
- Stop if the sequence cannot be tied to one current-build writer/object path; do not broaden to global memory scanning.
- No causal write-test or cinematic integration until a separate plan is approved.

## Final review

- Classify the observed path as confirmed, plausible or unresolved.
- Review changed paths against this plan and perform read-only Git status/diff review.
- Keep stable gameplay and cinematic research artifacts untouched.
