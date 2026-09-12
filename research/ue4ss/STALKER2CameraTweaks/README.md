# STALKER2CameraTweaks — UE4SS parallel prototype

This folder is a separate UE4SS implementation with the same module name as
the production ASI. The current build is intentionally limited to the
gameplay path so its native-like transition can be tested in isolation.

## Important status

This is a test prototype, not a replacement release. It does not use hardcoded
object IDs, raw native offsets, native hooks or undocumented ref/out calls.
It also does not claim that a reflected property write rebuilds the complete
UE camera/projection pipeline. The UE4SS log explicitly records accepted writes
and their route for later in-game comparison.

## Installation

Copy this folder as:

`Stalker2\Binaries\Win64\ue4ss\Mods\STALKER2CameraTweaks`

The required module entry is:

`STALKER2CameraTweaks : 1`

Keep the included `STALKER2CameraTweaks.ini` beside `Scripts\main.lua` in the
module folder.

## Test isolation

For a UE4SS test, disable the production `STALKER2CameraTweaks.asi` and any
older camera-writing UE4SS module. Keep UE4SS enabled. Do not run this module
and the ASI together because both can write the same camera state.

`End` forces one immediate state record. It does not change camera values.

The module also creates its own log beside the INI:

`Mods\STALKER2CameraTweaks\STALKER2CameraTweaks.log`

The first line reports `LOG_PATH`. The module still mirrors messages to the
global `UE4SS.log`, but the module log is the preferred file for evidence.

## Current prototype

- Dynamic discovery of the player `CameraComponent`, `CameraManager` bridge and
  `PlayerCameraManager`.
- Re-discovery after object invalidation/replacement.
- Gameplay wide-state transition following the confirmed order:
  unconstrained → constrained → native aspect → unconstrained, with FOV
  preserved. Each phase is bounded and logged.
- Change-only state logging for camera and PCM current/last/target POV.
- Cinematic lifecycle detection through the shared `SetCinematicMode` hook when
  available, with a reflected-hook fallback.
- Cinematic `AspectRatio` policies (`Native`, `Auto`, `16:9`, `21:9`, `32:9`)
  and the established Full Hor+ FOV policy for non-`Native` modes.
- Dialogue zoom policy detection and FOV application outside active cinematic
  mode (`Native`, `Adaptive`, `Reduced`, `Disabled`).
- Cinematic writes are bounded to once per cinematic entry so the module does
  not fight the game's own camera interpolation; later authored FOV changes
  are transformed once per new input value.

These cinematic and dialogue branches are a best-effort UE4SS port and have
not yet been confirmed in-game. They intentionally use only simple reflected
properties and value-only setters; they do not attempt `GetCameraView`,
`BlueprintUpdateCamera`, ref/out calls, native hooks or raw offsets.

## Not yet proven

- Native cinematic projection reevaluation after a property write.
- Exact causal ownership of letterbox creation/removal.
- Safe invocation of `GetCameraView` or `BlueprintUpdateCamera` with ref/out
  structures.
- Production equivalence across game builds or all cinematic types.

The production ASI remains the stable fallback until this prototype has
repeatable runtime evidence. Gameplay has been tested separately; cinematic
and dialogue behavior remain unvalidated until a new in-game test confirms
both the visual result and the module log lifecycle.
