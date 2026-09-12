# Cinematic Auto Aspect — Batch 2 Patch Result

## Scope

Applied the smallest lifecycle/cache correction identified by Batch 1:
fix-owned native-aspect restore observations must not replace the last
externally observed Auto aspect.

No FOV math, Full Hor+, subtitle logic or gameplay transition topology was
changed.

## Change

In `experimental_cinematic_21_9_combined_fix_204.cpp`, the gameplay writer
observation now treats this combination as an internal restore observation:

```text
ReplayState::AppliedConstrainPass
and aspect approximately 16:9
```

That observation is skipped when updating `g_lastObservedAspect`. The existing
`g_lastAutoRestoreSource` marker and restore write remain intact. A diagnostic
log line records the skipped fix-owned observation.

## Build validation

- Build script: `build-artifacts/test-scripts/build-experimental-cinematic-21x9-combined-fix-204.cmd`
- Output: `build-artifacts/test-asi/STALKER2CameraTweaks.asi`
- Build: **PASS**
- Runtime: **PASS** on Steam 2.0.4 with Windows aspect changes to 21:9 and
  32:9, including repeated cinematic ENTER/EXIT and gameplay recovery.

Runtime evidence:

- 21:9 Auto ENTER used aspect `2.38889` and transformed FOV `106.688`.
- 32:9 Auto ENTER used aspect `3.55556` and transformed FOV `126.87`.
- EXIT and subsequent gameplay camera re-arm/recovery completed on both
  tested aspect ratios.
- The recorded GPU crash during the 32:9 session was external to the mod and
  was not reproduced as a mod failure.

## Required regression matrix

1. 21:9 gameplay → first cinematic in `Auto`.
2. 21:9 repeated cinematic and EXIT.
3. 32:9 gameplay → first and repeated cinematic in `Auto`.
4. Change from 21:9 to 32:9 before the next cinematic.
5. Confirm the log reports the ultrawide aspect at ENTER and
   `Skipped fix-owned Auto restore...` for the internal 16:9 observation.
6. Re-check subtitle centering visually after Auto is corrected.

The subtitle issue remains a separate track and is not considered fixed by
this patch.

## Hotfix packaging

- Package: `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.5.1.zip`
- Package contents were checked and contain only the ASI, default INI, README,
  license and third-party notices.
- Hotfix ASI SHA-256:
  `7B6B486577C3604A0D7CB1315D0DCD071B32FA6CADDDE0E7DE7227076C28647B`
