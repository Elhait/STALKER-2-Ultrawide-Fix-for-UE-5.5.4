# Release Preparation v0.5.0 — Restarted A0 Scope Lock

Status: PASS — scope locked; A1 is the next permitted batch.

## Run policy

This is a new release-preparation run. Results from the superseded preparation
run are not inherited. The moved task plans and existing repository history are
inputs for review, not release-ready gate results.

No commit, tag, push or publication is performed by this run.

## Release identity

- Previous release baseline: `v0.4.0`.
- Target release: `v0.5.0`.
- Canonical public name: `STALKER 2 Ultrawide and Camera Tweaks for UE 5.5.4`.
- Current technical identity: `STALKER2CameraTweaks.asi`,
  `STALKER2CameraTweaks.ini`, `STALKER2CameraTweaks.log`.
- Target archive name:
  `STALKER2CameraTweaks-UE5.5.4-v0.5.0.zip`.
- Existing repository URL/slug is unchanged.

## Promoted production scope

- Gameplay ultrawide aspect correction with the existing two-pass behavior.
- Cinematic aspect/FOV correction and configurable cinematic framing.
- Dialogue zoom policies: `Native`, `Adaptive`, `Reduced` and `Disabled`.
- Projection-space optical dialogue model with lifecycle-aware ENTER/EXIT
  handling.
- Dialogue transient-state invalidation after material gameplay FOV context
  changes; bounded Steam 2.0.4 runtime regression passed.
- Optional runtime hotkeys for the next applicable cinematic/dialogue
  lifecycle, with F9/F10 defaults and INI-configurable supported keys.
- Managed INI template/category/comment synchronization without migration of
  previous INI settings.
- Dynamic dialogue resolver statically validated across Steam 2.0.2, 2.0.3 and
  2.0.4; runtime validation remains limited to Steam 2.0.4.

## Accepted limitations and non-goals

- Weapon/viewmodel FOV is deferred and must not be presented as fixed.
- Panini projection work is deferred.
- Runtime hotkeys are disabled by default and apply to the next corresponding
  lifecycle rather than an already active one.
- Runtime validation does not establish compatibility with other game builds
  beyond the identified Steam 2.0.4 executable.
- Existing historical names, old release records, research logs and prior
  reports remain factual provenance and are not globally renamed.
- No GitHub/Nexus publication, tag, push or commit is part of A0.

## Evidence boundary

- Runtime evidence: Steam 2.0.4 game image hash
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- The latest production-style candidate and its hash will be revalidated by A2;
  no previous binary or archive hash is inherited as release authority.
- Current working-tree/index state contains pre-existing staged and generated
  changes; A8 must classify them explicitly and must not be inferred from A0.

## Gate decision

The release scope, promoted features, accepted limitations and explicit
non-goals are unambiguous. A0: PASS.

Next permitted batch: A1 — Evidence, release delta and claim matrix.
