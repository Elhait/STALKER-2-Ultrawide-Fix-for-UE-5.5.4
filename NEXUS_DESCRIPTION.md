# STALKER 2 Gameplay Aspect Fix - Steam 2.0.2

Fixes the broken ultrawide gameplay camera state introduced by the UE 5.5 update. The mod restores the same internal camera transition produced by the reliable manual workaround `16:9 -> Auto`, but does it automatically.

Created and maintained by [Elhait](https://github.com/Elhait).

## Features

- Fixes the initial 32:9 gameplay view.
- Re-arms the fix when the exact gameplay-camera rebuild state is detected after a cutscene.
- Does not force FOV 90 or override your in-game FOV setting.
- Keeps constrained aspect-ratio behavior for a real 21:9 output.
- Includes a build guard for the tested Steam 2.0.2 executable.

## Requirements

- Steam version of S.T.A.L.K.E.R. 2: Heart of Chornobyl, build 2.0.2.
- Ultimate ASI Loader installed as `dsound.dll`.

## Installation

1. Install `dsound.dll` from Ultimate ASI Loader in `Stalker2\\Binaries\\Win64`.
2. Extract `STALKER2GameplayAspectFix.asi` into that same folder.
3. Delete other experimental `STALKER2*.asi` files to prevent conflicts.
4. Launch the game.

## Not Included

This first release does not include cutscene letterbox removal or cutscene FOV changes. Work on those fixes is ongoing.

I tested an integration of the [BigChenga Ultrawide / Flawless Widescreen solution](https://www.nexusmods.com/stalker2heartofchornobyl/mods/2337?tab=description). It still has bugs in this ASI-based setup, so it is deliberately excluded while I investigate a stable solution.

## Compatibility

Tested on Steam 2.0.2, UE 5.5.6, `5120x1440` (32:9), and `3440x1440` (custom 21:9). A later game update will make the plugin disable itself rather than apply a hook to an unknown executable.

## Support

If it does not activate, attach `STALKER2GameplayAspectFix.log` and state your game build, store, resolution, and aspect-ratio setting.
