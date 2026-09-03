### STALKER2UltrawideFix v0.5.0

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
- Dialogue zoom modes: Native, Adaptive, Reduced and Disabled.
- Adaptive preserves native optical zoom strength relative to the actual gameplay FOV; Reduced applies half of that optical effect.
- Dialogue transitions preserve native timing and use smooth EXIT recovery back to the gameplay FOV baseline.
- Optional F9/F10 hotkeys select the policy for the next cinematic or dialogue; hotkeys are disabled by default.
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

- Weapon/viewmodel FOV behavior is a separate game-side area and is not modified by the dialogue zoom feature.
- Manual configuration file changes require a game restart; runtime hotkey selections apply to the next corresponding lifecycle without restarting.
- Dialogue resolver portability was statically validated on Steam 2.0.2, 2.0.3 and 2.0.4; runtime dialogue validation is limited to Steam 2.0.4.
- Future patches may require updated signatures.

Do not load this release together with the old `STALKER2GameplayAspectFix.asi`.

The release archive includes the ASI, default INI, README, license and third-party notices.
