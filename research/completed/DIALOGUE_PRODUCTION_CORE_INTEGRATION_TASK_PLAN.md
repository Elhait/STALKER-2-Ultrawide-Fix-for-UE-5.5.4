# Dialogue Production Core Integration — Task Plan

## Objective

Integrate the validated dialogue sample-transform feasibility mechanism into
the unified production ASI as a local, fail-closed dialogue subsystem.

## Established evidence and current state

- Canonical Steam 2.0.4 runtime identity is validated by the existing unified
  ASI resolver contracts.
- Current 2.0.4 dialogue boundary is the unique WIDEBOY-derived call boundary
  at RVA `0xD20F77`, with `XMM1` carrying live native dialogue FOV samples.
- Feasibility testing passed `Reduced` and `Disabled` using the first boundary
  sample as baseline `G`, preserving native blend timing and recovery.
- ADS-only testing produced zero dialogue-boundary hits in the tested scenario.

## Approved scope

- Add `[Dialogue] Zoom=Native|Reduced|Disabled` parsing to the unified config.
- Add a unique, instruction-validated dialogue-boundary resolver and hook.
- Keep dialogue lifecycle state, baseline `G` and transform policy local to the
  dialogue subsystem.
- Ignore and reset all dialogue transient state while the existing coordinator
  reports `CinematicActive` or `CinematicExiting`; pass `XMM1` unchanged there.
- Preserve native pass-through for `Native` and all invalid/failure cases.
- Build and inspect the unified ASI; perform no in-game validation in this
  implementation batch.

## Explicit non-goals

- No hotkeys or config persistence.
- No gameplay/cinematic coordinator changes.
- No camera, GameData or timer writes.
- No fallback RVA, broad scans or new reverse-engineering.

## Expected files/areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `build-artifacts/test-scripts/build-experimental-cinematic-21x9-combined-fix-204.cmd`
- `build-artifacts/test-asi/STALKER2UltrawideFix.ini`

## Batches

1. Add dialogue config and local runtime state.
2. Add identity-gated resolver, hook and safe-failure cleanup.
3. Build unified ASI and review the bounded diff.

## Validation

- Compile with the existing unified ASI build script.
- Run `git diff --check` and inspect changed paths.
- Confirm no Java/Ghidra process or canonical project lock remains.
- Runtime matrix is deferred to the approved production integration gate:
  Native, Reduced, Disabled, ADS-only and gameplay/cinematic regression.

## Risks and safe failure

- Any signature ambiguity, instruction mismatch, invalid sample or invalid
  baseline disables dialogue transformation and preserves native behavior.
- Partial hook setup is rolled back before reporting failure.
- Existing gameplay/cinematic hooks remain untouched.

## Stop conditions

- Stop implementation on any resolver or contract mismatch.
- Do not add hotkeys/persistence or alter coordinator ownership in this task.
- Stop after build and bounded Git review; runtime claims require the separate
  in-game matrix.

## Expected final Git review

Compare the actual changed paths with this plan, report build status and limits,
and leave production runtime validation explicitly pending.
