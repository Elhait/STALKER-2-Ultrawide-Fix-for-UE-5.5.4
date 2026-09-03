# Release Preparation v0.5.0 — A0 Scope Lock

Status: PASS

## Release identity

- Current release: v0.5.0
- Previous release: v0.4.0
- Target artifact: unified `STALKER2UltrawideFix.asi`
- Target engine: UE 5.5.4
- Target game channel: Steam

The supplied current Nexus description is treated as the v0.4.0 baseline.
It is not accepted as final v0.5.0 documentation and will be handled in A4.

## Promoted production scope

- Existing gameplay ultrawide aspect correction.
- Existing cinematic aspect/FOV correction and configurable framing:
  `Auto`, `Native`, `16:9`, `21:9`, `32:9`.
- Dialogue zoom policies:
  `Native`, `Adaptive`, `Reduced`, `Disabled`.
- Dialogue lifecycle-aware sample transformation and smooth EXIT recovery.
- Cinematic/dialogue isolation and fail-closed dialogue handling.
- Optional configurable runtime hotkeys for the next applicable cinematic/dialogue.
- INI template synchronization/migration while preserving supported user values.
- Static dialogue resolver portability validation across Steam 2.0.2, 2.0.3
  and 2.0.4, with runtime validation limited to Steam 2.0.4.

## Accepted limitations

- Runtime support claims are limited to the tested Steam 2.0.4 executable.
- Static cross-patch portability is not runtime compatibility proof for 2.0.2
  or 2.0.3 and does not predict future patches.
- F9/F10 policy changes apply to the next corresponding lifecycle, not one
  already in progress.
- Weapon/viewmodel FOV ownership remains deferred and is not part of v0.5.0.
- Manual INI edits require a game restart; runtime hotkey selection does not.

## Explicit non-goals

- No weapon/viewmodel FOV implementation.
- No new renderer or generic camera research.
- No feasibility-only policy names or diagnostic instrumentation.
- No new overlay/UI subsystem.
- No automatic Git commit, tag, push, GitHub publication or Nexus publication.
- No deletion or cleanup of unrelated research/build artifacts.

## Target source and evidence rule

The current production source is the candidate source state for A2. The approved
binary identity, archive identity and all release-facing documentation must be
re-established by the new A1–A8 run. No result, SHA-256, archive hash or READY
status from the superseded preparation pass is inherited.

## Gate result

Release scope, promoted features and non-goals are unambiguous: PASS.

A1 is the next batch. It must produce the actual v0.4.0 → v0.5.0 release delta
and evidence/claim matrix before any release documentation is edited.
