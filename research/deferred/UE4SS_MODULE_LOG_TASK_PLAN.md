# UE4SS Module Log — Task Plan

## Objective

Give the UE4SS `STALKER2CameraTweaks` module its own log file so runtime
evidence is separated from the global `UE4SS.log`.

## Scope

- Write module messages to `Mods/STALKER2CameraTweaks/STALKER2CameraTweaks.log`.
- Keep console output unchanged.
- Keep gameplay implementation and all cinematic/dialogue behavior unchanged.
- Document that cinematic and dialogue remain unvalidated.

## Non-goals

- No new camera writes or lifecycle changes.
- No production ASI changes.
- No deployment to the game directory.

## Validation / stop conditions

- Confirm log path is derived from `Scripts/main.lua`, not the UE4SS working
  directory.
- Confirm the module log receives the same emitted lines.
- Inspect `git diff --check` and affected paths.
- Runtime confirmation of log creation remains pending the user's next launch.
