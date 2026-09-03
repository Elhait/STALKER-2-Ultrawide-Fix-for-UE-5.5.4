# Release Preparation v0.5.0 — A1 Evidence and Release Delta

Status: PASS

## Evidence sources reviewed

- Completed dialogue, hotkey, configuration, matching-image and cross-patch task plans in research/completed/.
- backlog/TASKLOG.md, including the 2026-09-03 dialogue promotion entry and 2026-09-02 cross-patch resolver entry.
- Current production source and unified build script.
- Canonical Steam 2.0.4 runtime log: STALKER2UltrawideFix.log.
- A0 scope lock: RELEASE_PREPARATION_v0.5.0_A0_SCOPE.md.
- The supplied Nexus text, treated as the v0.4.0 documentation baseline.

The superseded release-preparation report, prior ZIP hash and prior READY result were not used as A1 evidence.

## Actual release delta: v0.4.0 → v0.5.0

- Added a production dialogue zoom subsystem at the current 2.0.4 live dialogue boundary.
- Added four dialogue policies: Native, Adaptive, Reduced and Disabled.
- Promoted projection-space optical zoom math calibrated from native dialogue behavior. Adaptive preserves native optical zoom strength relative to the actual gameplay baseline; Reduced applies half of that optical strength.
- Added lifecycle-aware sample transformation and EXIT anchor/recovery to remove the native EXIT discontinuity while preserving native transition timing.
- Added cinematic isolation/reset so cinematic FOV cannot contaminate the next dialogue baseline.
- Added optional configurable runtime policy hotkeys. Defaults are F9 for the next cinematic and F10 for the next dialogue; hotkeys are disabled by default and do not alter an already active lifecycle.
- Added supported key parsing for F1-F12, 0-9 and A-Z.
- Added INI template synchronization/migration while preserving supported existing values and unrelated content.
- Added static cross-patch validation of the dialogue resolver contract across Steam 2.0.2, 2.0.3 and 2.0.4.
- Preserved the existing gameplay/cinematic aspect implementation and its validated Auto/custom framing behavior.

## Evidence-to-claim matrix

| Feature or claim | Evidence | Status | Allowed public wording |
| --- | --- | --- | --- |
| Dialogue boundary | Canonical 2.0.4 runtime log; unique validated boundary at RVA 0xD20F77 | Runtime PASS | Dialogue zoom tested on Steam 2.0.4 |
| Dialogue resolver portability | Matching-image static validation on Steam 2.0.2/2.0.3/2.0.4; unique matches and instruction contract | Static PASS | Resolver statically validated across Steam 2.0.2, 2.0.3 and 2.0.4 |
| Native dialogue policy | Canonical 2.0.4 runtime matrix and visual validation | Runtime PASS | Preserves the game's native dialogue zoom |
| Adaptive policy | High-FOV runtime endpoint/visual validation near 90, 110 and 120; projection-space model | Runtime PASS | Preserves native optical zoom strength relative to actual gameplay FOV |
| Reduced policy | Optical half-strength runtime endpoint/visual validation and smooth lifecycle recovery | Runtime PASS | Applies half of the Adaptive optical zoom strength |
| Disabled policy | Runtime cycles holding the captured gameplay baseline through dialogue | Runtime PASS | Keeps gameplay FOV during dialogue |
| EXIT recovery | Runtime visual validation after the native EXIT discontinuity; transformed recovery returned smoothly to baseline | Runtime PASS | Lifecycle-aware smooth dialogue recovery |
| Cinematic/dialogue isolation | Runtime regression after cinematic recovery; correct dialogue baseline captured; no dialogue transform during cinematic | Runtime PASS | Dialogue state is isolated from cinematic lifecycle |
| ADS specificity | ADS-only control produced zero dialogue-boundary hits in the tested scenario | Runtime PASS, bounded | Tested ADS does not activate the dialogue boundary |
| Fail-closed dialogue handling | Production source guards for identity/instruction/sample/baseline failures and native pass-through contract | Static/implementation PASS, bounded | Guarded validation fails safely to native behavior |
| Runtime policy hotkeys | Production runtime log and user testing of policy cycling/custom binding | Runtime PASS, bounded | Optional hotkeys select the policy for the next applicable lifecycle |
| INI persistence/migration | Runtime log shows direct-write fallback persistence; user tested template regeneration and custom bindings | Runtime PASS, bounded | Existing INI values are preserved and managed settings are synchronized |
| Existing gameplay/cinematics | Existing runtime regression plus promotion scope explicitly left those subsystems unchanged | Runtime PASS, bounded | Existing gameplay/cinematic behavior remains part of the tested unified fix |
| Weapon/viewmodel FOV | Current research remains unresolved/deferred | Deferred | Separate known game-side issue; not fixed by v0.5.0 |

## Reference and attribution classification

- Sota reference archive: semantic/data-layer reference for DialogFOVDefault; not copied or installed.
- WIDEBOY Fixes by BigChenga: runtime-boundary research reference that helped identify the dialogue call boundary; the production implementation was independently reverse engineered and runtime validated.
- Lyall's STALKER2Tweak: existing code/scaffolding attribution remains governed by THIRD_PARTY_NOTICES.md.

## Runtime and compatibility boundaries

- Runtime evidence is limited to the tested Steam 2.0.4 executable with game SHA-256 2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409.
- Static resolver portability on Steam 2.0.2/2.0.3/2.0.4 is not runtime compatibility proof for the older builds.
- Future patches require a fresh identity gate and resolver/runtime validation.
- Hotkey changes apply to the next corresponding cinematic/dialogue, not an active one. Manual INI changes require a game restart.

## A1 gate

Every promoted v0.5.0 feature in the A0 scope has a corresponding evidence entry and allowed public claim. Runtime and static-only results are separated, deferred weapon/viewmodel work is excluded, and the supplied v0.4.0 Nexus text is identified as a baseline rather than accepted release documentation.

A1 Evidence / Release Delta / Claim Matrix: PASS.

Next permitted batch: A2 — Production Binary Gate.
