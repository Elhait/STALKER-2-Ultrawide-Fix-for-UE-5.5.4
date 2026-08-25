# STALKER 2 Gameplay Aspect Fix

Source for the gameplay-only Steam 2.0.2 release. See [RELEASE_NOTES.md](RELEASE_NOTES.md) and [TESTING_AND_RESEARCH.md](TESTING_AND_RESEARCH.md) for scope, testing, and limitations.

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
