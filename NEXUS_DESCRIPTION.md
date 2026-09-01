# STALKER 2 Ultrawide Fix for UE 5.5.4 — Gameplay, Cinematics & Custom Framing

## Version 0.3.0

**Defender notice:** Microsoft reviewed the release binary and classified it as
**“Not malware.”** If an old detection remains, update Microsoft Defender
security intelligence and rescan the file.

Created and maintained by [Elhait](https://github.com/Elhait/STALKER-2-Ultrawide-Fix-for-UE-5.5.4).

## About

`STALKER2UltrawideFix.asi` is a unified gameplay and cinematic ultrawide fix
for the Steam version of **S.T.A.L.K.E.R. 2: Heart of Chornobyl**. It restores
the correct gameplay camera transition on ultrawide displays, corrects
cinematic aspect and FOV behavior, and adds optional custom cinematic framing.

The fix uses guarded signature resolution rather than relying only on fixed
addresses. If a required signature or instruction validation is missing,
ambiguous or incompatible, the ASI refuses to install that path safely.

Custom framing also allows users to deliberately choose 16:9, 21:9 or 32:9
cinematic presentation independently of their display aspect ratio.

## Support development

This mod is the result of extensive reverse engineering, runtime tracing,
implementation and testing across multiple game builds. The full development
process and the work completed for each stage are available here:

- [Testing and research history](https://github.com/Elhait/STALKER-2-Ultrawide-Fix-for-UE-5.5.4/blob/main/TESTING_AND_RESEARCH.md)
- [Project backlog and completed tasks](https://github.com/Elhait/STALKER-2-Ultrawide-Fix-for-UE-5.5.4/tree/main/backlog)

If this mod helped you and you would like to support future updates:

- [Ko-fi](https://ko-fi.com/elhait)
- [Donatello](https://donatello.to/Elhait)

Thank you for your support.

## Features

- Fixes the initial 21:9 and 32:9 gameplay camera state.
- Re-arms automatically after gameplay camera rebuilds, including death/load.
- Preserves the player's in-game gameplay FOV setting.
- Corrects cinematic aspect using the game's runtime camera aspect instead of
  desktop dimensions.
- Applies Hor+ FOV correction to cinematic transitions.
- Adds custom cinematic framing modes: `Auto`, `Native`, `16:9`, `21:9` and
  `32:9`.
- Allows 16:9 users to preview wider cinematic framing with letterbox bars.
- Creates the configuration file automatically when it is missing.
- Writes the loaded mod and game executable SHA-256 values to the startup log
  for easier support and version verification.

## Configuration

The ASI creates `STALKER2UltrawideFix.ini` beside the game executable on first
launch. The default configuration is:

```ini
[Gameplay]
Enabled=true

[Cinematics]
; Auto, Native, 16:9, 21:9, 32:9
AspectRatio=Auto
```

`Gameplay.Enabled=false` disables the gameplay aspect correction while leaving
cinematic options available.

`AspectRatio=Auto` follows the game's current runtime camera aspect and applies
matching Hor+ FOV. `Native` leaves the game's cinematic aspect and FOV behavior
untouched. The forced modes select the cinematic framing independently of the
physical display:

- `16:9` — forces 16:9 cinematic framing.
- `21:9` — 21:9 cinematic framing.
- `32:9` — 32:9 cinematic framing.

For example, forcing `32:9` on a 16:9 display produces a wider cinematic frame
with black bars above and below and matching FOV. Restart the game after
changing the configuration.

## Requirements

- Steam version of S.T.A.L.K.E.R. 2: Heart of Chornobyl.
- Game build tested: `2.0.4`.
- Unreal Engine target: `5.5.4`.
- [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)
  installed as `dsound.dll`.

## Installation

1. Install Ultimate ASI Loader and place `dsound.dll` in:

   ```text
   Stalker2\Binaries\Win64
   ```

2. Remove the previous `STALKER2GameplayAspectFix.asi` and its old INI/log.
   Do not load the old and unified ASI together.
3. Extract `STALKER2UltrawideFix.asi` and
   `STALKER2UltrawideFix.ini` into the same `Win64` folder.
4. Launch the game.

The plugin creates `STALKER2UltrawideFix.log` in the game folder. Include this
log when reporting a compatibility problem; it contains the mod and game
SHA-256 values and signature-resolution status.

## Tested compatibility

Tested on:

- Steam game build `2.0.4`.
- Unreal Engine `5.5.4` target.
- 21:9 gameplay, including startup, aspect switching and death/load rebuild.
- 32:9 gameplay regression.
- 21:9 and 32:9 cinematic framing with Hor+ FOV.
- Forced 32:9 cinematic framing at `2560x1440`, producing the expected
  letterbox presentation.

## Known Issues

- **Post-cinematic camera/projection jump:** a brief camera or projection jump
  may be visible immediately after a cinematic ends while native recovery hands
  control back to gameplay. The cinematic and gameplay corrections complete,
  but a short presentation handoff seam can remain visible.
- **21:9 weapon/viewmodel FOV:** weapon and viewmodel FOV can be incorrect after
  a cinematic, after loading, or after a camera rebuild. It may remain wrong
  until the player aims down sights or opens a menu, which refreshes the game's
  viewmodel state. This is a separate game-side issue.
- Resolution changes during a running session are not part of the validated
  scope. Restart the game after changing resolution.
- Signature-based resolution improves resilience to address relocation but does
  not guarantee compatibility with future patches. Game updates that only
  relocate validated code may continue to work without a mod update, while
  structural changes may require new signatures. The ASI fails safely when
  validation does not pass.
- Do not combine this release with the old gameplay ASI or other experimental
  cinematic ASI files.

## Changelog

### 0.3.0

- Unified gameplay and cinematic support in `STALKER2UltrawideFix.asi`.
- Added signature-resolved cinematic aspect and live FOV boundaries.
- Added Hor+ cinematic FOV correction for runtime and forced framing modes.
- Added `Auto`, `Native`, `16:9`, `21:9` and `32:9` cinematic framing.
- Added automatic INI creation and independent gameplay/cinematic settings.
- Added SHA-256 logging for the loaded mod and game executable.
- Generalized gameplay correction for ultrawide camera rebuilds, including
  21:9 startup and death/load re-entry.

### 0.1.1

- Gameplay-only aspect correction for the Steam UE 5.5.4 build.
- Replaced the fixed gameplay camera address with guarded signature resolution
  and instruction validation.
- Preserved the game's selected gameplay FOV and the verified two-pass aspect
  transition.
- Refused safely when the camera-writer signature was absent, ambiguous or
  invalid.

### 0.1.0

- Historical gameplay aspect-fix release for the earlier Steam 2.0.2 build.
- Superseded by the signature-resolved releases above.
