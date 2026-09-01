# Release Notes — 0.3.0

## STALKER2UltrawideFix.asi

Version 0.3.0 replaces the gameplay-only `STALKER2GameplayAspectFix.asi` with one unified configurable ASI for gameplay, cinematics and custom cinematic framing.

## Included

- Dynamic gameplay aspect correction for 21:9 and 32:9.
- Re-arming after gameplay camera rebuilds, including death/load.
- Preservation of the player's selected gameplay FOV.
- Cinematic aspect correction using the game's runtime camera aspect.
- Hor+ cinematic FOV correction at the live cinematic consumer boundary.
- Custom cinematic framing modes: `Auto`, `Native`, `16:9`, `21:9` and `32:9`.
- Automatic creation of `STALKER2UltrawideFix.ini` with safe defaults.
- Startup SHA-256 logging for the loaded ASI and game executable.
- Guarded signature resolution for gameplay and cinematic boundaries, with fail-closed behavior when signatures or instruction validation do not match.

## Configuration

```ini
[Gameplay]
Enabled=true

[Cinematics]
; Auto, Native, 16:9, 21:9, 32:9
AspectRatio=Auto
```

`Auto` uses the runtime camera aspect and matching Hor+ FOV. `Native` bypasses the cinematic aspect and FOV overrides. Forced ratios provide custom framing and matching FOV independently of the display; for example, `32:9` on a 16:9 display produces letterboxed cinematics.

## Compatibility and validation

- Validated with the Steam game build `2.0.4` using the UE 5.5.4 target.
- Gameplay runtime-tested at 21:9 and 32:9, including aspect switching, death/load camera rebuilds and FOV preservation.
- Cinematic runtime-tested at 21:9 and 32:9, including forced 32:9 framing at 2560x1440.
- The cinematic and gameplay boundaries resolve through guarded signatures, but signature resolution does not guarantee support for a future game patch.
- The plugin refuses safely when validation is missing, ambiguous or invalid.

## Known limitations

- A brief post-cinematic projection/FOV handoff transition may still be visible in some scenarios.
- Weapon/viewmodel FOV after loading on 21:9 is a separate game-side issue and is not addressed by this release.
- Resolution changes during a running session are not part of the validated scope; restart the game after changing resolution.
- Do not load `STALKER2GameplayAspectFix.asi` together with this unified ASI.

## Upgrade notes

Remove the previous `STALKER2GameplayAspectFix.asi` and its old INI/log before installing `STALKER2UltrawideFix.asi`. Copy the new ASI and INI into the game's `Stalker2\\Binaries\\Win64` directory and restart the game.

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
It has been superseded by the unified `STALKER2UltrawideFix.asi` in version
0.3.0. The older historical 0.1.0 archive remains separate and targets the
earlier Steam 2.0.2 release.
