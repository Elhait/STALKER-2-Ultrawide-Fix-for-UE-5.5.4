# CinematicLetterboxObserver

Read-only UE4SS Phase 1 observer for cinematic letterbox ownership.

Install the folder into:

`Stalker2\Binaries\Win64\ue4ss\Mods\CinematicLetterboxObserver\Scripts\main.lua`

Add to `ue4ss\Mods\mods.txt`:

`CinematicLetterboxObserver : 1`

The observer discovers the live `CameraComponent`, `Stalker2.CameraManager`
and `PlayerCameraManager` chain without numeric IDs. It samples every 100 ms
and logs only ownership/state deltas to the UE4SS log. `End` forces a marker
snapshot; it does not write to the game.

For the cleanest run, disable the cinematic branch of CameraTweaks. Keep UE4SS
read-only and do not use Live View writes. Capture at least two, preferably
three, different cinematic entries. The important evidence is the first
`DELTA` ordering, not the repeated `STATE` values.
