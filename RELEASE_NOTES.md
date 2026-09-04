# Release Notes — 0.5.0

## STALKER2CameraTweaks.asi

Version 0.5.0 extends the unified configurable ASI with FOV-aware dialogue zoom control.

## Included

- Dynamic gameplay aspect correction for 21:9 and 32:9.
- Re-arming after gameplay camera rebuilds, including death/load.
- Preservation of the player's selected gameplay FOV.
- Cinematic aspect correction using the game's runtime camera aspect.
- Hor+ cinematic FOV correction at the live cinematic consumer boundary.
- Custom cinematic framing modes: `Auto`, `Native`, `16:9`, `21:9` and `32:9`.
- Automatic creation of `STALKER2CameraTweaks.ini` with safe defaults.
- Startup SHA-256 logging for the loaded ASI and game executable.
- Guarded signature resolution for gameplay and cinematic boundaries, with fail-closed behavior when signatures or instruction validation do not match.
- Dialogue zoom modes: `Native`, `Adaptive`, `Reduced` and `Disabled`.
- `Adaptive` preserves native optical zoom strength relative to the current gameplay FOV; `Reduced` applies half of that optical effect.
- Dialogue transitions preserve native timing, including smooth EXIT recovery back to the actual gameplay FOV baseline.
- Optional F9/F10 hotkeys select the policy for the next cinematic or dialogue; hotkeys are disabled by default.
- Dialogue resolver contract statically validated across Steam builds 2.0.2, 2.0.3 and 2.0.4; runtime validation is limited to 2.0.4.

## Configuration

```ini
[Gameplay]
Enabled=true

[Cinematics]
; Auto, Native, 16:9, 21:9, 32:9
AspectRatio=Auto

[Dialogue]
; Native   - keep the game's original dialogue zoom, currently targeting 70°.
; Adaptive - preserve the native optical zoom strength relative to the current gameplay FOV.
; Reduced  - apply half of the Adaptive optical zoom strength.
; Disabled - keep the current gameplay FOV during dialogue.
Zoom=Reduced

[Hotkeys]
; Optional runtime controls for quickly testing different settings without restarting the game.
; Intended mainly for comparing modes and finding a preferred configuration; disable for normal use.
; Supported keys: F1-F12, 0-9 and A-Z.
Enabled=false

; Key used to cycle the cinematic mode for the next cinematic.
; Auto -> Native -> 16:9 -> 21:9 -> 32:9 -> Auto.
; Does not affect a cinematic that is already playing.
CinematicCycle=F9

; Key used to cycle the dialogue zoom mode for the next dialogue.
; Native -> Adaptive -> Reduced -> Disabled -> Native.
; Does not affect a dialogue that is already in progress.
DialogueCycle=F10
```

`Auto` uses the runtime camera aspect and matching Hor+ FOV. `Native` bypasses the cinematic aspect and FOV overrides. Forced ratios provide custom framing and matching FOV independently of the display; for example, `32:9` on a 16:9 display produces letterboxed cinematics.

## Compatibility and validation

- Validated with the Steam game build `2.0.4` using the UE 5.5.4 target.
- Gameplay runtime-tested at 21:9 and 32:9, including aspect switching, death/load camera rebuilds and FOV preservation.
- Cinematic runtime-tested at 21:9 and 32:9, including forced 32:9 framing at 2560x1440.
- The cinematic and gameplay boundaries resolve through guarded signatures, but signature resolution does not guarantee support for a future game patch.
- The plugin refuses safely when validation is missing, ambiguous or invalid.

## Known limitations

- Weapon/viewmodel FOV behavior is a separate game-side area and is not modified by the dialogue zoom feature.
- `AspectRatio=Auto` was validated across runtime resolution changes during the same session. Restart the game after manually changing the configuration file.
- Do not load `STALKER2GameplayAspectFix.asi` together with this unified ASI.

## Upgrade notes

Remove previous `STALKER2UltrawideFix.asi`, `STALKER2GameplayAspectFix.asi` and their old INI/log files before installing `STALKER2CameraTweaks.asi`. Copy the new ASI and INI into the game's `Stalker2\\Binaries\\Win64` directory and restart the game.

## Previous version — 0.1.1

Version 0.1.1 was the gameplay-only release for the Steam UE 5.5.4 build. It
included:

- Dynamic resolution of the gameplay camera writer from a complete executable
  `.text` signature.
- Instruction validation of the resolved `MOVSS [RBX+0x30], XMM0` camera-view
  copy path before installing the hook.
- The verified two-pass gameplay aspect transition while preserving the
  player's selected FOV.
- Safe refusal when the signature was absent, ambiguous or failed validation.

The 0.1.1 release intentionally did not include cinematic aspect correction,
cinematic FOV correction, custom cinematic framing or viewmodel FOV changes.
It has been superseded by the unified `STALKER2CameraTweaks.asi` in version
0.3.0. The older historical 0.1.0 archive remains separate and targets the
earlier Steam 2.0.2 release.
