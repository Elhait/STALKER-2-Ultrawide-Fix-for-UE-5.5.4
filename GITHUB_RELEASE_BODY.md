# STALKER 2 Gameplay Aspect Fix 0.1.1

Gameplay-only update with a dynamic camera-writer resolver.

Maintained by [Elhait](https://github.com/Elhait).

## Included

- Fixes the incorrect 32:9 gameplay camera state.
- Automatically replays the known-good `16:9 -> Auto` camera transition.
- Re-arms the fix when it detects the known post-cutscene gameplay-camera state.
- Preserves the game's own FOV setting.
- Finds the verified camera writer by instruction signature instead of relying on a fixed executable address.
- Refuses to install if the signature is ambiguous or the decoded target no longer matches the expected camera write.

## Requirements

- Steam version of S.T.A.L.K.E.R. 2: Heart of Chornobyl.
- Ultimate ASI Loader installed as `dsound.dll`.

## Not Included

Cutscene letterbox and cutscene FOV changes are not included in this first release. Work on them is ongoing.

I tested integrating the [BigChenga Ultrawide / Flawless Widescreen solution](https://www.nexusmods.com/stalker2heartofchornobyl/mods/2337?tab=description), but bugs remain in the ASI integration. It is excluded until a stable solution is found.

See the bundled README for installation, compatibility, and troubleshooting.
