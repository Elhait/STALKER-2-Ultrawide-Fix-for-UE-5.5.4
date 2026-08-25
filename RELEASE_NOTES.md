# Release Notes - 0.1.1

## Included

- `STALKER2GameplayAspectFix.asi`
- Gameplay aspect correction for the Steam UE 5.5.6 build.
- Automatic replay of the verified camera transition at game startup, plus re-arming for the known gameplay-camera rebuild state.
- Dynamic camera-writer resolution: the plugin scans `.text` for the complete verified camera-view copy path, then validates the target instruction as `MOVSS [RBX+0x30], XMM0` before the hook is installed.
- Safe refusal when the signature is absent, ambiguous, or no longer validates. The plugin does not hook an unknown target.

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

The release was also launched successfully against the Steam executable updated on 2026-08-25. This version does not contain cutscene changes.
