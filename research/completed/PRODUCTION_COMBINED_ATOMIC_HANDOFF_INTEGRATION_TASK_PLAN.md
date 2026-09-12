# PRODUCTION_COMBINED_ATOMIC_HANDOFF_INTEGRATION

## Objective

Integrate the runtime-validated Combined Atomic Cinematic/Gameplay Handoff
behavior into the production `STALKER2CameraTweaks.asi` build.

## Established evidence and current state

- The separate Combined Atomic candidate was runtime-validated by the user.
- Normal gameplay uses one atomic aspect apply: `1.777778`, flags `0x4`.
- Final cinematic EXIT uses the first confirmed descending native FOV sample
  as `RecoveryStart`, applies the same atomic gameplay aspect state once and
  leaves native FOV recovery untouched.
- The current production source still contains the legacy staged `0x5`
  gameplay path behind compile-time test guards.
- Native FOV recovery and the physical setter at RVA `0x205FCC8` are research
  scope and must not be changed.

## Approved scope

- Modify the established production source and production build script only as
  needed to make the validated atomic paths the default production behavior.
- Remove the staged `0x5` transition from the active gameplay path.
- Make the RecoveryStart handoff active without deferred timers or completion
  waits.
- Produce `STALKER2CameraTweaks.asi` with the existing resolver, identity and
  fail-safe behavior preserved.

## Explicit non-goals

- No writes to `CameraComponent +0x230` or the physical FOV setter.
- No cinematic FOV formula changes, dialogue changes or user-FOV changes.
- No Pass1–Pass4 instrumentation, debugger code or native FOV bypass code in
  the production artifact.
- No release packaging, upload, version metadata or publication.

## Files or areas expected to be touched

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `build.cmd`
- This task plan
- Production build output `STALKER2CameraTweaks.asi`

## Implementation batches

### Batch 1 — production source integration

Replace the active staged gameplay transition with the shared atomic mutation
and enable the validated first-downward-sample RecoveryStart handoff. Preserve
the existing gates, source validation, cancellation behavior and native
pass-through.

### Batch 2 — build and static artifact review

Build the production ASI using the existing MSVC environment. Verify the
artifact exists and inspect strings/symbol-independent source conditions to
ensure research instrumentation and native FOV writes are absent from the
production build path.

### Batch 3 — bounded runtime validation

Use the user-controlled game run to verify ordinary gameplay, one cinematic
EXIT and dialogue coexistence. Confirm one atomic apply, no legacy `0x5` pass,
native FOV preservation and no new FOV writes.

## Risks and rollback / safe-failure behavior

- The source has extensive historical research conditionals; only the active
  production path may change in this task.
- Any resolver or instruction validation failure must retain existing safe
  initialization failure behavior.
- If build or runtime behavior fails, restore the previous production source
  and artifact from the user-retained copy; do not delete unrelated research
  outputs.

## Stop conditions and phase gates

- Stop before build if the change requires altering cinematic FOV, dialogue or
  native CameraComponent state.
- Stop promotion if the source diff introduces timer/deferred replay logic,
  FOV writes or research-only hooks.
- Runtime validation is required before calling the production integration
  complete.

## Expected final Git review

Inspect status, diff/statistics and changed paths against this plan. Separate
completed, remaining, deferred, blocked and not-runtime-validated items. Do
not commit or publish.
