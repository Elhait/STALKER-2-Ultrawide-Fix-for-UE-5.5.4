# CameraEvaluationAutoDump

Read-only UE4SS Lua diagnostic mod for the STALKER 2 camera evaluation study.

## Install

Copy the folder `CameraEvaluationAutoDump` into:

`E:\Steam\steamapps\common\S.T.A.L.K.E.R. 2 Heart of Chornobyl\Stalker2\Binaries\Win64\ue4ss\Mods`

The required structure is:

`Mods\CameraEvaluationAutoDump\Scripts\main.lua`

Also add this line to `ue4ss\Mods\mods.txt`:

`CameraEvaluationAutoDump : 1`

## What it does

- Finds the current player `CameraComponent`, `Stalker2.CameraManager` and
  `PlayerCameraManager` without hard-coded numeric IDs.
- Samples every 500 ms.
- Logs a snapshot only when tracked camera/POV values change.
- `End` forces a snapshot even when values are unchanged. F5/F6 are not used
  because they are reserved by the game for save/load; Insert and Home are
  reserved by OptiScaler and ReShade in this setup.
- Includes CameraComponent values, CameraManager references and PCM current,
  last-frame and ViewTarget POV values.

Output is written to the UE4SS console/log. It does not create individual JSON
object dumps and does not modify any property. The previously observed crash
was associated with later interaction with a full `BP_Stalker2Character_C`
dump, not established as a failure of the dump operation itself.

## Test sequence

1. Start with the gameplay fix enabled.
2. Load into gameplay and press `End` once for the A marker.
3. Change the aspect setting through the normal game graphics menu.
4. Press `End` after the first visible transition and again after the state is
   stable.
5. Press `End` after returning to gameplay.

Use the automatic `SNAPSHOT` blocks as the primary differential record; the
manual A/B/C snapshots are control markers. The script does not label a state
as native or correct; it records observed values only.
