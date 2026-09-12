# GETCAMERAVIEW_CALLABILITY_PROBE

## Objective

Determine whether the current UE4SS Lua surface can safely invoke the live
`CameraComponent.GetCameraView` function and inspect its `FMinimalViewInfo`
output, without changing production camera behavior.

## Established evidence and current state

- A live player `CameraComponent` is discoverable through the confirmed
  `Stalker2.CameraManager -> CameraComponent` reference chain.
- Current-build UE4SS metadata exposes:
  `CameraComponent.GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)`.
- Existing observer modules are read-only and do not provide a validated
  invocation path for this function.
- The production `STALKER2CameraTweaks` ASI must remain untouched.

## Approved scope

- Add or adapt one isolated UE4SS research probe for
  `CameraComponent.GetCameraView` only.
- Confirm invocation capability, output readability and immediate state
  invariance.
- Compare returned `DesiredView` with the active Camera/PCM reflected POV when
  the comparison can be made without guessed layouts or pointers.

## Explicit non-goals

- No runtime invocation of `BlueprintUpdateCamera` or other candidate
  functions. A later read-only metadata/callability audit of that secondary
  candidate was performed outside this primary probe scope; it introduced no
  invocation or behavior change.
- No property writes, suppression, slot calls or PCM mutation.
- No production ASI changes or release packaging.
- No broad function inventory, renderer scan or new static reverse engineering.
- No claim that a successful call is an active pipeline refresh.

## Expected files or areas

- `research/ue4ss/GETCAMERAVIEW_CALLABILITY_PROBE/`
- `build-artifacts/test-ue4ss/` only if a test archive is explicitly produced.
- Existing observer and stable source remain untouched.

## Implementation batches

### Batch 1 — UE4SS surface inspection

Inspect existing Lua helpers and current UE4SS documentation/metadata for a
deterministic function invocation and `FMinimalViewInfo` construction/readback
path. If no safe surface exists, stop without adding a runtime probe.

### Batch 2 — isolated probe (only if Batch 1 passes)

Implement a minimal read-only probe targeting the already-known live
`CameraComponent`. Invoke `GetCameraView` at most once per explicit manual
trigger, capture success/failure and the returned view fields that are safely
readable. Record pre/post Camera/PCM values for immediate mutation checks.

### Batch 3 — scoped validation

Validate load, target acquisition, invocation result and pre/post state. Treat
game launch or module load as insufficient evidence for invocation or refresh
semantics.

## Validation

- Static: inspect the probe diff and confirm only the approved function is
  referenced.
- Load: confirm UE4SS reports the isolated probe loaded.
- Runtime, if safe invocation exists: one controlled call in gameplay and one
  in the tested cinematic/dialogue state; record output and pre/post state.
- Classify separately: invocation, output, side effects and active-pipeline
  refresh.

## Risks and rollback / safe failure

- Incorrect UE4SS struct construction or invocation can crash the game.
- If argument construction, return handling or object validity is uncertain,
  the probe must refuse to call and log `CALLABILITY_BLOCKED`.
- Disable/remove only the isolated research module to roll back; do not modify
  the production ASI or existing stable configuration.

## Stop conditions and phase gates

- Stop before runtime if UE4SS cannot deterministically construct the required
  arguments or expose a safe call API.
- Gate A: invocation completes safely.
- Gate B: a valid/readable `FMinimalViewInfo` is obtained.
- Gate C: Camera/PCM state is unchanged immediately after the call.
- Gate D: returned view is compared with active effective POV.
- A passing invocation is diagnostic evidence only; active refresh remains
  unproven unless PCM/render state demonstrably changes through a permitted
  native path (which this task does not attempt).

## Expected final Git review

Review status, diff and changed paths against this plan. Report completed,
remaining, deferred, blocked and not-runtime-validated items. Do not leave the
plan in the repository root after the task is complete; move it to the
appropriate research archive.
