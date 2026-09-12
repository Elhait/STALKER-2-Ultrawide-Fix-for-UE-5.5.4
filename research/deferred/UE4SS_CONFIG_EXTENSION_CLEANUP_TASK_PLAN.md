# UE4SS Config Extension Cleanup — Task Plan

## Objective

Remove the separate `DisplayAspectRatio` and `PreserveAuthoredFOV` extension
contract from the UE4SS `STALKER2CameraTweaks` prototype, including the Lua
parser and logging code that handled those keys.

## Scope

- Update only the UE4SS module Lua/config and refreshed test package.
- Keep `Cinematics.AspectRatio` as the sole cinematic configuration selector.
- Preserve the gameplay two-pass implementation unchanged.

## Non-goals

- No gameplay algorithm changes.
- No cinematic/dialogue activation.
- No production ASI changes.
- No runtime deployment or validation.

## Validation / stop conditions

- Confirm no Lua reference remains to either removed configuration key.
- Confirm the INI contains neither key.
- Inspect package contents and `git diff --check`.
- Stop after static cleanup; runtime behavior remains unvalidated.
