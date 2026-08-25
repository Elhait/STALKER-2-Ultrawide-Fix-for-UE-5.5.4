# STALKER 2 Gameplay Aspect Fix 0.1.0

First public gameplay-only release for Steam 2.0.2.

Maintained by [Elhait](https://github.com/Elhait).

## Included

- Fixes the incorrect 32:9 gameplay camera state.
- Automatically replays the known-good `16:9 -> Auto` camera transition.
- Re-arms the fix when it detects the known post-cutscene gameplay-camera state.
- Preserves the game's own FOV setting.

## Requirements

- Steam build 2.0.2.
- Ultimate ASI Loader installed as `dsound.dll`.

## Not Included

Cutscene letterbox and cutscene FOV changes are not included in this first release. Work on them is ongoing.

I tested integrating the [BigChenga Ultrawide / Flawless Widescreen solution](https://www.nexusmods.com/stalker2heartofchornobyl/mods/2337?tab=description), but bugs remain in the ASI integration. It is excluded until a stable solution is found.

See the bundled README for installation, compatibility, and troubleshooting.
