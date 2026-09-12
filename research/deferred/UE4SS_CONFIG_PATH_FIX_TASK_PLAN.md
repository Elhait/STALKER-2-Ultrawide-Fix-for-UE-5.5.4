# UE4SS Config Path Fix — Task Plan

## Objective

Make the UE4SS `STALKER2CameraTweaks` prototype read its own INI beside the
module script, eliminating ambiguity caused by UE4SS working-directory-relative
paths.

## Established evidence

- The installed module INI contains `Gameplay.Enabled=true`.
- The fresh UE4SS log reports `Gameplay.Enabled=false`.
- The current Lua code tries relative paths beginning with `Mods/...`, so the
  reported value is not reliably tied to the module's adjacent INI.

## Approved scope

- Change only `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua` config
  path resolution and its config-path log line.
- Refresh the test ZIP.

## Non-goals

- No gameplay algorithm changes.
- No cinematic/dialogue changes.
- No production ASI changes.
- No deployment to the installed game directory.

## Validation and stop conditions

- Confirm the script resolves `../STALKER2CameraTweaks.ini` from its own file
  location and logs the selected path.
- Inspect ZIP contents and Git diff/status.
- Runtime validation remains pending the user's next launch.
- Stop if the script-location API is unavailable; retain a clear blocked log
  rather than silently reading an unrelated config.

## Rollback

Restore the previous `main.lua` or disable the UE4SS module. The stable ASI is
untouched.
