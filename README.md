# STALKER 2 Gameplay Aspect Fix

Source for the gameplay-only Steam 2.0.2 release. See [RELEASE_NOTES.md](RELEASE_NOTES.md) and [TESTING_AND_RESEARCH.md](TESTING_AND_RESEARCH.md) for scope, testing, and limitations.

## Download

Prebuilt releases and installation instructions are available on [Nexus Mods](https://www.nexusmods.com/stalker2heartofchornobyl/mods/2416).

## Screenshots

32:9 gameplay comparison, Default FOV 90:

| Without Fix | Fix Enabled |
| --- | --- |
| ![32:9 without fix](screens/32-9%20default%20fov%2090%20without%20fix%20-%20nexus.jpg) | ![32:9 fix enabled](screens/32-9%20default%20fov%2090%20fix%20on%20-%20nexus.jpg) |

16:9 baseline, Default FOV 90:

![16:9 baseline](screens/16-9%20default%20fov%2090%20without%20fix.png)

## Build Requirements

- Visual Studio 2022 with the Desktop development with C++ workload.
- C++23 compiler support.
- [SafetyHook](https://github.com/cursey/safetyhook), including its bundled Zydis source.
- [spdlog](https://github.com/gabime/spdlog).

Place the dependencies under `external/safetyhook` and `external/spdlog` respectively, then run `build.cmd` from this directory.

## Release Scope

This source intentionally contains only the verified gameplay aspect fix. It does not include experimental trackers, Dumper-7 modifications, cutscene letterbox changes, or cutscene FOV changes.

## License And Credits

Copyright for this project is held by Elhait and released under MIT. Some helper and ASI-scaffolding portions are derived from Lyall's MIT-licensed STALKER2Tweak; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Support Development

If this mod helped you and you would like to support future updates or cutscene fixes:

- [Ko-fi: elhait](https://ko-fi.com/elhait)
- PayPal: `entoni97l@gmail.com`

Thank you for your support.
