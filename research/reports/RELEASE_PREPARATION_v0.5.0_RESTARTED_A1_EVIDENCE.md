# Release Preparation v0.5.0 — Restarted A1 Evidence and Claims

Status: PASS

## Reused evidence

- The previous v0.5.0 A1 matrix is reused as the factual baseline where its
  claims remain unchanged.
- The dialogue lifetime/invalidation regression is added as new evidence.
- The renamed production identity and final source/build state are treated as
  new downstream validation inputs; previous binary and archive hashes are not
  inherited as release authority.

## Actual release delta: v0.4.0 → v0.5.0

- Added configurable dialogue zoom policies: `Native`, `Adaptive`, `Reduced`
  and `Disabled`.
- Added projection-space optical dialogue transformation with preserved native
  ENTER timing and corrected EXIT anchor/recovery.
- Added provisional dialogue-candidate activation and invalidation after a
  material gameplay FOV-context change, preventing stale baseline recovery.
- Added cinematic/dialogue lifecycle isolation.
- Added optional F9/F10 runtime policy hotkeys, configurable through the INI;
  hotkeys are disabled by default and affect the next applicable lifecycle.
- Added supported hotkey parsing for F1-F12, 0-9 and A-Z.
- Added managed INI category/comment synchronization. Previous INI settings
  are not migrated; supported values in the current INI remain subject to the
  managed template rules.
- Renamed the current technical identity to `STALKER2CameraTweaks.*` while
  preserving old names in migration guidance and historical evidence.
- Preserved the existing gameplay and cinematic aspect/FOV implementation.

## Evidence-to-claim matrix

| Feature or claim | Evidence | Status | Allowed public wording |
| --- | --- | --- | --- |
| Dialogue boundary | Unique validated 2.0.4 boundary at RVA `0xD20F77` | Runtime PASS | Dialogue zoom tested on Steam 2.0.4 |
| Dialogue resolver portability | Static matching-image validation on Steam 2.0.2/2.0.3/2.0.4 | Static PASS | Resolver statically validated across Steam 2.0.2, 2.0.3 and 2.0.4 |
| Native dialogue | Prior bounded runtime/visual matrix | Runtime PASS | Preserves native dialogue zoom behavior, targeting the game's native 70° dialogue FOV |
| Adaptive dialogue | Projection-space runtime/visual validation at multiple gameplay FOVs | Runtime PASS | Preserves native optical zoom strength relative to current gameplay FOV |
| Reduced dialogue | Optical half-strength runtime/visual validation | Runtime PASS | Applies half of Adaptive optical zoom strength |
| Disabled dialogue | Runtime/visual validation holding gameplay baseline | Runtime PASS | Keeps gameplay FOV during dialogue |
| Smooth EXIT recovery | Runtime evidence for EXIT discontinuity and anchor/recovery | Runtime PASS | Preserves smooth dialogue recovery |
| FOV-context invalidation | Diagnostic and production-style 2.0.4 regression: `120→90` and `90→120` on same source | Runtime PASS, bounded | Clears stale transient dialogue state after a material gameplay FOV change |
| Cinematic/dialogue isolation | Cinematic recovery regression and visual validation | Runtime PASS, bounded | Dialogue state is isolated from cinematic lifecycle |
| ADS specificity | ADS-only control produced zero dialogue-boundary hits | Runtime PASS, bounded | Tested ADS does not activate the dialogue boundary |
| Fail-closed handling | Static guards and native pass-through contract | Static/implementation PASS, bounded | Invalid resolver/sample/state conditions fail safely to native behavior |
| Runtime hotkeys | Runtime policy cycling and custom binding tests | Runtime PASS, bounded | Optional hotkeys select policy for the next applicable lifecycle |
| INI behavior | Template synchronization and persistence tests; migration intentionally removed | Runtime PASS, bounded | Current configuration is created/synchronized; previous INI settings are not migrated |
| Renamed identity | New ASI/INI/log names and production-style runtime identity | Runtime PASS, bounded | Current release uses `STALKER2CameraTweaks.*` |
| Weapon/viewmodel FOV | Research remains unresolved | Deferred | Not fixed by v0.5.0 |
| Panini projection | Future research | Deferred | Not included in v0.5.0 |

## Runtime and compatibility boundaries

- Runtime evidence is limited to Steam 2.0.4 game SHA-256
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Static resolver portability across 2.0.2/2.0.3/2.0.4 is not runtime support
  proof for those older builds.
- Previous preparation reports and hashes are historical evidence only.
- Manual INI edits require a game restart. Runtime hotkey selections do not,
  but apply only to the next corresponding lifecycle.

## A1 gate

The previous evidence matrix remains valid where unchanged. New evidence covers
the dialogue invalidation fix and rename blast radius, while the intentional
removal of previous-INI migration is explicitly reflected. Every promoted
feature has an evidence classification and public-claim ceiling.

A1 Evidence / Release Delta / Claim Matrix: PASS.

Next permitted batch: A2 — Production Binary Gate.
