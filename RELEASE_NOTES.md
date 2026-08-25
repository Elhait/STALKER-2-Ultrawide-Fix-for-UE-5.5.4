# Release Notes - 0.1.0

## Included

- `STALKER2GameplayAspectFix.asi`
- Gameplay aspect correction for the Steam 2.0.2 executable.
- Automatic replay of the verified camera transition at game startup, plus re-arming for the known gameplay-camera rebuild state.
- Safe executable validation before the hook is installed.

## Excluded On Purpose

- Cutscene letterbox patches.
- Cutscene FOV scaling.
- Viewmodel FOV changes.
- Console, HUD, input, intro, or other unrelated tweaks from the original STALKER2Tweak.

## Verification

The release was visually tested at 5120x1440 (32:9). The log confirmed the two camera passes:

```text
Replayed constrained pass: ... aspect=3.55556 flags=0x5.
Replayed Auto restore: ... aspect=1.77778 flags=0x4.
```

The build also re-arms this transition when it detects the known post-cutscene camera state. Run one final visual cutscene test before public publication.
