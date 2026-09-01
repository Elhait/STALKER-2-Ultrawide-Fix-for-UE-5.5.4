# STALKER 2 Ultrawide Fix for UE 5.5.4 — Gameplay, Cinematics & Custom Framing

## Version 0.4.0

Source for `STALKER2UltrawideFix.asi`, a unified gameplay and cinematic ultrawide fix for **S.T.A.L.K.E.R. 2: Heart of Chornobyl**. The current implementation is validated against the Steam game build tested with Unreal Engine `5.5.4`.

## Features

- Corrects gameplay aspect transitions on 21:9 and 32:9 displays.
- Re-arms gameplay correction after camera rebuilds, including death/load.
- Preserves the player's selected gameplay FOV.
- Corrects cinematic aspect and applies Hor+ cinematic FOV.
- Uses the game's runtime camera aspect rather than desktop dimensions.
- Dynamically follows `Auto` aspect changes during the same game session without requiring a restart.
- Supports custom cinematic framing independently of the physical display: `Auto`, `Native`, `16:9`, `21:9` or `32:9`.
- Creates `STALKER2UltrawideFix.ini` automatically when it is missing.
- Resolves gameplay and cinematic hook locations through guarded signatures and refuses safely when validation is ambiguous or fails.

## Download and installation

Prebuilt releases and installation instructions are available on [Nexus Mods](https://www.nexusmods.com/stalker2heartofchornobyl/mods/2416).

For local builds:

1. Install [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) as `dsound.dll` in `Stalker2\\Binaries\\Win64`.
2. Remove the previous `STALKER2GameplayAspectFix.asi` and its old INI/log. Do not load both ASIs together.
3. Copy `STALKER2UltrawideFix.asi` and `STALKER2UltrawideFix.ini` to the same `Win64` directory.
4. Start the game normally.

The plugin creates `STALKER2UltrawideFix.log` beside the game executable. The startup log includes SHA-256 values for the loaded mod and game executable, which helps verify support reports.

## Configuration

```ini
[Gameplay]
Enabled=true

[Cinematics]
; Auto, Native, 16:9, 21:9, 32:9
AspectRatio=Auto
```

`Auto` follows the game's runtime camera aspect and applies matching Hor+ FOV. It also updates when the game resolution changes during the same session. `Native` leaves the game's cinematic aspect and FOV behavior untouched. Forced modes provide custom cinematic framing and matching FOV; for example, forcing `32:9` on a 16:9 display produces cinematic letterbox bars. Configuration changes take effect after restarting the game.

## Build requirements

- Visual Studio 2022 with the Desktop development with C++ workload.
- C++23 compiler support.
- [SafetyHook](https://github.com/cursey/safetyhook), including its bundled Zydis source.
- [spdlog](https://github.com/gabime/spdlog).

Place dependencies under `external/safetyhook` and `external/spdlog`, then run `build.cmd` from this directory.

## Tested scope and limitations

- Runtime-tested on Steam game build `2.0.4`; the tested executable identity is recorded in the startup log.
- Gameplay tested at 21:9 and 32:9, including startup, hot aspect switching, death/load rebuild and FOV preservation.
- `Auto` cinematics tested through `16:9 → 21:9 → 32:9 → 16:9 → 21:9 → 32:9` without restarting the game.
- Cinematics tested with `Native`, forced `16:9`, forced `21:9` and forced `32:9` policies on `5120x1440`.
- Forced 32:9 framing was also tested at `2560x1440` and correctly produced cinematic letterbox bars.
- A brief post-cinematic projection/FOV handoff transition may still be visible in some scenarios.
- Weapon/viewmodel FOV after loading on 21:9 is a separate known game issue and is not fixed by this mod.
- Changing the game resolution during a session is validated for `AspectRatio=Auto`; restart the game after changing the configuration file itself.
- Signature resolution improves resilience to address relocation but does not guarantee compatibility with future patches. The plugin fails safely when validation does not pass.

## Screenshots

32:9 gameplay comparison, default FOV 90:

| Without Fix | Fix Enabled |
| --- | --- |
| ![32:9 without fix](screens/32-9%20default%20fov%2090%20without%20fix%20-%20nexus.jpg) | ![32:9 fix enabled](screens/32-9%20default%20fov%2090%20fix%20on%20-%20nexus.jpg) |

16:9 baseline, default FOV 90:

![16:9 baseline](screens/16-9%20default%20fov%2090%20without%20fix.png)

## Cinematic framing comparisons

The same cinematic framing policy can be selected independently of the
physical display aspect ratio. These examples show `Auto` at 16:9, and forced
16:9, 21:9 and 32:9 framing on a 32:9 display, plus forced 32:9 framing on a
16:9 display.

| Configuration | Example |
| --- | --- |
| `Auto` at 2560x1440 | ![Auto cinematic framing at 2560x1440](screens/Cutscene%20Auto-Default%20in%202560x1440.jpg) |
| Forced `16:9` at 5120x1440 | ![Forced 16:9 cinematic framing](screens/Cutscene%2016-9%20in%205120x1440.jpg) |
| Forced `21:9` at 5120x1440 | ![Forced 21:9 cinematic framing](screens/Cutscene%2021-9%20in%205120x1440%2Cjpg.jpg) |
| Forced `32:9` at 5120x1440 | ![Forced 32:9 cinematic framing](screens/Cutscene%2032-9%20in%205120x1440jpg.jpg) |
| Forced `32:9` at 2560x1440 | ![Forced 32:9 cinematic letterbox](screens/Cutscene%2032-9%20in%202560x1440.jpg) |

## License and credits

Copyright for this project is held by Elhait and released under MIT. Some helper and ASI-scaffolding portions are derived from Lyall's MIT-licensed STALKER2Tweak; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

This project is independent and is not a release of Lyall's STALKER2Tweak.

## Support development

- [Ko-fi](https://ko-fi.com/elhait)
- [Donatello](https://donatello.to/Elhait)
