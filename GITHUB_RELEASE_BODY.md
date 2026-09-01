### STALKER2UltrawideFix v0.4.0

Unified gameplay and cinematic ultrawide fix for S.T.A.L.K.E.R. 2, validated on Steam build 2.0.4 with the UE 5.5.4 target.

**Highlights**

- Gameplay correction for 21:9 and 32:9.
- Automatic re-arm after camera rebuilds, including death/load.
- Preserves the in-game gameplay FOV.
- Cinematic aspect correction using the runtime camera aspect.
- Hor+ cinematic FOV correction.
- Dynamic `Auto` aspect updates when changing resolution during the same game
  session, without requiring a restart.
- Custom cinematic framing: Auto, Native, 16:9, 21:9 and 32:9.
- Automatic creation of STALKER2UltrawideFix.ini.
- Guarded signature-based resolution with safe refusal on mismatch.
- SHA-256 logging for the loaded ASI and game executable.

**Configuration**

```ini
[Gameplay]
Enabled=true

[Cinematics]
; Auto, Native, 16:9, 21:9, 32:9
AspectRatio=Auto
```

**Installation**

1. Install Ultimate ASI Loader.
2. Place `dsound.dll` in `Stalker2\Binaries\Win64`.
3. Remove the previous `STALKER2GameplayAspectFix.asi` and its old INI/log.
4. Copy `STALKER2UltrawideFix.asi` and `STALKER2UltrawideFix.ini` into the same folder.
5. Launch the game.

**Tested**

- Steam build 2.0.4.
- UE 5.5.4.
- Gameplay at 21:9 and 32:9.
- Camera rebuilds after death/load.
- Cinematics at 21:9 and 32:9.
- Forced 32:9 cinematic framing at 2560x1440.
- `Auto` cinematic switching through `16:9 → 21:9 → 32:9 → 16:9 → 21:9 → 32:9`
  without restarting the game.
- `Native`, forced `16:9`, forced `21:9` and forced `32:9` at `5120x1440`.

**Known issues**

- A brief camera/projection jump may occur after a cinematic ends.
- Weapon/viewmodel FOV may be incorrect on 21:9 after cinematics, loading or camera rebuilds until aiming down sights or opening a menu refreshes it.
- Configuration file changes require a game restart.
- Future patches may require updated signatures.

Do not load this release together with the old `STALKER2GameplayAspectFix.asi`.

The release archive includes the ASI, default INI, README, license and third-party notices.
