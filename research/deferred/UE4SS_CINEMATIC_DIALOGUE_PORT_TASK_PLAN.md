# UE4SS Cinematic and Dialogue Port — Task Plan

## Objective

Restore the cinematic aspect/FOV and dialogue zoom branches in the separate
`STALKER2CameraTweaks` UE4SS module after gameplay-only validation, using the
original policy names and no separate aspect/FOV extension keys.

## Evidence and constraints

- Gameplay two-pass UE4SS behavior has a user-reported runtime PASS.
- Current cinematic and dialogue behavior has not been validated.
- `AspectRatio` is the intended sole cinematic policy setting.
- Complex ref/out reflected calls remain unsupported; no `GetCameraView` or
  `BlueprintUpdateCamera` invocation is allowed.

## Approved scope

- Re-enable bounded cinematic policy handling.
- Re-enable the existing dialogue zoom model.
- Use the available `SetCinematicMode` lifecycle callback when present, with a
  reflected-state fallback.
- Keep gameplay two-pass logic unchanged.
- Keep the module-specific log and current dynamic discovery.

## Non-goals

- No native hooks, raw offsets or guessed pointers.
- No new projection/evaluation claims.
- No changes to the production ASI.
- No runtime claim until the user tests gameplay, dialogue and cinematic.

## Validation and stop conditions

- Confirm no separate `DisplayAspectRatio` or `PreserveAuthoredFOV` contract
  remains.
- Confirm cinematic/dialogue writes are reachable only under their policy
  settings and never under `Native`.
- Package and inspect the test ZIP.
- Stop at static validation; visual/runtime behavior remains pending.
