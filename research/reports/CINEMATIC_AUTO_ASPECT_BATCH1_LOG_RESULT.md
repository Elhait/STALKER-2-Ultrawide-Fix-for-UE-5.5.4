# Cinematic Auto Aspect — Batch 1 Log Result

## Runtime identity

- Mod runtime identity: present in log
- Game SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Configuration: `Cinematics.AspectRatio=Auto`
- Cinematic ENTER/EXIT hooks: validated and installed
- Camera writer hook: validated and installed

## Observed sequence

The log shows the following relevant progression:

```text
Gameplay source aspect: 1.77778
Gameplay source aspect: 2.38889
Replayed constrained pass: aspect=2.38889, flags=0x5
Normalized 21:9 camera aspect: 2.38889 -> 1.77778, flags=0x5
Replayed Auto restore: aspect=1.77778, flags=0x4
Cinematic ENTER: aspect=1.77778, policy=Auto
```

The source object remains the same throughout the sequence. The important
point is that the runtime had already observed the ultrawide value
`2.38889`, but the first cinematic received `1.77778`.

## Static/runtime correlation

The current implementation updates `g_lastObservedAspect` in
`ReplayManualTransitionOriginal` before the state-specific writes complete.
The guard checks `g_lastAutoRestoreSource`, but that marker is assigned only
after the native-aspect restore write. Therefore the restore observation can
be cached as the new Auto runtime aspect before the marker becomes active.

Relevant order:

```text
read aspect
→ cache g_lastObservedAspect
→ write native 16:9 Auto restore
→ set g_lastAutoRestoreSource
```

At cinematic ENTER, `ReadRuntimeAspect(0)` then returns the cached native
`1.77778` instead of the previously observed ultrawide aspect.

## Result

**Batch 1: PASS — reproducible Auto source/timing explanation.**

The log confirms that the reported Auto failure is not an inability to read
the display aspect. It is a cache lifecycle issue: the fix's own native
restore overwrites the cached aspect immediately before cinematic ENTER.

This does not yet authorize a production patch. Batch 2 must test the smallest
reversible correction that prevents the fix-owned native restore from replacing
the authoritative ultrawide Auto aspect, while preserving the existing
two-pass gameplay transition and EXIT behavior.

## Subtitle status

This log contains no subtitle layout coordinates or UI ownership evidence.
Subtitle left-shift remains an independent unresolved track.
