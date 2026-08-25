# Testing And Research Summary

## Verified Solution

The game starts 32:9 gameplay in a camera state that produces the wrong view. The reliable in-game workaround was:

```text
16:9 with constrained aspect ratio enabled -> Auto
```

Runtime tracing showed the meaningful state transition:

```text
Broken wide state: aspect=3.55556, flags=0x4
Temporary constrained pass: aspect=3.55556, flags=0x5
Correct Auto state: aspect=1.77778, flags=0x4
```

`STALKER2GameplayAspectFix.asi` reproduces only those two writes at the validated gameplay camera function. It leaves the FOV selected in the game untouched. The same state can return after a cutscene, so the plugin re-arms when it detects that exact wide state again.

## Important 21:9 Finding

At `3440x1440` custom 21:9 output, keeping the constrained-aspect flag active is necessary for the intended viewport behavior. Changing the camera aspect alone to 16:9 while retaining that flag was tested successfully. A later output-FOV-only experiment was discarded because it was not the correct fix for that mode.

## Approaches That Were Tested And Rejected

### INI Aspect Constraint

`AspectRatioAxisConstraint=AspectRatio_MaintainYFOV` did not fix the live gameplay camera state. The game overwrites the relevant runtime values after loading gameplay.

### Fixed FOV Values

Forcing FOV 90 changed the symptom but did not reproduce the correct camera transition. It also ignored the player's FOV setting, so it was rejected.

### Static Letterbox Patches In The Gameplay Module

The [BigChenga Ultrawide / Flawless Widescreen solution](https://www.nexusmods.com/stalker2heartofchornobyl/mods/2337?tab=description) was inspected and its letterbox signatures were identified. Combining that approach with the gameplay hook in one ASI caused startup instability and other bugs. Letterbox and cutscene FOV work remain separate and are not part of this release while a stable solution is investigated.

### Broad Object And Viewport Tracking

Several object-array, viewport, and camera trackers were tested. They either produced no useful updates after settings changes or depended on unstable object discovery. They were discarded in favor of the validated live camera writer.

### Dumper-7 SDK Dump

Dumper-7 found `GObjects` but could not initialize `FNamePool` because the game layout was incompatible with its name decoder. It was useful for confirming object-layout direction, but not reliable enough for the final fix.

### Static Analysis

Ghidra analysis identified the active compact view writer at `RVA 0x00AF3A17`. The instruction is verified as `MOVSS [RBX+0x30], XMM0` before the hook is installed. This replaced blind signature edits with a build-validated camera hook.
