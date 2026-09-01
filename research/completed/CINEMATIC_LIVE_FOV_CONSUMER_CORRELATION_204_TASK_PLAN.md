# Cinematic live-FOV consumer correlation — 2.0.4

Status: Complete. The current-build live-FOV consumer boundary was confirmed
for both ENTER and EXIT source patterns; write feasibility was handled by a
separate plan.

## Objective

Create one isolated, read-only research ASI that records XMM0 immediately
before the two current-build calls to `FUN_146B6DAAC` inside
`FUN_142EE68DA`, correlated with the already validated cinematic ENTER/EXIT
lifecycle. Determine whether this current descendant carries the live FOV
curve without modifying game state.

## Established evidence and current state

- Executable: Steam 2.0.4, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Static mapping found `FUN_142EE68DA` as a very strong descendant of the
  legacy transition hub, including shared helper, resolver pair and exact
  ENTER/EXIT vtable-slot pairs.
- Current ENTER callsites are `0x2EE6936` and `0x2EE69A7`; both load XMM0
  immediately before calling `FUN_146B6DAAC`.
- Current EXIT lifecycle anchor is RVA `0x6B6C482`; validated ENTER setter
  anchor is RVA `0x6B7CB05`.
- Native cinematic FOV behavior is already known behaviorally, but current
  XMM0 runtime semantics at this mapped boundary are unproven.

## Approved scope

- Add one separate read-only C++ research artifact and its build command.
- Install ordinary SafetyHookMid hooks only at the two exact scalar-consumer
  callsites and the existing validated ENTER/EXIT lifecycle anchors.
- Log phase, XMM0, source-side sanity values, thread, timestamp and
  authoritative inner FOV/aspect/flags when readable.
- Build the artifact and verify resolver/byte validation and output existence.

## Explicit non-goals

- No camera/FOV/aspect writes.
- No post-call bridge, return-address reconstruction or manual assembly.
- No PAGE_GUARD, VEH, polling, random sampling or broad hooks.
- Do not change stable gameplay ASI, global experimental ASI or existing
  cinematic artifacts.
- No in-game/runtime claim until the user supplies a clean isolated run log.

## Expected files/areas

- `src/cinematic_live_fov_consumer_correlation_204.cpp`
- `build-artifacts/test-scripts/build-cinematic-live-fov-consumer-correlation-204.cmd`
- `build-artifacts/obj/cinematic_live_fov_consumer_correlation_204.obj`
- `build-artifacts/STALKER2CinematicLiveFovConsumerCorrelation204.asi`

## Batches and validation

### Batch 1 — plan and source — COMPLETED

Implemented fail-closed byte validation, lifecycle correlation and read-only
callsite logging. Source was checked against the two decoded CALL instructions.

### Batch 2 — build — COMPLETED

Built only this research artifact successfully. The expected ASI exists at
`build-artifacts/test-asi/STALKER2CinematicLiveFovConsumerCorrelation204.asi`.
Build success is not runtime proof.

### Batch 3 — review/handoff — COMPLETED

Source review found no game-state writes; hooks are limited to the two mapped
consumer callsites plus validated lifecycle anchors. Read-only Git review was
performed. User runtime test remains separate and must use only this ASI.

## Risks and safe failure

- Exact callsite bytes may differ; refuse installation and log the mismatch.
- XMM0 may not be preserved by the hook framework at the selected boundary;
  record observed values without inferring semantics.
- Lifecycle identity may be unavailable at the callsite; log the hit as
  uncorrelated rather than guessing object provenance.
- On any hook creation failure, reset all created hooks and leave game state
  untouched.

## Stop conditions and phase gates

- Stop if either callsite fails exact byte validation.
- Stop after one isolated read-only artifact and one user-run verdict.
- Do not add writes or promote a production hook from this plan.
- Runtime promotion requires the scalar sequence to correlate with ENTER and
  EXIT FOV curves; otherwise reject this boundary.

## Final review requirements

- Record executable hash, exact RVAs/bytes and build result.
- Confirm only research files in this plan changed.
- Report completed, remaining, deferred, blocked and not-runtime-validated
  outcomes separately.
