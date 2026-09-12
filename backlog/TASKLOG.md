# Task Log

This document records evidence-backed research outcomes, completed implementations and exceptional repository housekeeping. Ordinary README, release-text, index and link-maintenance edits are intentionally not logged here.

## 2026-09-12 — Clarify boolean values in production INI

### Scope and non-goals

- Add plain-language explanations for `true` and `false` under the
  `[Gameplay] Enabled` and `[Hotkeys] Enabled` settings.
- Synchronize the distributed INI and the ASI-generated default template.
- No runtime logic, defaults, hooks, FOV behavior or resolver changes.

### Changed paths

- Updated `release-assets/STALKER2CameraTweaks.ini`.
- Updated the embedded INI template in
  `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- Rebuilt `STALKER2CameraTweaks.asi` and refreshed the 0.6.0 archive.
- Completed and archived `INI_BOOLEAN_HELP_TEXT_TASK_PLAN.md`.

### Validation and limits

- Build: PASS with the established Visual Studio 2022 x64 toolchain.
- New ASI SHA-256:
  `7F88F7A3547AB88D6E5358DE7E28757692DC4F651D72B6D31FD0BAC9E3351ACE`.
- Embedded template contains both boolean explanations and the v0.6.0 header.
- Production markers remain present; research/deferred markers remain absent.
- Release asset and extracted archive contain the same new ASI SHA.
- Runtime log validation on Steam 2.0.5: PASS. The new SHA was loaded, all
  production hooks initialized, and gameplay plus RecoveryStart atomic applies
  behaved as expected with no legacy staged replay.

### Status

- Completed: user-facing boolean help text and template synchronization.
- Completed: runtime sanity check of the new comment-only binary.
- Production behavior: unchanged by design.

### Patch summary

Clarified boolean configuration values for non-technical users and kept the
packaged INI synchronized with the automatically generated template.

## 2026-09-12 — Prepare v0.6.0 release package

### Scope and non-goals

- Execute the release preparation playbook for 0.6.0 through A8.
- Promote only the runtime-validated production binary and documented atomic
  gameplay/RecoveryStart behavior.
- No source, build configuration or production behavior changes.
- No publication, commit, tag, upload or native FOV bypass research.

### Changed paths

- Updated `README.md`, `RELEASE_NOTES.md`, `TESTING_AND_RESEARCH.md`,
  `GITHUB_RELEASE_BODY.md` and `NEXUS_DESCRIPTION.md` for 0.6.0 claims.
- Replaced `release-assets/STALKER2CameraTweaks.asi` with the exact
  runtime-tested production binary.
- Updated release-assets README and INI metadata.
- Created `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.6.0.zip`.
- Moved nine historical release ZIPs to
  `build-artifacts/archive/release-assets-history/` without deletion.
- Created and completed `RELEASE_0.6.0_EXECUTION_TASK_PLAN.md`, archived under
  `research/completed/`.

### Validation and Git state

- Production ASI SHA-256: `A0A5D0823C79B3A6E785F7244439402E5CED04CE37CE670F75FE8CCFE1932372`.
- Release-assets ASI and extracted archive ASI match that SHA byte-for-byte.
- Archive contains exactly: ASI, INI, README, LICENSE and third-party notices.
- A0–A8 release gates: PASS.
- Branch: `main`; HEAD: `8b9a329`.
- Working tree contains extensive pre-existing user changes and research
  artifacts; no unrelated paths were modified by this release batch.
- Release-facing `git diff --check`: PASS. Existing unrelated TASKLOG
  trailing whitespace remains untouched.

### Completed / remaining / deferred

- Completed: evidence/claims, production binary provenance, plan/archive
  classification, documentation, release assets, archive extraction and final
  consistency review.
- Remaining: user publication and any Git commit/tag approval.
- Deferred: native cinematic FOV bypass; interpolation owner remains unresolved.
- Not runtime-validated: Steam builds older than 2.0.5.

### Patch summary

Prepared the 0.6.0 release package around the exact production binary already
validated in-game, with claims limited to runtime Steam 2.0.5 evidence and
static resolver portability across Steam 2.0.2–2.0.5.

### Changelog summary

0.6.0 documents and packages the atomic gameplay correction and single
post-cinematic RecoveryStart handoff, removing the mod's secondary flick while
preserving native FOV recovery.

## 2026-09-12 — Integrate Combined Atomic Cinematic/Gameplay Handoff into production build

### Scope and non-goals

- Promote the runtime-validated Combined Atomic Cinematic/Gameplay Handoff
  behavior into the production `STALKER2CameraTweaks.asi` build.
- Replace the active staged gameplay transition with the atomic
  `1.777778 / flags 0x4` apply and use the first confirmed descending native FOV
  sample as the one-shot `RecoveryStart` trigger.
- No native FOV recovery, `CameraComponent +0x230`, physical setter
  `RVA 0x205FCC8`, cinematic FOV formula, dialogue behavior or user-selected
  gameplay FOV changes.
- No Pass1–Pass4 instrumentation, debugger code, timer/deferred replay or
  native FOV bypass in the production build.

### Changed paths

- Updated `build.cmd` to enable only the validated atomic gameplay and
  RecoveryStart handoff paths and link `bcrypt.lib`.
- Updated `src/experimental_cinematic_21_9_combined_fix_204.cpp` so the old
  staged Auto-restore branch is excluded from the atomic production build.
- Archived completed plan as
  `research/completed/PRODUCTION_COMBINED_ATOMIC_HANDOFF_INTEGRATION_TASK_PLAN.md`.
- Rebuilt `STALKER2CameraTweaks.asi`.

### Validation and Git state

- Build: PASS with Visual Studio 2022 MSVC x64 toolchain.
- Artifact: `STALKER2CameraTweaks.asi`, 1,105,408 bytes.
- Artifact SHA-256:
  `A0A5D0823C79B3A6E785F7244439402E5CED04CE37CE670F75FE8CCFE1932372`.
- Binary string checks: legacy staged/deferred/research trace markers absent;
  `AtomicReplayApplied` and `RecoveryStart` present.
- `git diff --check`: PASS for the changed source/build paths.
- Repository: `main` at `8b9a329`; working tree contains extensive pre-existing
  user changes and untracked research/build material. No unrelated paths were
  modified by this task.
- In-game production runtime validation: PASS on Steam 2.0.5.

### Completed / remaining / deferred

- Completed: production build selection now matches the validated combined
  atomic candidate behavior; old staged path is not present in the artifact;
  no native FOV writes were introduced.
- Completed: user-run production validation of normal gameplay, final
  cinematic EXIT and dialogue coexistence on the unchanged production binary.
- Deferred: native cinematic FOV bypass; its interpolation owner remains
  unresolved and outside this integration.
- Blocked: none for build delivery.

### Patch summary

Promoted the validated atomic gameplay aspect apply and first-downward-sample
RecoveryStart handoff into the production ASI build without changing native FOV
ownership or dialogue/cinematic FOV behavior.

### Changelog summary

Production candidate now uses one atomic gameplay framing correction and one
atomic post-cinematic RecoveryStart handoff, with the legacy staged `0x5`
transition removed from the production artifact. Runtime validation confirms
one natural native FOV recovery remains and the mod's secondary post-cinematic
flick is absent.

## 2026-09-01 — Reject direct ADS-register primitive transition

### Scope and non-goals

- Validate the refined read-only 2.0.4 primitive/ADS observer against the post-cinematic weapon/viewmodel correction.
- No camera, primitive or render-state writes; no production ASI changes; no `MarkRenderStateDirty` promotion.

### Changed paths

- Updated `src/weapon_viewmodel_primitive_ads_observer_204.cpp` with ADS register probes.
- Rebuilt the isolated diagnostic ASI.
- Updated `WEAPON_VIEWMODEL_FOV_POST_CINEMATIC_CAUSAL_TRACE_TASK_PLAN.md` with the runtime result.

### Validation and evidence

- Camera-writer, mesh-assignment, ADS IN and ADS OUT signatures resolved uniquely on Steam 2.0.4 and all hooks installed.
- At ADS IN/OUT markers, camera `+0x234` remained `90` and `+0x262` remained `0x1`.
- ADS register values remained stable. `RSI` had `+0x265=0x2` and parent `+0x265=0x0`; `RAX` and `RCX` did not form valid primitive/parent pairs.
- No inspected pointer or `+0x265` transition was observed at the reported correction boundary.

### Completed

- Rejected the direct ADS-register primitive transition as the causal explanation in this dataset.

### Remaining / deferred

- Actual rendered weapon/viewmodel primitive remains unresolved.
- Native primitive setter or direct render-refresh anchor requires separate bounded validation.
- `MarkRenderStateDirty` remains deferred until a concrete native refresh event is identified.

### Patch summary

Used the refined read-only observer to eliminate the shared-camera scalar and inspected ADS-register primitive-transition hypotheses.

### Changelog summary

Diagnostic-only research; stable gameplay/cinematic behavior unchanged.

## 2026-09-01 — Organize reverse-engineering research archive

### Scope and non-goals

- Separate active task plans from completed, deferred and rejected research after the published v0.4.0 release.
- Preserve all historical plans and evidence; no source, ASI, INI, release archive or build artifact changes.
- No new reverse-engineering or runtime validation.

### Changed paths

- Added `backlog/README.md` and moved six active plans into `backlog/active/`.
- Added `research/README.md`.
- Moved completed plans into `research/completed/`, deferred plans into
  `research/deferred/` and negative/blocked branches into `research/rejected/`.
- Updated `TESTING_AND_RESEARCH.md` with research progression and archive links.
- Updated workspace `AGENTS.md` to describe the unified stable release scope.
- Preserved `backlog/TASKLOG.md` at the backlog root.

### Validation and Git state

- Repository: `main`, baseline `a63c292` (`release: prepare STALKER2UltrawideFix v0.4.0`).
- Working tree contains the planned path migration and documentation changes;
  no unrelated implementation changes were observed.
- `git diff --check` passed; no runtime validation was applicable.

### Result

- Completed: active backlog and research archive are separated and indexed.
- Deferred: future curation of `research/reports/` and `research/evidence/`.
- Blocked: none.

Patch summary: reorganized 77 historical task plans without deleting research content and added navigation for active and archived work.

Changelog summary: clarified the research archive and documented the progression from gameplay writer discovery to the unified dynamic Auto policy.

## 2026-09-01 — Remove standalone cinematic FOV toggle for 0.3.1

### Scope and non-goals

- Make cinematic FOV behavior part of the selected `AspectRatio` policy.
- Migrate existing INI files by removing obsolete `Cinematics.FovCorrection`
  and its exact generated comment.
- No new RE, signature changes, gameplay algorithm changes, runtime game test,
  release upload or Git history mutation.

### Changed paths

- Updated `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- Updated public README, release notes, Nexus description and GitHub release
  body to remove the obsolete option.
- Updated generated/test and release INI files.
- Added `backlog/CINEMATIC_FOV_POLICY_CLEANUP_031_TASK_PLAN.md`.
- Rebuilt `build-artifacts/test-asi/STALKER2UltrawideFix.asi`.

### Implementation result

- `Native` now bypasses both cinematic aspect and FOV correction.
- `Auto` and forced aspect modes use their selected aspect policy for the
  existing cinematic FOV path.
- New INI files contain only `Gameplay.Enabled` and `Cinematics.AspectRatio`.
- Existing INI files are migrated in place without rewriting unrelated keys or
  comments; only the obsolete key and its exact generated comment are removed.

### Git review

- Read-only status and diff review performed; `git diff --check` passed.
- Existing staged, unstaged and untracked user changes were preserved. No Git
  staging, commit, reset, checkout or history rewrite was performed.

### Validation and limits

- Unified ASI build succeeded with the existing Visual Studio build script.
- Candidate ASI SHA-256 before the 16:9/21:9 cache correction:
  `2C58405AA9ABE6EC78B2DEB1F01B2C5DB431535AE268327FA05B8E804FD384EE`.
- Follow-up correction: preserve the cached ultrawide aspect across gameplay
  Auto restore while still accepting native 16:9 from a readable runtime
  object. Rebuild succeeded; current candidate SHA-256 is
  `6D1825FD0EE1EB8CC42D0A6D80E70A5B42788098069B40DBB4393377F246F5A5`.
- Follow-up correction: in `Auto` mode, the cinematic aspect store now uses
  the cached runtime camera aspect, matching the FOV boundary instead of
  reading the object's already-native 16:9 value after the native store.
  Rebuild succeeded; latest candidate SHA-256 is
  `C657D9EE55C1B3951441331CA8F29525DD4ACFFB5544AEDFC8DC1703A2292D82`.
- Follow-up correction: valid aspects from the gameplay/runtime camera,
  including native 16:9 after a hot resolution change, now refresh the Auto
  cache. The cinematic store remains excluded as a cache source. Build
  succeeded; follow-up candidate SHA-256 is
  `74496AD5EE4374C688410018017EFDE7A84B5151BFC1BCB65982D5728A2AD4FB`.
- Confirmed `FovCorrection` is absent from public documents and generated INI;
  source references are limited to migration and compatibility handling.
- No injected runtime test was performed in this batch. User validation of
  `Native`, `Auto` and forced framing remains required.

### Completed / remaining / deferred

- Completed: policy cleanup, INI migration implementation, documentation and
  candidate build; corrected the `Auto` 16:9 resolver so invalid zero-aspect
  values cannot reach the cinematic store; aligned the `Auto` aspect-store
  target with the runtime aspect used by cinematic FOV.
- Remaining: regression of `Native` and forced framing modes for the 0.3.1
  release decision.
- Completed: one-session hot-resolution `Auto` test across 16:9, 21:9 and 32:9
  with cinematic EXIT recovery into gameplay.
- Deferred: packaging/publishing `0.3.1`, weapon/viewmodel FOV and dynamic
  resolution behavior.

### Follow-up refinement — exclude self-authored Auto restore from cache

- Runtime evidence showed that the broad valid-aspect cache update treated the
  fix's own `Auto restore` to native 16:9 as a new runtime aspect, so later
  cinematics stayed at 16:9.
- Added source-aware suppression for that self-authored restore while keeping
  valid 16:9 from a new gameplay/runtime camera source eligible for caching.
- Rebuilt successfully; follow-up candidate SHA-256 is
  `7404D5288E8F60E925C497B055143EF97EA79798E511850DE908ABBB32376453`.
- `git diff --check` passed.

### Runtime validation — hot aspect changes

- Validated candidate SHA-256
  `7404D5288E8F60E925C497B055143EF97EA79798E511850DE908ABBB32376453` in one
  session through `16:9 → 21:9 → 32:9 → 16:9 → 21:9 → 32:9`.
- Cinematic `Auto` resolved each runtime aspect correctly: `1.77778`,
  `2.38889`, `3.55556`, then the same sequence again.
- Matching cinematic FOV values were observed: `90`, `106.688`, `126.87`.
- Each tested cinematic EXIT recovered to gameplay and re-entered the normal
  gameplay transition without a restart. User reported all tested views were
  visually correct.
- This closes the hot-resolution cache regression for the validated 2.0.4
  executable. Weapon/viewmodel FOV remains a separate known game issue.

### Policy regression — native 5120×1440 display

- Validated `Native`, forced `16:9`, forced `21:9` and forced `32:9` with the
  same candidate and game executable identity.
- `Native` correctly bypassed cinematic aspect/FOV hooks while gameplay hooks
  remained active.
- Forced cinematic modes produced `1.77778 / FOV 90`, `2.33333 / FOV 105.392`
  and `3.55556 / FOV 126.87`, respectively. The user reported all views as
  visually correct.
- Follow-up policy correction: forced `21:9` now uses canonical `3440×1440`
  aspect `2.3888889`, matching `Auto` on a real 3440×1440 display instead of
  the abstract mathematical `21/9` value.
- Rebuilt successfully; new candidate SHA-256 is
  `A763D3D275991FC2CDBC34A3CB5585FEF0A1CC13E0ECF0B2367188D123338ABF`.
- `git diff --check` passed. The previous matrix remains evidence for
  unchanged branches.
- Focused forced-`21:9` runtime validation passed: `Global cinematic ENTER`
  and `Cinematic aspect store` both used `aspect=2.38889`, with transformed
  FOV `106.688`. The user reported the view was visually correct.
- This completes the planned cinematic policy regression for the validated
  Steam 2.0.4 / UE 5.5.4 target. `Native` and forced policies are now runtime
  validated alongside the previously completed `Auto` hot-switch matrix.

### Patch summary

Removed the contradictory standalone cinematic FOV toggle and made FOV behavior
follow the selected cinematic aspect policy, with safe migration of old INIs.

### Changelog summary

Version 0.3.1 simplifies cinematic configuration: `Native` preserves native
cinematic behavior, while `Auto` and forced framing modes apply matching Hor+
FOV automatically. The 16:9 `Auto` black-screen case is fail-safe corrected.

## 2026-09-01 — Create reusable Nexus description for 0.3.0

### Scope and non-goals

- Create a reusable Nexus Mods description for `STALKER2UltrawideFix.asi`
  version `0.3.0`.
- No source, ASI, INI or release archive changes; no external Nexus upload.

### Changed paths

- Added `NEXUS_DESCRIPTION.md` with current features, configuration,
  installation, compatibility, limitations and Defender notice.
- Added `backlog/NEXUS_DESCRIPTION_0_3_0_TASK_PLAN.md` for this bounded
  documentation task.

### Git review

- Read-only Git review and `git diff --check` were performed.
- Existing staged, unstaged and untracked user changes were preserved; no Git
  staging, commit, reset, checkout or history rewrite was performed.

### Validation and limits

- Confirmed the description uses version `0.3.0`, Steam `2.0.4` and UE `5.5.4`.
- Confirmed obsolete 2.0.3/gameplay-only claims are absent except for the old
  ASI filename intentionally retained in upgrade instructions.
- The description reflects existing runtime evidence; no new build or game
  validation was performed.

### Completed / remaining / deferred

- Completed: reusable Nexus publication text.
- Remaining: copy the text into Nexus and update it there when publishing.
- Deferred: future-patch compatibility, dynamic resolution behavior and the
  separate weapon/viewmodel FOV issue.

### Patch summary

Added a maintained Nexus-ready description for the unified 0.3.0 release.

### Changelog summary

Replaced the obsolete gameplay-only publication text with current unified
gameplay, cinematic and custom framing documentation.

## 2026-09-01 — Prepare version 0.3.0 release package

### Scope and non-goals

- Prepare the user-facing `0.3.0` package for `STALKER2UltrawideFix.asi`.
- Update release documentation, include the generated INI and create a clean
  distributable archive.
- No upload to GitHub/Nexus, Git staging/commit, source changes or new runtime
  experiment was performed in this packaging task.

### Changed paths

- Updated `release-assets/README.md` for the unified gameplay/cinematic fix.
- Added `release-assets/STALKER2UltrawideFix.asi`.
- Added `release-assets/STALKER2UltrawideFix.ini`.
- Added `release-assets/STALKER2UltrawideFix-UE5.5.4-v0.3.0.zip`.
- Updated `backlog/RELEASE_0_3_0_TASK_PLAN.md` with completed batch status.

### Package contents

The archive contains exactly:

- `STALKER2UltrawideFix.asi`
- `STALKER2UltrawideFix.ini`
- `README.md`
- `LICENSE.md`
- `THIRD_PARTY_NOTICES.md`

The historical `STALKER2GameplayAspectFix-2.0.2-v0.1.0-elhait.zip` remains
untouched and is not part of the new package.

### Git review

- Read-only Git review was performed after packaging.
- Existing staged, unstaged and untracked user changes were preserved; no
  staging, commit, reset, checkout or history rewrite was performed.
- The changed release and task-log paths were compared with the approved plan;
  unrelated pre-existing worktree changes were left untouched.

### Validation and limits

- Archive creation succeeded and its exact five-file contents were verified.
- `git diff --check` passed for the reviewed worktree.
- SHA-256: ASI
  `949B61998A49FB04276D91B64BC5D3F087989999CA2779ACD8C703E97DBF7607`.
- SHA-256: INI
  `C74C4E3383A2A539FDF296AF980D2F474C03F37537AE40D7D07F1D6CEAC79C1C`.
- SHA-256: archive
  `53708362200137C95E0EC4D3B2D566CDB1596877042093185A38498F9E424F82`.
- This batch did not rebuild or inject the ASI; the package reflects the
  previously validated Steam 2.0.4 runtime evidence.

### Completed / remaining / deferred

- Completed: README update, INI inclusion, versioned archive and package
  inspection.
- Remaining: user upload/publication to GitHub/Nexus.
- Deferred: weapon/viewmodel FOV issue, dynamic resolution policy and support
  validation for future game patches.
- Not changed: historical release archive, Git history and unrelated staged
  user work.

### Patch summary

Prepared the `0.3.0` release package for the unified configurable
`STALKER2UltrawideFix.asi`, including the default INI and user-facing
installation/compatibility documentation.

### Changelog summary

Version `0.3.0` packages gameplay ultrawide correction, cinematic aspect/FOV
support and custom cinematic framing modes for the validated UE 5.5.4 / Steam
2.0.4 environment.

## 2026-09-01 — Implementation history before `STALKER2UltrawideFix`

### Scope and intent

This entry records the evidence and implementation path leading to the unified
`STALKER2UltrawideFix.asi`. It is based on the repository diff from
`HEAD 5d3d157` (`Create FUNDING.yml`), the completed task plans, Ghidra
reports and user-supplied 2.0.4 runtime logs. The working tree contains
intentional staged, unstaged and untracked user work; this entry does not
normalize or remove it.

### Diff inventory

The read-only diff against `HEAD` contains 223 changed paths:

- 48 C++ source files, including stable gameplay code, the unified source and
  separate diagnostic/research artifacts;
- 63 task plans and status documents;
- 56 test build scripts;
- 53 object/build outputs;
- documentation and supporting project files.

The large diff is a combined research/build snapshot, not a claim that every
path belongs in the release package. The production-relevant implementation
areas are `src/gameplay_aspect_fix.cpp`, the unified
`src/experimental_cinematic_21_9_combined_fix_204.cpp`, its build script and
the generated `STALKER2UltrawideFix.ini`. `src/cutscene_letterbox_fix.cpp`
and the other diagnostic sources remain research history or isolated test
artifacts.

### Work completed before the unified implementation

#### 1. Stable gameplay baseline

- Established the existing gameplay aspect correction and its two-pass
  constrained/Auto-restore contract.
- Confirmed preservation of the player's selected FOV.
- Replaced the original fixed-RVA assumption with a unique executable `.text`
  signature resolver and Zydis validation of `MOVSS [RBX+0x30], XMM0`.
- Preserved fail-closed behavior for ambiguity, decode failure, invalid
  instruction form and hook setup failure.
- Investigated the 21:9 startup/death-load regression. Runtime evidence showed
  that the old 32:9-only predicate rejected the actual 21:9 aspect
  `2.38889`.
- Generalized the predicate to finite aspects wider than native 16:9 and kept
  the observed source aspect through the constrained pass.
- User runtime-tested 21:9 startup, `21:9 → 16:9 → 21:9`, death/load rebuild
  and 32:9 regression successfully.

#### 2. Early cinematic and FOV research

- Reconstructed the legacy 2.0.3 cinematic transition and letterbox/FOV
  boundaries, including the shared ENTER/EXIT topology.
- Tested and rejected durable `+0x230`/state-based, delayed, timer, polling,
  generic dispatcher, interpolation-shape and broad renderer approaches.
- Used read-only runtime correlation to reject unrelated static candidates,
  including `FUN_1422FC35A`, `FUN_1405EDA3A`, `FUN_1431FA182`,
  `FUN_14027A5E4`, `FUN_146880C06`, `FUN_1404A4CCE`, `FUN_142D08BB0` and
  `FUN_1476C41A6`.
- Closed the scalar-shape ranking method after demonstrating that structural
  FP similarity did not establish live cinematic FOV provenance.

#### 3. Current-build live FOV recovery

- Mapped legacy transition hub `FUN_142EE14BC` to the strong 2.0.4 descendant
  `FUN_142EE68DA` through shared helpers, resolver topology and exact
  direction-specific vtable slot pairs.
- Confirmed the current ENTER boundary where authored `XMM0=90.0` reaches the
  shared live-FOV consumer and the EXIT boundary where `[RDI+0x38]` carries the
  native gameplay target.
- Runtime evidence confirmed both source patterns on the same transition
  context and thread.
- A read-only feasibility test transformed ENTER FOV `90.0` to
  `126.869896` for 32:9 and produced the expected wider cinematic result,
  without durable camera-state writes.

#### 4. Current-build aspect recovery

- Reconstructed the legacy aspect/letterbox role and identified the exact
  current ENTER native store equivalent of `RVA 0x6B7CB05`.
- A read-only provenance hook confirmed the store executes with `RAX` equal to
  the authoritative cinematic inner and the expected pre-store lifecycle
  flags.
- Immediate-patch feasibility replaced only the native `1.7777778` immediate
  with runtime `3.5555556` at 5120x1440, preserving native control flow and
  lifecycle side effects.
- The aspect source was corrected from desktop dimensions to the runtime game
  camera aspect after discovering that a 5120x1440 desktop can contain a
  3440x1440 game window.

#### 5. Combined cinematic integration research

- Combined the already validated aspect immediate and transient live-FOV
  boundaries with the generalized gameplay correction in isolated artifacts.
- User runtime-tested correct 21:9 and 32:9 cinematic framing, including
  2560x1440 with forced 32:9 letterbox framing.
- Investigated the post-EXIT seam. Native FOV recovery and gameplay B/C were
  both confirmed necessary; atomic same-invocation B/C scheduling did not
  remove the visible intermediate presentation state. That coordinator path
  was closed as a production solution and is not presented as solved.
- Kept the native EXIT recovery, gameplay algorithm and stable gameplay source
  contract intact.

#### 6. Unified configuration and identity

- Added automatic creation of `STALKER2UltrawideFix.ini`.
- Added independent gameplay enablement and cinematic policy configuration.
- Replaced cinematic boolean aspect control with `AspectRatio=Auto`,
  `Native`, `16:9`, `21:9` or `32:9`, while retaining independent
  `FovCorrection`.
- Preserved legacy `AspectFix`/`FovFix` parsing as compatibility fallback.
- Added startup SHA-256 logging for the loaded ASI and game executable.
- Corrected the file reader after the first `unavailable` result; a subsequent
  runtime log confirmed uppercase hashes and successful resolver installation.
- Removed high-frequency coordinator recovery/suppression spam while keeping
  state behavior unchanged.

### Resulting implementation state

The unified artifact now contains:

- signature-resolved gameplay camera correction;
- runtime-camera cinematic aspect policy and native immediate replacement;
- signature-resolved cinematic ENTER/EXIT live-FOV transform;
- coordinator isolation between cinematic lifecycle and gameplay replay;
- automatic INI creation and policy parsing;
- startup mod/game SHA-256 identity logging;
- fail-closed resolver and hook setup behavior.

### Validation and limits

- Build validation succeeded for the unified ASI and its test output.
- User runtime evidence confirms the unified 2.0.4 path at 21:9 and 32:9,
  including forced cinematic framing modes and gameplay re-entry behavior.
- The tested Steam game executable identity was
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Signature resolution is more resilient than fixed RVAs but does not itself
  guarantee a future patch. No 2.0.5 support claim is made.
- The weapon/viewmodel FOV-after-load issue remains a separate known game-side
  issue.
- The brief post-cinematic presentation seam remains a known limitation of the
  experimental cinematic integration.

### Completed / remaining / deferred

- Completed: evidence-led gameplay correction, cinematic boundary recovery,
  unified 2.0.4 implementation, configuration, identity logging and supplied
  runtime validation.
- Remaining: final release packaging and user-facing release documentation.
- Deferred: weapon/viewmodel FOV research, dynamic resolution policy and fresh
  validation for any executable newer than 2.0.4.
- Not changed by this documentation record: source behavior, build outputs,
  Git staging and commit history.

### Patch summary

The unified ASI was built only after separate gameplay and cinematic boundary
reconstruction, runtime validation, rejection of unsafe alternatives and
resolution of the 21:9 lifecycle regression. The final implementation combines
the validated mechanisms without the earlier desktop-aspect or fixed-RVA
assumptions.

### Changelog summary

Documented the complete path from the stable gameplay resolver and bounded
cinematic research to the configurable `STALKER2UltrawideFix` implementation,
including evidence limits and deferred issues.

## 2026-09-01 — Reconcile completed backlog and testing summary

### Scope

- Review every task plan in `backlog`.
- Move only completed or explicitly closed plans into `backlog/complete/`.
- Update `TESTING_AND_RESEARCH.md` with the confirmed 2.0.4 gameplay,
  cinematic, configuration and compatibility status.
- No source, build, ASI, INI or runtime behavior changes.

### Changed

- Moved 32 completed/closed task plans to `backlog/complete/`.
- Added `backlog/COMPLETE_BACKLOG_AND_TESTING_SUMMARY_TASK_PLAN.md`.
- Reworked `TESTING_AND_RESEARCH.md` to document the unified
  `STALKER2UltrawideFix.asi`, its policy configuration, 2.0.4 evidence,
  signature limits, known issues and release checklist.
- Preserved active, pending, deferred and unresolved plans in `backlog/`.

### Git review

- Read-only status review performed after the archival move.
- Existing staged and untracked user changes were preserved; no Git staging,
  commit, reset or history rewrite was performed.
- The moved plans appear as staged/deleted plus untracked archived copies where
  the worktree already contained user-staged additions; this was not altered.

### Validation and limits

- Confirmed 32 exact source plans moved without destination collisions.
- Confirmed 41 task plans now exist in `backlog/complete/` and 31 remain as
  active or deferred plans in `backlog/`.
- Checked the updated summary against supplied runtime evidence and existing
  task plans.
- `git diff --check` passed for the repository worktree.
- No build, game launch, ASI injection or new runtime validation was performed.

### Completed / remaining / deferred

- Completed: backlog classification, archival of completed/closed plans and
  testing/research summary update.
- Remaining: active plans in `backlog/`, including unresolved projection,
  pending signature follow-up, post-EXIT handoff classification and logging
  cleanup runtime confirmation.
- Deferred: weapon/viewmodel FOV research and future-patch compatibility.
- Not runtime-validated by this documentation batch: no new implementation or
  runtime behavior.

### Patch summary

Archived completed research and implementation plans and synchronized the
project testing summary with the validated 2.0.4 state.

### Changelog summary

The backlog now separates completed/closed work from active research, and the
testing summary documents the unified configurable gameplay/cinematic fix.

## 2026-08-30 — Runtime-informed top-tier semantic reranking

### Scope

- Re-rank the retained 2.0.4 top-500 using completed runtime negatives.
- Decompile-review the tied top tier and the `FUN_14569...` family.
- No runtime artifact, game launch, hook, write or stable/global ASI change.

### Changed

- Added the reranking plan, helper and generated report.
- Added a read-only Ghidra decompile helper and top-tier review report.

### Git review

- Existing uncommitted research/source/build-artifact changes were preserved.
- Stable gameplay and global experimental ASI files were not modified.
- Recent HEAD at review: `5d3d157 Create FUNDING.yml`.

### Validation and limits

- Existing 2.0.4 inventory was reused; no full executable re-analysis was run.
- Top-500 input was deduplicated to `351` function/entry rows.
- Top-tier decompilation ran read-only against the verified 2.0.4 program hash.
- Static review does not establish runtime ownership.

### Completed

- Initial shortlist was closed and runtime-informed ranking completed.
- Most tied candidates were deprioritized as subsystem-specific, large/generic
  or lacking lifecycle semantics.
- Held shortlist: `FUN_14027A5E4`, `FUN_146880C06`, and the `FUN_14569...`
  family represented by `FUN_14569CAD2`.

### Remaining / deferred

- No candidate is runtime-promoted; each held candidate requires a separate
  read-only plan before testing.
- Native cinematic FOV transition owner remains unresolved.

### Patch summary

Applied runtime-informed penalties and completed semantic review of the tied
top tier without expanding into runtime instrumentation.

### Changelog summary

Initial static-ranked shortlist was replaced by a smaller held shortlist based
on subsystem and lifecycle plausibility; no production code changed.

## 2026-08-30 — `FUN_1431FA182` runtime correlation gate

### Scope

- Run one isolated, read-only 2.0.4 runtime correlation pass for
  `FUN_1431FA182`.
- No game-state writes, correction logic, stable gameplay changes or global
  experimental ASI changes.

### Changed

- Added the candidate-specific task plan and runtime evidence review under
  `02-Research/Ghidra/reports`.

### Git review

- Existing uncommitted research/source/build-artifact changes were preserved.
- Stable gameplay and global experimental ASI files were not modified.
- Recent HEAD at review: `5d3d157 Create FUNDING.yml`.

### Validation and limits

- User-supplied runtime log shows successful installation and valid ENTER/EXIT
  anchors on the same `inner=0x27B7E02A6C0` and thread `19768`.
- Candidate hit count was zero over approximately `12.4 s`; no write-test was
  performed.

### Completed

- `FUN_1431FA182` was rejected for the tested cinematic lifecycle.
- The initial three-candidate runtime shortlist is exhausted.

### Remaining / deferred

- Native cinematic FOV transition owner remains unresolved.
- Runtime-informed reranking of the top-500 requires a separate plan.

### Patch summary

Recorded a clean read-only negative runtime verdict for `FUN_1431FA182`.

### Changelog summary

`FUN_1431FA182` was silent during the validated cinematic lifecycle and was
rejected without modifying production or correction code.

## 2026-08-30 — `FUN_1405EDA3A` runtime correlation gate

### Scope

- Run one isolated, read-only 2.0.4 runtime correlation pass for the static
  reserve candidate `FUN_1405EDA3A`.
- No game-state writes, correction logic, stable gameplay changes or global
  experimental ASI changes.

### Changed

- Added the candidate-specific task plan and runtime evidence review under
  `02-Research/Ghidra/reports`.

### Git review

- Existing uncommitted research/source/build-artifact changes were preserved.
- Stable gameplay and global experimental ASI files were not modified by this
  pass.
- Recent HEAD at review: `5d3d157 Create FUNDING.yml`.

### Validation and limits

- User-supplied runtime log shows successful installation and valid ENTER/EXIT
  anchors on the same `inner=0x2D701A50100` and thread `7188`.
- Candidate hit count was zero over approximately `11.55 s`; no write-test was
  performed.

### Completed

- `FUN_1405EDA3A` was rejected for the tested cinematic lifecycle.

### Remaining / deferred

- `FUN_1431FA182` remains the next reserve candidate, pending a separate plan.
- Native cinematic FOV transition owner remains unresolved.

### Patch summary

Recorded a clean read-only negative runtime verdict for `FUN_1405EDA3A`.

### Changelog summary

`FUN_1405EDA3A` was silent during the validated cinematic lifecycle and was
rejected without modifying production or correction code.

## 2026-08-27 — Close cinematic constrained-vs-native projection state diff research

### Scope

- Analyze the 2.0.3 Ghidra project on the durable E: path.
- Compare the confirmed `16:9 / 2560x1440 / correct framing` and `32:9 / 5120x1440 / incorrect framing` cinematic states.
- Validate only `FUN_140186BE8` as the remaining projection-like candidate.
- No stable gameplay changes, A/B changes, runtime tracer, build or source implementation.

### Changed

- Added and updated `backlog/CINEMATIC_CONSTRAINED_NATIVE_PROJECTION_STATE_DIFF_TASK_PLAN.md`.
- Added read-only Ghidra helpers under `02-Research/Ghidra/ghidra-scripts` and stored analysis logs under `02-Research/Ghidra/workspace`.
- No Ghidra project data or stable ASI source was intentionally modified.

### Git review

- Canonical repository: project root.
- Branch/HEAD: `main@179222d` (`chore: organize build artifacts and research workflow`).
- Read-only Git review performed with an explicit per-command `safe.directory` override; global Git configuration was not changed.
- Existing user modifications and untracked research/build artifacts remain untouched.

### Validation and limits

- Executable: game 2.0.3, SHA-256 `81961B7281C7CF528CE49C549CE086FCC684BD676F32FAF042BC743D939E3C69`.
- `FUN_140280F58`, `FUN_140311CB4` and `FUN_14038E760` were rejected as graph/lighting/resource paths.
- `FUN_1401BD890` consumes mode/flag bytes but does not read the runtime aspect field.
- `FUN_14010C076` is a constructor/default initializer, not a transition owner.
- `FUN_140186BE8` builds matrix-like data, but caller/data-flow evidence ties it to movable-point-light/shadow and `ViewSpace...` scene resources, not cinematic camera ownership.
- No runtime validation or implementation was performed; this is a bounded static negative result.

### Completed / remaining / deferred

- Completed: Batch 1 state-family diff and the approved single-candidate ownership validation.
- Remaining: none within the approved bounded scope.
- Deferred: any new cinematic research requires new structural evidence and a separate approved plan.
- Blocked: no validated cinematic projection-only or refresh intervention point was found.
- Not runtime-validated: all conclusions in this entry are static-analysis conclusions.

### Patch summary

Completed a bounded 2.0.3 Ghidra ownership review and rejected the remaining projection-like candidate as unrelated scene/shadow processing.

### Changelog summary

Closed the constrained-vs-native cinematic projection research task as blocked without changing the stable gameplay fix.

## 2026-08-27 — Reject transient/durable FOV split experiment

### Scope

- Approved experimental A/B test of transient cinematic Hor+ FOV versus durable camera-state FOV.
- Determine whether cinematic framing can remain correct when `state + 0x54` receives the original source FOV.
- Stable gameplay ASI and release behavior were out of scope.

### Changed

- Added a diagnostic-only split-state branch and dedicated build script under the existing experimental workflow.
- Built Variant A and Variant B diagnostic ASIs for manual comparison.
- Updated `backlog/CUTSCENE_TRANSIENT_DURABLE_FOV_SPLIT_TASK_PLAN.md` with the completed result.

### Git review

- Canonical repository: project root.
- Branch/HEAD: `main@179222d` (`chore: organize build artifacts and research workflow`).
- Read-only review performed; no Git state was mutated.
- Existing diagnostic source, object files and untracked research plans remain visible in the working tree.

### Validation and limits

- Both diagnostic variants compiled successfully.
- Variant B confirmed transient `126.869896` with durable `state + 0x54 = 90`.
- Gameplay logging after the cutscene recorded `primaryFOV=90`, not `126.869896`.
- Manual testing showed that Variant B visually resembled gameplay without `STALKER2GameplayAspectFix`: the large gameplay-visible FOV contamination was removed, but the required cinematic Hor+ framing was lost.
- This does not identify the exact final projection consumer or provide a release-ready solution.

### Completed / remaining / deferred

- Completed: transient/durable split mechanism tested and rejected as final architecture.
- Remaining: find a solution that preserves cinematic Hor+ without injecting the transformed value into gameplay-visible camera state.
- Deferred: further implementation and projection-owner research until a new concrete direction is approved.
- Not runtime-validated: no release build or compatibility claim was made.

### Patch summary

Tested and rejected a split-state FOV design: it prevents the `126.87` gameplay leak but loses the required cinematic Hor+ framing.

### Changelog summary

Closed the split-state experiment with causal evidence that the transformed FOV must pass through camera-state machinery for the observed cinematic framing.

## 2026-08-27 — Reject cutscene FOV durable state as causal blend owner

### Scope

- Approved bounded static/runtime research of the established cutscene FOV state path.
- Determine whether `state + 0x54` is consumed as an active post-cutscene blend or projection source.
- No source behavior, ASI output, workaround, delayed replay or broad scan changes.

### Changed

- Updated `backlog/CUTSCENE_FOV_STATE_RUNTIME_TRACE_TASK_PLAN.md` with the final ownership-bounded result.
- Recorded that `FUN_1431D2094` reads `state + 0x54` only for equality/update checking.
- Recorded that the runtime-derived virtual target at executable RVA `0x20939B8` does not read `+0x54`.

### Git review

- Canonical repository: project root.
- Branch/HEAD: `main@179222d` (`chore: organize build artifacts and research workflow`).
- Read-only review performed; no Git state was mutated.
- Existing source, object files and untracked research plans remain outside this documentation batch.

### Validation and limits

- Re-ran the bounded read-only Ghidra inspection against the 2.0.3 program in `Dump/STALKER2-Ghidra`.
- Confirmed the durable `+0x54` write and the existing marker-correlated runtime state handoff evidence.
- Confirmed no downstream blend/projection reader was established in the known state family.
- This result does not identify the final camera handoff owner and does not validate a new implementation.

### Completed / remaining / deferred

- Completed: `state + 0x54` durable-state path rejected as a validated causal blend owner.
- Remaining: determine a safer architecture for preventing transformed cinematic FOV from affecting the subsequent camera handoff.
- Deferred: further blend-owner search, implementation and compatibility claims.
- Not runtime-validated: no new implementation runtime test was performed in this batch.

### Patch summary

Closed the bounded cutscene FOV state investigation after confirming durable state handoff but finding no active downstream blend consumer.

### Changelog summary

Rejected the `state + 0x54` persistence path as an implementation basis; future work must use a separate, evidence-backed camera handoff design.


## 2026-08-26 — Trim vendored dependency material and add notices

### Scope

- Approved cleanup of vendored dependency contents and third-party notices.
- Keep the dependencies vendored; no submodule migration.
- Preserve current source/build behavior and release assets.

### Changed

- Retained `external/spdlog/include/` and `external/spdlog/LICENSE`.
- Removed unused spdlog examples, tests, benchmarks, CMake/CI files, scripts, logos and metadata outside the retained paths.
- Preserved `external/safetyhook/` and bundled Zydis unchanged.
- Added SafetyHook BSL-1.0, Zydis MIT and spdlog MIT provenance/license notes to `THIRD_PARTY_NOTICES.md`.
- Closed `backlog/complete/DEPENDENCIES_CLEANUP_TASK_PLAN.md`.

### Git review

- Canonical repository: project root.
- Branch/HEAD: `main@f8b6603` (`Add files via upload`).
- Read-only review performed; no Git state was mutated.
- `release-assets/` and C++ source were untouched.

### Validation and limits

- Representative `build-transition-trace.cmd` completed successfully from `build-artifacts/test-scripts/`.
- Confirmed retained spdlog headers were sufficient for compilation and output remained in the test artifact folders.
- No game launch, ASI injection or runtime validation was performed.

### Completed / remaining / deferred

- Completed: dependency trim and licensing/provenance documentation.
- Remaining: none for this cleanup batch.
- Deferred: optional future submodule migration remains a separate task.

### Patch summary

Reduced vendored spdlog to build-required headers and license text while documenting all retained third-party dependencies.

### Changelog summary

Removed unused dependency project material and added clear SafetyHook, Zydis and spdlog notices.

## 2026-08-26 — Update testing and research summary

### Scope

- Approved documentation-only update based on completed task plans and established runtime evidence.
- Correct stale fixed-RVA/source claims and document current stable, experimental and deferred research status.
- No source, build script, release asset or runtime behavior changes.

### Changed

- Rewrote `TESTING_AND_RESEARCH.md` to document the dynamic gameplay resolver, 5120×1440 runtime observations, failed letterbox validation and rejected gameplay-camera state branch.
- Preserved the distinction between confirmed evidence, rejected paths, unresolved weapon/viewmodel ownership and executable/session limits.
- Added and closed `backlog/complete/TESTING_AND_RESEARCH_TASK_PLAN.md`.

### Git review

- Canonical repository: project root.
- Branch/HEAD: `main@f8b6603` (`Add files via upload`).
- Only the approved documentation and task-log paths were changed in this batch; no Git state was mutated.
- `git diff --check` passed.

### Validation and limits

- Checked the updated summary against the completed plans in `backlog/complete/`.
- Confirmed obsolete `RVA 0x00AF3A17` and fixed-RVA framing were removed.
- No build, game launch, ASI injection or new runtime validation was performed.

### Completed / remaining / deferred

- Completed: testing/research summary aligned with current evidence.
- Remaining: none for this documentation batch.
- Deferred: separate research task for the weapon/viewmodel object or downstream projection owner.

### Patch summary

Replaced stale testing notes with an evidence-based summary of the dynamic gameplay resolver, runtime state-diff result and experimental letterbox status.

### Changelog summary

Updated testing and research documentation to reflect current validated behavior and clearly bounded unresolved work.

## 2026-08-26 — Organize test build scripts and diagnostic ASI outputs

### Scope

- Approved cleanup of root test build scripts and diagnostic `.asi` files.
- Keep only the main `build.cmd` in the project root.
- Preserve release assets and leave `TESTING_AND_RESEARCH.md` unchanged.

### Changed

- Moved six diagnostic `build-*.cmd` scripts to `build-artifacts/test-scripts/`.
- Moved root diagnostic ASI files to `build-artifacts/test-asi/`.
- Updated moved test scripts to resolve source/dependency paths from the project root and write `.obj` files to `build-artifacts/obj/` and `.asi` files to `build-artifacts/test-asi/`.
- Kept the existing global `*.asi` ignore rule; no redundant folder-specific rule was added.
- Closed `backlog/TEST_BUILD_ARTIFACTS_TASK_PLAN.md` with the verified outcome.

### Git review

- Canonical repository: project root.
- Branch/HEAD: `main@f8b6603` (`Add files via upload`).
- Read-only review confirmed only `build.cmd` and no `.asi` remain in the project root; `release-assets/` was untouched.
- `git diff --check` passed; no Git state was mutated.
- Pre-existing staged diagnostic sources, dependencies and related workspace changes remain outside this cleanup scope.

### Validation and limits

- `build-transition-trace.cmd` succeeded from its new location.
- Verified `.obj` output in `build-artifacts/obj/` and `.asi` output in `build-artifacts/test-asi/`.
- No game launch, ASI injection or runtime validation was performed.

### Completed / remaining / deferred

- Completed: test script and diagnostic ASI organization with path-preserving build correction.
- Remaining: none for this cleanup batch.
- Deferred: release packaging and stable `build.cmd` output policy remain separate tasks.

### Patch summary

Separated diagnostic build scripts and ASI outputs from the project root while preserving their build paths and the existing global ignore policy.

### Changelog summary

Kept the project root focused on the main build entry point and moved test artifacts into dedicated directories.

## 2026-08-26 — Organize task plans and root build artifacts

### Scope

- Approved cleanup of root-level `.obj` files and project task plans.
- Preserve all artifacts and plan contents; do not change source behavior, release assets or Git history.
- Keep `TESTING_AND_RESEARCH.md` unchanged for a later documentation task based on `backlog/complete`.

### Changed

- Moved eight root `.obj` files to `build-artifacts/obj/`.
- Removed the project-local `*.obj` rule from `.gitignore`.
- Moved four completed plans to `backlog/complete/`.
- Added English status lines and closure reasons to the completed plans.
- Added `backlog/BACKLOG_ORGANIZATION_TASK_PLAN.md` with status `Closed` and a closure reason.
- Updated direct references affected by plan moves.

### Git review

- Canonical repository: project root.
- Branch/HEAD: `main@f8b6603` (`Add files via upload`).
- Read-only review confirmed no root-level `.obj` or `*_TASK_PLAN.md` remains.
- `git diff --check` passed; no Git state was mutated.
- Pre-existing staged diagnostic sources, dependencies and related workspace changes remain outside this cleanup scope.

### Validation and limits

- Confirmed all eight `.obj` files exist in `build-artifacts/obj`.
- Confirmed all four completed plans exist in `backlog/complete` and the cleanup plan exists in `backlog`.
- Confirmed `.gitignore` no longer ignores `.obj`.
- No build or runtime validation was required or performed.

### Completed / remaining / deferred

- Completed: bounded artifact and task-plan organization.
- Remaining: none for this cleanup batch.
- Deferred: update `TESTING_AND_RESEARCH.md` from completed plans as a separate documentation task.

### Patch summary

Moved reusable object files and completed task plans into dedicated folders, normalized plan statuses and removed the obsolete `.obj` ignore rule.

### Changelog summary

Improved project navigation by separating build artifacts from source and maintaining a backlog with a completed-plan archive.

## 2026-08-26 — Reject gameplay-camera state as weapon FOV causal owner

### Scope

- Approved research-only ADS runtime state-diff and marker validation.
- Compare the bounded 12-field gameplay-camera context across cutscene exit, ADS and pause transitions.
- No stable gameplay logic, experimental letterbox logic, memory writes or game-function calls.

### Changed

- Updated `src/gameplay_transition_trace.cpp` with five manual markers: F8 cutscene exit, F9/F10 ADS enter/exit and F11/F12 pause open/close.
- Updated `backlog/complete/ADS_STATE_DIFF_TASK_PLAN.md` with the completed batch result and next phase gate.
- Built `STALKER2GameplayTransitionTrace.asi`; runtime log was written outside the repository in the game's Win64 directory.

### Git review

- Canonical repository: project root.
- Branch/HEAD: `main@f8b6603` (`Add files via upload`).
- Working tree contains pre-existing diagnostic sources, plans and vendored dependencies; this batch changed only the tracer source and its task plan.
- No Git state was mutated.

### Validation and limits

- Build succeeded and produced `STALKER2GameplayTransitionTrace.asi`.
- Runtime log loaded the tracer and recorded all five markers: sequences 1903, 2119, 2425, 2526 and 2557.
- Across the marker windows, the 12 sampled gameplay-camera values showed no distinct state pattern associated with ADS or pause correction; `outputFov == inputFov` throughout.
- Runtime evidence is limited to the tested executable/session and does not identify the weapon/viewmodel owner.

### Completed / remaining / deferred

- Completed: gameplay-camera state branch rejected as the causal owner by marker-correlated runtime evidence.
- Remaining: find the separate weapon/viewmodel object or downstream projection context.
- Deferred: ownership tracing and any implementation; no compatibility claim is made.
- Not runtime-validated: no new letterbox or stable gameplay implementation was tested in this batch.

### Patch summary

Added precise manual event markers to the bounded runtime sampler and used them to exclude the known gameplay-camera context from the weapon/viewmodel FOV causal path.

### Changelog summary

Closed the gameplay-camera state investigation with a reproducible negative runtime result; future research starts from a separate weapon/viewmodel or projection context.

## 2026-08-25 — Remove obsolete publishing documents

### Scope

- Approved cleanup of two obsolete project-local publishing documents.
- No source, runtime, release archive, `HANDOFF.md`, `AGENTS.md` or Git history changes.

### Changed

- Removed `PUBLISHING_CHECKLIST.md`.
- Removed `NEXUS_DESCRIPTION.md`.

### Git review

- Canonical repository: project root.
- Branch: `main`, still tracking `origin/main`.
- Cleanup appears as two expected deleted paths.
- The pre-existing `RELEASE_NOTES.md` documentation change remains separate and was not removed.

### Validation and limits

- Confirmed both exact target paths no longer exist.
- `git diff --check` passed; only a normal LF-to-CRLF warning was reported.
- No build, game launch, ASI load or runtime validation performed.

### Completed / remaining

- Completed: obsolete publishing and Nexus description files removed.
- Remaining: none for this cleanup batch.

### Patch summary

Removed two unused project-local publishing documents while preserving source and release assets.

### Changelog summary

Cleaned obsolete publishing metadata from the canonical project tree.

## 2026-08-25 — Align documentation contract with canonical v0.1.1 source

### Scope

- Documentation-only follow-up to Task 1 static/source verification.
- Align the stable contract and project/release documentation with canonical `main@dad8176`.
- Keep `HANDOFF.md` as historical context; do not alter source, resolver code, release assets or Git refs.

### Changed

- Updated `AGENTS.md` to describe the implemented `.text` signature resolver and its decode/safe-refusal contract.
- Corrected project release documentation from UE 5.5.6 to the supported UE 5.5.4 wording.
- Updated `PUBLISHING_CHECKLIST.md` for v0.1.1 and marked the retained v0.1.0 archive as historical.

### Git review

- Canonical repository: project root.
- Branch/commit before documentation changes: `main@dad8176`.
- Working tree was clean before this batch; source and Git refs were not changed.

### Validation and limits

- Confirmed resolver provenance in canonical `src/gameplay_aspect_fix.cpp`.
- Checked documentation references for the obsolete fixed-RVA/v0.1.0 contract and UE 5.5.6 claims.
- No build, ASI load, game launch or runtime validation performed.

### Completed

- Documentation contract now matches the current resolver source at the stated Git identity.
- Task 1 remains complete at static/source evidence level only.

### Remaining / deferred / blocked

- Runtime operation and compatibility across game builds remain unvalidated.
- The retained release archive remains historical v0.1.0 packaging.

### Patch summary

Aligned stable and release documentation with the canonical v0.1.1 dynamic resolver without changing implementation or runtime state.

### Changelog summary

Corrected resolver provenance, UE version wording and v0.1.1 publishing guidance.

## 2026-08-25 — Restore post-change Git review workflow

### Scope

- Restore the rule that every approved change is followed by a read-only Git review and an implementation-vs-plan comparison.
- Define a stable task-log format for future patch and changelog writing.

### Changed

- Updated `AGENTS.md` with the post-change Git review and task-log workflow.
- Added this root `TASKLOG.md` as the workspace task-log location.

### Git review

- Canonical repository: project root.
- Branch: `main`.
- Git state: existing user Markdown changes and handoff history remain uncommitted; no Git state was mutated for this task.
- Source, build outputs and release assets were not changed.

### Validation and limits

- Confirmed the new rules are present in `AGENTS.md`.
- This task did not build the ASI, launch the game or perform runtime hook validation.

### Completed

- Post-change `git status`/diff review is now mandatory before task closure.
- Future entries must distinguish completed, remaining, deferred, blocked and not-runtime-validated work.

### Remaining / deferred

- No source implementation work was requested in this task.
- The canonical source remains subject to the version and resolver limitations documented in `AGENTS.md` and `HANDOFF.md`.

### Patch summary

Restored a factual post-change Git audit and task-log workflow for the native C++/ASI workspace.

### Changelog summary

Added structured task logging for reliable patch and release-note preparation.

## 2026-08-30 — `FUN_1422FC35A` runtime correlation gate

### Scope

- Run one isolated, read-only 2.0.4 runtime correlation pass for the highest-
  ranked exploratory static candidate `FUN_1422FC35A`.
- No game-state writes, correction logic, stable gameplay changes or global
  experimental ASI changes.

### Changed

- Added the candidate-specific task plan and runtime evidence review under
  `02-Research/Ghidra/reports`.

### Git review

- Canonical repository contains pre-existing uncommitted research/source and
  build-artifact changes; they were preserved.
- This pass did not modify stable gameplay or global experimental ASI files.
- Recent HEAD at review: `5d3d157 Create FUNDING.yml`.

### Validation and limits

- User-supplied runtime log corresponds to the recorded 2.0.4 executable hash
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- The run produced `2782` hits across three candidate objects, all on the
  ENTER thread, but candidate scalar state did not track FOV convergence.
- No write-test was performed.

### Completed

- `FUN_1422FC35A` was tested under the approved runtime gate and rejected as
  the native cinematic FOV owner.

### Remaining / deferred

- Secondary static candidates remain held; no automatic promotion is allowed.
- Native FOV transition owner remains unresolved.

### Patch summary

Recorded the read-only runtime correlation result for the top static candidate
without changing stable or global fix behavior.

### Changelog summary

Runtime evidence rejects `FUN_1422FC35A` as the current-build cinematic FOV
owner; no production or correction code was changed.

## 2026-08-25 — Restore adapted workflow and model routing modules

### Scope

- Re-analyze the previously removed workflow documentation.
- Restore useful process rules under native C++/ASI/Ghidra realities.
- Preserve the removal of unrelated Messenger/web/Prisma guidance.

### Changed

- Added engineering discipline, task-plan authority, contextual module registry and `GPT-5.6 Luna`/`Terra`/`Sol` routing to `AGENTS.md`.
- Added adapted modules under `docs/`: code style, architecture, implementation, testing, documentation and model routing.
- Updated root `HANDOFF.md` to reference the adapted workflow modules.

### Git review

- Workspace-level documentation changed; canonical source files and Git history were not changed.
- Existing canonical repository user Markdown state remains uncommitted.

### Validation and limits

- Confirmed every registered module exists and is referenced by `AGENTS.md`.
- Confirmed model routing distinguishes bounded execution, architecture/coordination and safety/contradiction escalation.
- No build, game launch, injected ASI test or runtime hook validation was performed.

### Completed

- Useful workflow rules from the prior documentation are restored in project-specific form.
- Unrelated web-project rules remain excluded.

### Remaining / deferred

- No source implementation or resolver recovery was requested.

### Patch summary

Restored evidence-driven phase gates, contextual engineering modules and Sol/Terra/Luna model routing for the native reverse-engineering workspace.

### Changelog summary

Reintroduced project-specific engineering workflow and model escalation guidance without restoring unrelated web-stack documentation.

## 2026-08-25 — Clarify plan fields, contradictory evidence and task-log timing

### Scope

- Apply the three optional workflow clarifications from the documentation review.
- Keep model routing and the stable/research boundaries unchanged.

### Changed

- Added the canonical Required Task Plan Fields checklist to `AGENTS.md` and the implementation guidelines.
- Defined contradictory evidence as evidence that invalidates an accepted contract, ownership conclusion, executable identity or safety assumption.
- Separated per-batch Git review from final task-level `TASKLOG.md` entry timing.
- Added the same timing distinction to the documentation guidelines.

### Validation and limits

- Confirmed all three rules appear in the relevant modules.
- No source, build, release asset, runtime hook or Git state was changed.

### Completed

- Plans now have a stable checklist for scope, evidence, batches, validation, risks, gates and final Git review.
- Consistent evidence does not reopen a phase; contract-invalidating evidence does.
- Batch reviews can be recorded without fragmenting one task into multiple final task-log entries.

### Remaining / deferred

- No implementation or runtime validation was requested.

### Patch summary

Formalized task-plan fields, contradiction handling and batch-versus-task task-log timing.

### Changelog summary

Clarified agent workflow gates and task-log semantics for reliable patch preparation.
## 2026-08-31 — Reject `FUN_1476C41A6` as legacy live-FOV equivalent

### Scope

- Complete the planned read-only micro-review of current-build candidate `FUN_1476C41A6`.
- Do not build or run a runtime artifact; do not modify stable/global ASI code.

### Changed

- Added the single-function Ghidra inspection script under `02-Research/Ghidra/ghidra-scripts`.
- Added `legacy-live-fov1476c41a6-micro-review-204.md` with the instruction-level verdict.
- Marked Batch 7 complete in `LEGACY_LIVE_FOV_CONSUMPTION_RECOVERY_204_TASK_PLAN.md`.

### Validation and limits

- Reused the existing 2.0.4 Ghidra program read-only with `-noanalysis`.
- Verified executable SHA-256: `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Confirmed the ranked scalar slice is sensitivity-settings/UI data, not cinematic FOV.
- No build, game launch, injected ASI test or runtime hook validation was performed.

### Completed

- `FUN_1476C41A6` rejected as a current-build equivalent of the legacy live cinematic FOV consumption boundary.
- No runtime promotion or write-test justified.

### Remaining / deferred

- Native current-build live cinematic FOV consumption boundary remains unresolved.
- Remaining ranked/family candidates require a separate decision; no automatic runtime promotion was made.

### Patch summary

Completed the final planned micro-review candidate and recorded its settings/UI scalar lineage.

### Changelog summary

Rejected `FUN_1476C41A6` as unrelated sensitivity-settings code; stable gameplay and experimental global ASI remained untouched.
## 2026-08-31 — Reconstruct legacy live-FOV callgraph boundary

### Scope

- Complete Batch 1 of the separate cross-version legacy callgraph/data-flow
  reconstruction plan.
- Use existing Ghidra programs read-only; no runtime artifact or production
  source changes.

### Changed

- Added `legacy-fov-callgraph-boundary-203.md` with the exact 2.0.3
  ENTER/EXIT scalar and callgraph neighbourhood.
- Added `InspectLegacyWorkingFovBoundary203.java` for reproducible read-only
  inspection.
- Marked Batch 1 complete in `LEGACY_FOV_CALLGRAPH_DATAFLOW_RECONSTRUCTION_204_TASK_PLAN.md`.

### Validation and limits

- Used the existing 2.0.3 Ghidra program with `-noanalysis -readOnly`.
- Verified executable SHA-256:
  `81961b7281c7cf528ce49c549ce086fcc684bd676f32faf042bc743d939e3c69`.
- Confirmed ENTER `MOVSS XMM0,[0x149EDE50C]` → `FUN_146B68976` and the EXIT
  counterpart `MOVSS XMM0,[RDI+0x38]` → the same callee.
- No current-build descendant was claimed; no build, game launch or runtime
  validation was performed.

### Completed

- Established the portable legacy fingerprint: shared lifecycle function,
  direction-specific scalar sources, shared XMM0 consumer, related-object
  resolver and shared transition helper.

### Remaining / deferred

- Search the existing 2.0.4 program for structural/data-flow descendants.
- No runtime promotion until a concrete current-build boundary is found.

### Patch summary

Recovered the legacy ENTER/EXIT live-FOV callgraph and register/data-flow
contract from the validated 2.0.3 executable.

### Changelog summary

Documented the portable legacy FOV-consumption fingerprint without reusing its
historical addresses as 2.0.4 hook targets.

## 2026-09-01 — Reject ADS primitive-setter correction path

### Scope

- Validate whether the unique 2.0.4 `FirstPersonPrimitiveType` (`+0x265`) setter
  is invoked during the native ADS correction of the post-cinematic weapon/viewmodel state.
- Keep the test read-only and isolated; do not change the stable ASI.

### Changed

- Reviewed the supplied `STALKER2WeaponViewmodelPrimitiveSetterTrace204.log`.
- Updated `WEAPON_VIEWMODEL_FOV_POST_CINEMATIC_CAUSAL_TRACE_TASK_PLAN.md` with the runtime result.
- Rate-limited diagnostic ADS marker logging for the next trace build.

### Validation and limits

- Primitive setter, ADS IN/OUT and camera-writer resolvers installed successfully.
- No `Primitive setter:` event occurred during the captured ADS marker window.
- The log contained no explicit cinematic ENTER/EXIT marker, so the result closes
  the setter-during-ADS hypothesis for the captured window but does not identify
  the downstream correction mechanism.
- No production ASI or game state was modified.

### Completed

- Rejected the hypothesis that the observed ADS correction necessarily invokes
  the resolved `+0x265` setter.

### Remaining / deferred

- Actual first-person/viewmodel projection or refresh consumer remains unresolved.
- No production fix is justified from this branch.

### Patch summary

Closed the bounded ADS primitive-setter branch on negative runtime evidence and
reduced diagnostic marker spam.

### Changelog summary

Confirmed that the captured ADS correction path did not call the current 2.0.4
`+0x265` setter; stable gameplay/cinematic code remains unchanged.

## 2026-09-01 — Close known ADS primitive/mesh differential paths

### Scope

- Analyze the corrected single-window ADS differential capture.
- Determine whether known primitive setter or mesh-assignment paths change during
  the post-cinematic weapon/viewmodel correction.

### Changed

- Updated `WEAPON_VIEWMODEL_FOV_POST_CINEMATIC_CAUSAL_TRACE_TASK_PLAN.md` with the clean capture result.

### Validation and limits

- Exactly one ADS differential window was captured.
- No `+0x265` setter event and no mesh-assignment event occurred inside the window.
- Camera `+0x234`, `+0x262` and aspect remained stable; only world FOV transitioned
  from approximately `90.6557` to `83.6122`.
- The trace does not identify the downstream consumer that visually corrects the
  weapon/viewmodel.

### Completed

- Closed the known primitive-setter and mesh-assignment hypotheses for the
  captured ADS correction path.

### Remaining / deferred

- Identify the first-person/viewmodel projection or consumer path that responds
  to ADS while the inspected scalar and primitive paths remain unchanged.
- Static bounded audit of `+0x234` reads is next; no production implementation is justified.

### Patch summary

Used a clean ADS differential capture to reject the known `+0x265`/mesh refresh
paths and isolate the remaining problem to a downstream consumer layer.

### Changelog summary

Confirmed that the observed ADS correction does not use the inspected primitive
setter or mesh-assignment path; stable gameplay/cinematic code remains unchanged.

## 2026-09-01 — Close broad camera FOV offset intersection audit

### Scope

- Search current 2.0.4 code for consumers intersecting camera `+0x230`,
  `+0x234` and `+0x262`.
- Promote only a small runtime-plausible first-person projection consumer.

### Changed

- Added `02-Research/Ghidra/ghidra-scripts/AuditCameraFirstPersonFovConsumers204.java`.
- Added `02-Research/Ghidra/reports/camera-first-person-fov-consumer-audit-204.md`.
- Updated `WEAPON_VIEWMODEL_FOV_POST_CINEMATIC_CAUSAL_TRACE_TASK_PLAN.md` with
  the bounded scan result.

### Validation and limits

- Read-only Ghidra scan completed against the current program.
- Found 516 functions with at least two target offsets and 9 functions with all
  three offsets.
- All 9 all-three functions were large generic contexts; no compact projection
  consumer was justified.
- No runtime artifact or production ASI change was made.

### Completed

- Closed the raw `+0x230/+0x234/+0x262` intersection as insufficient for safe
  consumer selection.

### Remaining / deferred

- The downstream first-person/viewmodel consumer remains unresolved.
- Further work must narrow from ADS data-flow or a stronger semantic anchor,
  without opening a broad renderer search.

### Patch summary

Completed a bounded static consumer triage and rejected raw offset intersection
as a sufficient hook-selection method.

### Changelog summary

Documented that camera FOV offset intersections remain too generic for a safe
runtime hook; stable gameplay/cinematic code remains unchanged.

## 2026-09-01 — Audit ADS local call/data flow

### Scope

- Inspect only the validated current 2.0.4 ADS IN/OUT neighborhoods.
- Classify direct calls, object dereferences, transition math and state writes.
- Do not build a runtime tracer or alter the stable ASI without a stronger
  ownership contract.

### Changed

- Added `02-Research/Ghidra/ghidra-scripts/AuditAdsLocalCallChain204.java`.
- Added `02-Research/Ghidra/ghidra-scripts/RankAdsDirectCallees204.java`.
- Added `02-Research/Ghidra/ghidra-scripts/DecompileRankedAdsCallees204.java`.
- Added `02-Research/Ghidra/ghidra-scripts/PrintProgramIdentity204.java`.
- Added `02-Research/Ghidra/reports/ads-local-callchain-audit-204.md`.
- Updated `WEAPON_VIEWMODEL_FOV_POST_CINEMATIC_CAUSAL_TRACE_TASK_PLAN.md` with
  the Batch 9 result.

### Validation and limits

- The read-only Ghidra scripts executed successfully, but the selected Ghidra
  image was stale relative to the current runtime 2.0.4 executable.
- Static ADS matches were `RVA 0x6A849C` and `0x6A8639`; current runtime logs
  resolve the corresponding anchors at different RVAs.
- Direct-callee ranking and decompilation are invalidated for current 2.0.4
  address selection.
- No runtime artifact, production hook or stable ASI change was made.

### Completed

- Detected and explained the RVA discrepancy as a Ghidra image-identity
  mismatch rather than a function-entry versus hook-instruction offset.
- Preserved the stale-image findings as historical guidance only.

### Remaining / deferred

- Reopen the ADS local ranking against a Ghidra image matching the current game
  SHA-256 before selecting any runtime candidate.
- No runtime follow-up or production implementation is justified from this
  static batch.

### Patch summary

Added a read-only ADS call-chain audit, then invalidated its candidate ranking
after reconciling the stale Ghidra image with the current runtime executable.

### Changelog summary

Confirmed that the initial ADS static ranking used a stale image; current-build
candidate selection remains deferred, and stable gameplay/cinematic code remains
unchanged.

## 2026-09-02 — Revalidate ADS static analysis on matching 2.0.4 image

### Scope

- Use the canonical `Dump/STALKER2-Ghidra` entry
  `Stalker2-Win64-Shipping.exe (2.0.4)`.
- Enforce executable identity before repeating the ADS local audit and ranking.
- Reconcile current ADS anchors and inspect only the bounded direct-callee set.
- Do not create a runtime tracer or modify production ASI logic.

### Changed

- Updated `02-Research/Ghidra/ghidra-scripts/RankAdsDirectCallees204.java` to
  use the current 2.0.4 ADS containing-function entry `RVA 0x6ABB9C`.
- Updated `02-Research/Ghidra/ghidra-scripts/DecompileRankedAdsCallees204.java`
  with the current-image top three bounded candidates.
- Extended `02-Research/Ghidra/reports/ads-local-callchain-audit-204.md` with
  the matching-image identity header, current ADS slice and current ranking.

### Validation and limits

- Ghidra `.text` block size: `130818560`, matching the current executable.
- Image base: `0x140000000`, matching the current executable.
- Current executable SHA-256:
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- ADS IN resolved uniquely at `RVA 0x6ABE7E`; ADS OUT resolved uniquely at
  `RVA 0x6AC01B`.
- Current ADS containing function is `FUN_1406ABB9C`; the old
  `FUN_1406A81BA` result is stale-image evidence and remains invalid.
- Current-image ranking found 13 direct callees. The top three were reviewed;
  none has sufficient ownership and causal evidence for a hook.
- Primitive-setter reconciliation is not included in this batch.

### Completed

- Passed the matching-image identity gate for the canonical 2.0.4 Ghidra entry.
- Revalidated the ADS local call-chain anchor and bounded direct-callee ranking
  on the correct image.
- Preserved stale-image provenance instead of replacing the earlier result.

### Remaining / deferred

- Reconcile the current-image primitive setter against the runtime-known
  `RVA 0x5665FA6` anchor.
- Revalidate any surviving ADS-state ownership candidate before proposing a
  bounded runtime trace.
- No production implementation is justified yet.

### Patch summary

Reopened the ADS static investigation on the canonical matching Steam 2.0.4
Ghidra image and replaced the invalid stale-image candidate set with a current
bounded ranking.

### Changelog summary

Corrected the analysis provenance for the ADS research; no stable gameplay or
 cinematic behavior was changed.

## 2026-09-02 — Complete matching-image production and P1/P2 revalidation

### Scope

- Reconcile the canonical `Dump/STALKER2-Ghidra` project with the Steam 2.0.4
  executable before interpreting static evidence.
- Revalidate production-facing cinematic/gameplay contracts and bounded
  high-potential cinematic alternatives.
- Do not modify production C++/ASI/INI logic, create a runtime tracer, launch
  the game or expand into weapon/viewmodel research.

### Changed

- Updated `MATCHING_IMAGE_REVALIDATION_TASK_PLAN.md` through final review.
- Updated `02-Research/Ghidra/reports/matching-image-revalidation-2026-09-02.md`
  with fresh current-image identity, contract reconciliation and P1/P2
  results.
- Preserved stale-image provenance and corrected the lock note: the leftover
  lock was caused by an agent-owned headless process, not the user's GUI
  session.

### Validation and limits

- Canonical `2.0.4` Ghidra entry SHA-256:
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- `.text` size: `130818560`; image base: `0x140000000`.
- ADS IN/OUT anchors matched current runtime RVAs `0x6ABE7E` and `0x6AC01B`.
- Current ENTER aspect setter uniquely resolved at `RVA 0x6B7CB05`; its
  local data-flow contains aspect/mode stores but no FOV/projection owner.
- The generic cinematic helper returned `ENTER_MATCHES=0`; this was reconciled
  as a stale broad helper pattern, not promoted as an anchor failure.
- The recorded camera-field intersection contains `516` generic functions; no
  causal projection consumer was promoted from that non-specific result.
- No build, runtime tracer or in-game validation was performed in this task.

### Completed

- Matching-image identity gate: PASS.
- Production safety contracts: PASS within the tested current-image/source
  contract; no production contract was changed.
- P1/P2 cinematic alternatives: no concrete native or simpler replacement
  advantage demonstrated; no branch promoted or reopened.
- Agent-owned Ghidra process and stale lock were verified absent after
  analysis.

### Remaining / deferred

- Any runtime candidate trace or production change requires a separate approved
  task with its own identity gate and validation scope.
- Weapon/viewmodel causal research remains outside this task.

### Patch summary

Completed the matching-image recovery pass after the stale-Ghidra-image
incident, confirmed the current production contracts and closed the bounded
P1/P2 revalidation without changing production behavior.

### Changelog summary

Restored static-evidence provenance for the current 2.0.4 image; no native
cinematic replacement mechanism was proven, and no production or weapon FOV
implementation was changed.

## 2026-09-02 — Cross-patch production resolver validation

### Scope

- Apply the unchanged `0.4.0` production resolver contracts to the canonical
  Steam `2.0.2`, `2.0.3` and `2.0.4` Ghidra entries.
- Validate identity, uniqueness, gameplay instruction decoding and cinematic
  aspect/ENTER/EXIT contracts.
- Do not generalize signatures, modify production code, build/inject an ASI or
  expand into weapon research.

### Changed

- Added the read-only helper
  `02-Research/Ghidra/ghidra-scripts/ValidateCurrentProductionResolversAcrossPrograms.java`.
- Added
  `02-Research/Ghidra/reports/cross-patch-production-resolver-validation-2026-09-02.md`.
- Added and completed
  `CROSS_PATCH_PRODUCTION_RESOLVER_VALIDATION_TASK_PLAN.md`.

### Validation and limits

- Identity `PASS` for all three images; `.text` sizes were `130803712`,
  `130806784` and `130818560` respectively, with distinct `.text` hashes.
- Gameplay, cinematic aspect, ENTER and EXIT patterns each matched uniquely on
  `2.0.2`, `2.0.3` and `2.0.4`.
- Gameplay decode passed as `MOVSS [RBX+0x30], XMM0` on all three images.
- Overall Gate A result: `WOULD INSTALL` for all three versions.
- This is static resolver portability evidence, not runtime injection or
  in-game validation on the older versions.
- No production source, ASI or runtime tracer was changed.

### Completed

- Confirmed current production resolver portability across the retained Steam
  `2.0.2`, `2.0.3` and `2.0.4` images.
- Confirmed that no Gate B signature generalization is required for this set.
- Verified the agent-owned Ghidra process exited and the canonical project has
  no remaining lock file.

### Remaining / deferred

- Future patches still require a fresh identity gate and validation.
- Runtime validation on older game versions remains unperformed.
- Weapon/viewmodel research remains a separate bounded task.

### Patch summary

Added a read-only cross-patch Gate A simulation and documented unique resolver
matches and decode contracts across Steam 2.0.2, 2.0.3 and 2.0.4.

### Changelog summary

Confirmed that the unchanged 0.4.0 resolver architecture is statically
portable across the three tested Steam images; no production behavior changed.

## 2026-09-02 — Revalidate weapon/viewmodel FOV ownership on canonical 2.0.4

### Scope

- Revalidate primitive setter/MRSD linkage and ADS ownership on the matching
  Steam 2.0.4 Ghidra image.
- Re-rank direct ADS callees and inspect bounded camera-field evidence.
- Do not create a runtime tracer, modify production source/ASI or expand the
  weapon branch beyond the approved static scope.

### Changed

- Added the read-only helper
  `02-Research/Ghidra/ghidra-scripts/AuditCurrentWeaponViewmodelOwnership204.java`.
- Added
  `02-Research/Ghidra/reports/weapon-viewmodel-fov-revalidation-2026-09-02.md`.
- Updated `WEAPON_VIEWMODEL_FOV_POST_CINEMATIC_CAUSAL_TRACE_TASK_PLAN.md` with
  current-image ownership and ADS results.

### Validation and limits

- Canonical 2.0.4 identity passed: executable SHA-256
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`,
  `.text` size `130818560`, image base `0x140000000`.
- Primitive setter `RVA 0x5665FA6` resolved to `FUN_145665FA6`; its current
  instruction flow compares/writes `+0x265` and jumps to `0x140072660`.
- The setter has one discovered xref from `FUN_1454B26FE`; weapon/viewmodel
  ownership of its `this` object remains unproven.
- ADS resolved to `FUN_1406ABB9C`; 13 direct callees were ranked.
- Strongest candidate `FUN_1424BE5AE` is a generic interpolation helper with
  128 callers. No direct callee established a causal weapon/viewmodel
  projection owner.
- No build, runtime tracer or in-game validation was performed.

### Completed

- Current primitive setter/MRSD linkage: PASS.
- Current ADS containing-function and direct-callee ranking: PASS/COMPLETE.
- Direct primitive/MRSD explanation for the captured ADS correction: CLOSED as
  non-causal on available evidence.

### Remaining / deferred

- Downstream first-person/viewmodel projection ownership remains unresolved.
- A future runtime gate requires a specific current-image ownership candidate.
- No production implementation is justified.

### Patch summary

Revalidated weapon/viewmodel ownership anchors on the correct 2.0.4 image and
closed the stale-image primitive/MRSD branch without creating a new tracer.

### Changelog summary

Confirmed current primitive setter/MRSD linkage and current ADS ownership, but
found no bounded native weapon/viewmodel projection candidate.

## 2026-09-02 — Reject AF4FA4 projection branch for post-cinematic weapon correction

### Scope

- Validate the `FUN_140AF4FA4` entry predicate using the existing read-only
  runtime gate.
- Record `source+0x25C`, its absolute value, the computed branch and source
  flags `+0x260/+0x261` across cinematic EXIT and ADS correction.
- Do not add production behavior or expand into generic renderer analysis.

### Changed

- Updated the existing PRE-only read-only runtime tracer to observe the entry
  predicate; removed the unused POST-hook path.
- Added the matching-image predicate audit and updated the downstream consumer
  report and task plan.

### Validation and limits

- Canonical Steam 2.0.4 identity passed.
- Runtime capture recorded 1914 PRE events across the cinematic/ADS sequence.
- `source+0x25C=0`, `abs(+0x25C)=0`, and `predicate=early` for all observed
  events; `+0x260/+0x261` remained `0x0/0x0`.
- `source+0x230` and output `+0x30/+0x38` still showed the ADS world-FOV
  transition.
- No projection/tanf/atanf path entry was observed; no production ASI was
  changed.

### Completed

- Rejected `FUN_140AF4FA4` as the causal owner for the captured post-cinematic
  weapon correction.
- Preserved the independent world-FOV propagation finding.

### Remaining / deferred

- The downstream first-person/viewmodel correction owner remains unresolved.
- No new runtime tracer is authorized until a new bounded static candidate is
  identified.

### Patch summary

Added PRE-only predicate observation and closed the AF4FA4 projection branch on
current 2.0.4 runtime evidence.

### Changelog summary

Confirmed that the observed ADS weapon correction does not enter the candidate
consumer's projection-derived path; no production behavior changed.

## 2026-09-02 — Stop local output+0x38 downstream-use audit

### Scope

- Inspect only the local caller tail after `FUN_140AF4022 + 0xAF42A7`.
- Find direct or obvious local consumers of the validated presentation
  output/`+0x38` with first-person/viewmodel semantics.
- Do not scan generic renderer/data-flow, create a runtime tracer or change
  production behavior.

### Changed

- Added the bounded helper
  `02-Research/Ghidra/ghidra-scripts/AuditOutput38DownstreamUse204.java`.
- Added
  `02-Research/Ghidra/reports/ads-output38-downstream-use-audit-204-2026-09-02.md`.
- Completed `ADS_OUTPUT38_DOWNSTREAM_USE_TASK_PLAN.md`.

### Validation and limits

- Canonical 2.0.4 identity passed.
- No direct `RBX+0x38` read or write was found after the consumer call.
- No direct callee was established as a first-person/viewmodel presentation
  owner.
- `RAX/RCX+0x38` accesses were not tied to the validated output object.
- No runtime tracer or production ASI change was made.

### Completed

- Bounded output+0x38 downstream-use theory stopped as `UNRESOLVED`.
- Generic renderer/data-flow expansion was explicitly not performed.

### Remaining / deferred

- The downstream weapon/viewmodel owner remains unresolved and is deferred.
- Dialogue zoom is the next independent research scope.

### Patch summary

Audited the local presentation-output tail and stopped without promoting an
unsupported downstream consumer.

### Changelog summary

No local current-2.0.4 first-person/viewmodel consumer was identified; no
production behavior changed.

---

## 2026-09-05 — Cinematic axis-constraint ownership Batch 1B

### Scope

- Perform the approved narrow source-to-binary correspondence pass for the
  UE 5.5 `GetProjectionData` / effective axis-policy handoff hypothesis.
- Do not change production source, build artifacts or runtime behavior.

### Changed paths

- `research/reports/CINEMATIC_ASPECT_RATIO_AXIS_CONSTRAINT_OWNERSHIP_BATCH1B.md`
- `backlog/active/CINEMATIC_ASPECT_RATIO_AXIS_CONSTRAINT_OWNERSHIP_TASK_PLAN.md`

### Validation and limits

- Current Steam 2.0.4 identity gate: `PASS`.
- Existing current-build evidence was checked for viewport/aspect, effective
  FOV, projection construction and axis-dependent semantics.
- Historical 2.0.3 projection logs were retained as provenance only.
- No safe current-build projection boundary or effective owner was established.
- No runtime test, tracer, source change, build or production modification
  was performed.

### Result

- Batch 1B: `COMPLETE — BLOCKED OUTCOME`; no promotable current-build
  boundary was found.
- UE 5.5 engine contract: confirmed as a research target, not as STALKER 2
  binary proof.
- Batch 2 `MaintainYFOV` runtime test remains blocked.
- Production Full Hor+ remains unchanged.

### Patch summary

Checked the approved `GetProjectionData` structural target without promoting
generic or historical projection helpers as current-build ownership.

### Changelog summary

No production behavior changed; the axis-constraint branch remains unresolved
and deferred pending new evidence.

---

## 2026-09-03 — Dialogue zoom production promotion

### Scope

- Promote the validated Adaptive and optical half-strength dialogue model into
  the production dialogue subsystem.
- Keep the existing resolver boundary, cinematic isolation, ADS specificity,
  gameplay and cinematic subsystems unchanged.
- Use `DialogueCycle=F10` as the production default; custom bindings remain
  supported.

### Changed

- Updated `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- Updated the unified test/build script and public configuration examples.
- Added `DIALOGUE_ZOOM_PRODUCTION_PROMOTION_TASK_PLAN.md`.
- Production policy cycle is now `Native → Adaptive → Reduced → Disabled → Native`.
- `Reduced` now uses the promoted optical half-strength model.
- Promoted the validated EXIT anchor/recovery behavior.
- Removed feasibility-only `OpticalReduced` and EXIT diagnostic instrumentation
  from the production build path.

### Validation and limits

- Build succeeded for the production candidate.
- `git diff --check` passed with only existing line-ending warnings.
- Canonical Steam 2.0.4 runtime identity passed:
  `gameSha256=2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Runtime log confirmed all four policies, custom hotkey cycling, persistence,
  sequential dialogue recovery and cinematic/gameplay hook installation.
- User visually confirmed smooth ENTER/EXIT behavior for all four policies.
- Runtime coverage is the tested Steam 2.0.4 scenarios; future patches and
  untested game states remain outside this result.

### Completed

- Dialogue production core promotion: PASS.
- Native, Adaptive, Reduced and Disabled: PASS.
- Sequential dialogue cycles and baseline recovery: PASS.
- Hotkey cycle and persistence: PASS.
- Existing gameplay/cinematic behavior: no regression observed.

### Remaining / deferred

- Final release archive/package review for v0.5.0 remains.
- Runtime compatibility on Steam 2.0.2/2.0.3 remains statically portable but
  not runtime-validated.

### Patch summary

Promoted the validated FOV-aware optical dialogue zoom model and smooth EXIT
recovery into the production dialogue path.

### Changelog summary

Added Adaptive dialogue zoom, changed Reduced to optical half-strength behavior,
and preserved the native dialogue lifecycle with smooth recovery.

---

## 2026-09-10 — D3D12 overlay ownership feasibility boundary

### Scope

- Investigate the approved bounded path from DXGI factory creation toward an
  active game swapchain for optional notification-overlay feasibility.
- Keep the stable Camera/FOV core and release assets untouched.
- Stop before Present, ResizeBuffers, command submission, render resources,
  ImGui or notifications.

### Changed paths

- `src/d3d12_notification_overlay_feasibility_204.cpp`
- `backlog/active/DXGI_COM_SAFE_SWAPCHAIN_INTERCEPTION_TASK_PLAN.md`
- `backlog/active/EXISTING_ACTIVE_SWAPCHAIN_OWNERSHIP_TASK_PLAN.md`
- `research/reports/D3D12_NOTIFICATION_OVERLAY_BATCH1A_BOOTSTRAP_RESULT.md`
- `research/reports/EXISTING_ACTIVE_SWAPCHAIN_OWNERSHIP_GATE_A_INVENTORY.md`
- `build-artifacts/test-asi/STALKER2NotificationOverlayFeasibility.asi`

### Validation and evidence

- Visual Studio 2022 x64 C++23 build: PASS.
- Final test ASI size: 800,256 bytes.
- Final test ASI SHA-256:
  `298F3455697F1D51E7648995CEA9EDD9EB108CD5B8275749733162DF3E98B8C3`.
- Earlier build with composition slot `17` was invalid and excluded from
  evidence; the final build uses `24`.
- Official DXGI interface review confirmed the relevant method family and
  D3D12 `pDevice` semantics; the ABI mapping used in the final build is
  
  `10=CreateSwapChain`, `15=CreateSwapChainForHwnd`,
  `16=CreateSwapChainForCoreWindow`, `24=CreateSwapChainForComposition`.
- User runtime log: `CreateDXGIFactory1` returned `IDXGIFactory4`, the queried
  `IDXGIFactory2` pointer matched the returned address, and the private
  25-entry clone installed without a crash.
- No `CreateSwapChain*` callback was observed in the corrected run.
- No swapchain, device or queue provenance was obtained.
- Git commands were intentionally not run at the user's request; no Git state
  change was performed or claimed.

### Completed

- Rejected the earlier unsafe direct SafetyHook COM-VMT approach after its
  access violation evidence.
- Implemented and built the bounded per-instance COM clone path.
- Added forensic factory IID, returned-interface, ABI and provenance logging.
- Corrected the composition method slot before accepting the final runtime
  result.
- Completed the corrected factory-path observation as a valid bounded negative:
  no creation callback was observed.
- Completed Gate A inventory for existing swapchain acquisition: no
  independent current-build, documented runtime or retained-COM acquisition
  anchor was found.

### Remaining / deferred / blocked

- `EXISTING_ACTIVE_SWAPCHAIN_OWNERSHIP`: `BLOCKED / DEFERRED` at Gate A.
- Swapchain ownership, device provenance and queue provenance were not reached.
- Present/Resize lifecycle, command submission, rendering and ImGui were not
  started.
- Overlay idea remains `DEFERRED`, not failed or rejected; resume requires a
  genuinely new ownership evidence class.
- Production Camera/FOV core and release behavior remain unchanged.

### Patch summary

Added a bounded COM-safe factory observation implementation, corrected the DXGI
ABI mapping, and documented the absence of an independently proven path to an
already-existing game swapchain.

### Changelog summary

No production behavior changed. D3D12 notification overlay research is deferred
because presentation ownership could not be established safely.

---

## 2026-09-10 — UE4SS reflected camera-call route closed

### Scope

- Audit the UE4SS reflected-call surface for `CameraComponent.GetCameraView`.
- Perform a follow-up read-only metadata/callability audit of
  `PlayerCameraManager.BlueprintUpdateCamera`.
- Keep runtime invocation, writes, suppression, production ASI changes and
  undocumented helper reverse engineering out of scope.

### Changed paths

- `research/deferred/GETCAMERAVIEW_CALLABILITY_PROBE_TASK_PLAN.md`

### Validation and evidence

- Current UE4SS metadata confirms `GetCameraView(float, FMinimalViewInfo&)`.
- Current UE4SS metadata confirms
  `BlueprintUpdateCamera(AActor*, FVector&, FRotator&, float&)`.
- `SafeObject.call(...)` was inspected and does not establish safe complex
  ref/out storage or readback semantics.
- `CallFunctionsByHandle(...)` exists only through a wrapper; no source-backed
  contract, packing rules, lifetime rules or ref/out example was found.
- No runtime invocation was attempted by design.
- Existing unrelated working-tree changes were preserved.

### Completed

- Closed the UE4SS reflected-call route as `CLOSED / BLOCKED BY LUA ABI`.
- Classified both camera candidates as metadata-positive but invocation-blocked.
- Corrected the archived plan to record the later
  `BlueprintUpdateCamera` read-only audit without misrepresenting it as part of
  the primary runtime probe.

### Remaining / deferred / blocked

- The camera-evaluation hypothesis remains open; only this Lua invocation path
  is exhausted.
- Resume requires a documented/source-backed call bridge, a simpler callable
  reflected method, or a new independent lifecycle anchor.
- No production camera behavior or release asset changed.

### Patch summary

Documented the bounded UE4SS callability audit and its concrete Lua ABI blocker;
no runtime probe or behavior modification was added.

### Changelog summary

No production changes. Reflected camera evaluation remains deferred pending a
safe, documented invocation contract.

---

## 2026-09-10 — ASI vtable `+0x638` static gate closed

### Scope

- Classify only the virtual dispatch immediately after the confirmed camera
  writer for the canonical Steam 2.0.4 image.
- Determine whether trusted target/type evidence establishes camera/view or
  projection reevaluation semantics.
- No runtime calls, writes, new hooks, broad scans or production changes.

### Changed paths

- `research/deferred/ASI_LAST_GATE_VTABLE_638_TASK_PLAN.md`

### Validation and evidence

- Existing bounded Ghidra report
  `02-Research/reports/CINEMATIC_AXIS_POLICY_OUTPUT_OWNERSHIP_BATCH1_1_VSLOT638.md`
  records identity `PASS` for SHA-256
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Image base `0x140000000` and the recorded `.text` layout match the canonical
  Steam 2.0.4 image.
- `FUN_1453BA300` preserves the source/output pair across
  `FUN_140AF4022` and the following virtual call at `[RDI + 0x638]`.
- No trusted concrete virtual target or owner/type association was recovered
  within the bounded pass.
- No runtime feasibility test was performed or authorized.

### Completed

- Completed the final ASI static gate as `COMPLETE / BOUNDED PARTIAL`.
- Confirmed same-object source/output handoff into the virtual dispatch.
- Closed the ASI cinematic route at this gate as `CLOSED / BLOCKED` for lack
  of proven reevaluation semantics.

### Remaining / deferred / blocked

- The broader camera-evaluation hypothesis is not disproven.
- Resume requires genuinely new target/type or lifecycle evidence; neighboring
  slot scans and further caller climbing remain out of scope.
- Production ASI and stable gameplay behavior remain unchanged.

### Patch summary

Recorded the existing bounded vtable-slot audit and its unresolved polymorphic
target; no runtime or production code was changed.

### Changelog summary

No production changes. The final ASI cinematic research gate is deferred because
`+0x638` could not be promoted to a proven camera/view reevaluation boundary.

---

## 2026-09-10 — UE4SS cinematic architecture feasibility closed

### Scope

- Audit the installed UE4SS observer, state-access and callable surfaces for a
  future cinematic subsystem.
- Keep the stable Camera/FOV ASI, release assets and runtime behavior untouched.

### Changed paths

- `research/reports/UE4SS_CINEMATIC_ARCHITECTURE_FEASIBILITY.md`
- `research/deferred/UE4SS_CINEMATIC_ARCHITECTURE_FEASIBILITY_TASK_PLAN.md`

### Validation and evidence

- Existing UE4SS observers confirm live Camera/CameraManager/PCM discovery,
  reflected state reads and lifecycle sampling.
- Reflected property mutation is available, but storage mutation alone causing
  the required downstream reevaluation was not observed.
- `GetCameraView` and `BlueprintUpdateCamera` metadata is present, but complex
  ref/out invocation remains unsupported by a confirmed Lua ABI contract.
- `CallFunctionsByHandle` is available only through an opaque wrapper without
  source-backed packing, lifetime or readback rules.
- No runtime writes, undocumented calls, replacement module or production
  migration was performed.
- Existing unrelated working-tree changes were preserved.

### Completed

- Completed capability matrix and architecture decision gate.
- Classified UE4SS observer/state architecture as `FEASIBLE`.
- Classified UE4SS cinematic native-refresh architecture as
  `BLOCKED / UNPROVEN`.
- Deferred production migration as `NOT JUSTIFIED`.

### Remaining / deferred / blocked

- Cinematic research remains deferred pending a genuinely new evidence class.
- Native reevaluation and downstream projection rebuild remain unresolved.
- Stable production ASI remains the active fallback and was not modified.

### Patch summary

Documented the UE4SS architecture boundary: strong observation/state access,
but no proven safe native cinematic reevaluation mechanism.

### Changelog summary

No production changes. UE4SS migration is deferred; only observer/state-layer
use is currently justified.

---

## 2026-09-10 — UE4SS STALKER2CameraTweaks parallel prototype

### Scope

- Create a separate UE4SS module named `STALKER2CameraTweaks`.
- Port the established gameplay, cinematic and dialogue policies where the
  available UE4SS Lua/property surface is safe and deterministic.
- Keep the production ASI and all native source behavior untouched.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/STALKER2CameraTweaks.ini`
- `research/ue4ss/STALKER2CameraTweaks/README.md`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks.zip`
- `UE4SS_STALKER2CAMERATWEAKS_PORT_TASK_PLAN.md` (archived after review)

### Validation and evidence

- Verified dynamic CameraComponent/CameraManager/PCM discovery logic and
  object invalidation recovery paths by source inspection.
- Verified the ZIP contains only the intended UE4SS module files.
- `git diff --check` found no new whitespace errors in the implementation;
  one pre-existing trailing-whitespace warning remains in an unrelated line of
  `backlog/TASKLOG.md`.
- No Lua interpreter is installed in the workspace, so automatic Lua syntax
  validation was not available.
- No game launch or runtime validation was performed.

### Completed

- Added the separate UE4SS module with the required module name.
- Added configuration-compatible gameplay, cinematic and dialogue policy
  handling with bounded reflected writes and change-only state logging.
- Documented the unsupported native reevaluation/ref-out limits and ASI-off
  test isolation requirement.
- Built the test ZIP without production ASI files.

### Remaining / deferred / blocked

- In-game behavior, visual framing and post-exit restoration remain
  `NOT RUNTIME-VALIDATED`.
- Native downstream projection reevaluation after reflected writes remains
  `UNPROVEN`; no unsafe invocation was added.
- The stable ASI remains the production fallback and was not modified.

### Patch summary

Added a parallel UE4SS implementation that exposes the current camera policy
model through dynamic live-object discovery and bounded property operations.

### Changelog summary

New research/test artifact: `STALKER2CameraTweaks` UE4SS prototype. No change
to the stable ASI release path.

---

## 2026-09-10 — UE4SS gameplay native-like transition correction

### Scope

- Re-analyze the confirmed native gameplay A→B→C transition.
- Correct only the gameplay path of the UE4SS `STALKER2CameraTweaks`
  prototype.
- Keep cinematic and dialogue mutation disabled for this validation build.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/STALKER2CameraTweaks.ini`
- `research/ue4ss/STALKER2CameraTweaks/README.md`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks-gameplay-test.zip`
- `UE4SS_GAMEPLAY_NATIVE_FIX_TASK_PLAN.md` (archived after review)

### Validation and evidence

- Reconfirmed from the supplied UE4SS log that the previous build only loaded
  with gameplay disabled and recorded state; it did not reproduce the native
  two-pass behavior.
- Replaced the one-step final-state write with a bounded sequence:
  constrained on → native 16:9 aspect → constrained off.
- Added explicit phase, retry-limit and completion logging.
- Static structural checks passed; no Lua interpreter is available for an
  automatic syntax check.
- Archive contents were inspected and contain only the intended module files.
- No in-game runtime validation was performed after this correction.

### Completed

- Implemented the gameplay-only native-like transition attempt.
- Disabled cinematic/dialogue mutation execution in this test build.
- Preserved dynamic object discovery and authored gameplay FOV.

### Remaining / deferred / blocked

- Visual gameplay framing and downstream projection rebuild remain
  `NOT RUNTIME-VALIDATED`.
- UE4SS setter side effects are not yet proven equivalent to the native
  reevaluation path.
- Cinematic and dialogue ports remain intentionally untested.
- Production ASI remains untouched.

### Patch summary

Corrected the UE4SS gameplay prototype to follow the evidence-backed native
two-pass transition instead of writing only the final reflected state.

### Changelog summary

Updated the UE4SS gameplay test artifact; no production ASI changes.

---

## 2026-09-10 — UE4SS module config path correction

### Scope

- Resolve the UE4SS prototype's adjacent INI from the script location rather
  than relying on UE4SS's current working directory.
- Keep gameplay algorithm, cinematic/dialogue behavior and production ASI
  untouched.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks-gameplay-configpath-fix.zip`
- `research/deferred/UE4SS_CONFIG_PATH_FIX_TASK_PLAN.md`

### Validation and evidence

- Fresh installed log showed `Gameplay.Enabled=false` while the adjacent
  installed INI contained `Enabled=true`, proving the previous log value could
  not be trusted as evidence of the adjacent file.
- The module now logs `CONFIG_PATH` and resolves
  `Scripts/../STALKER2CameraTweaks.ini` first.
- Structural checks and archive-content inspection passed.
- Runtime validation after the path correction remains pending.

### Completed

- Removed the `.ini` path ambiguity without introducing `.conf` handling.
- Produced a new gameplay test ZIP with the corrected loader.

### Remaining / deferred / blocked

- Gameplay visual success remains unvalidated until a new launch log is
  supplied.
- No cinematic or dialogue testing was performed.
- Production ASI remains untouched.

### Patch summary

Fixed UE4SS configuration resolution so `STALKER2CameraTweaks` reads the INI
beside its own script and reports the exact selected path.

### Changelog summary

Configuration-path reliability fix for the UE4SS gameplay test prototype.

---

## 2026-09-10 — UE4SS module-specific logging

### Scope

- Add a dedicated `STALKER2CameraTweaks.log` beside the UE4SS module INI.
- Keep gameplay behavior unchanged and keep cinematic/dialogue unvalidated.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/README.md`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks-gameplay-module-log.zip`
- `research/deferred/UE4SS_MODULE_LOG_TASK_PLAN.md`

### Validation and evidence

- Log path is derived from `Scripts/main.lua`, independent of UE4SS working
  directory.
- Module messages continue to appear in `UE4SS.log` and are also written to
  the dedicated module log.
- Static checks and ZIP content inspection passed.
- Runtime creation was not revalidated after this change.

### Completed

- Added dedicated module logging with a `LOG_PATH` startup record.
- Preserved gameplay logic and did not confirm cinematic/dialogue behavior.

### Remaining / deferred / blocked

- Runtime validation of the new log remains pending.
- Cinematic and dialogue fixes remain unconfirmed.
- Production ASI remains untouched.

### Patch summary

Added isolated logging for UE4SS `STALKER2CameraTweaks` evidence collection.

### Changelog summary

UE4SS test module now writes its own module-specific log.

---

## 2026-09-10 — UE4SS cinematic and dialogue policy port

### Scope

- Restore the cinematic aspect/FOV and dialogue policy branches in the
  separate `STALKER2CameraTweaks` UE4SS module.
- Keep the validated gameplay branch and production ASI unchanged.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/README.md`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks-all-policies-test.zip`
- `research/deferred/UE4SS_CINEMATIC_DIALOGUE_PORT_TASK_PLAN.md`

### Validation and evidence

- Re-enabled cinematic lifecycle handling through the shared
  `SetCinematicMode` callback when available, with a reflected-hook fallback.
- Re-enabled configured cinematic aspect policies and the existing Full Hor+
  FOV policy for non-`Native` modes.
- Re-enabled the existing dialogue zoom detector and policy model.
- Confirmed no separate `DisplayAspectRatio` or `PreserveAuthoredFOV` contract
  remains in the module.
- Inspected the test archive; it contains only the module README, INI and
  `Scripts/main.lua`.
- Runtime cinematic and dialogue behavior was not validated.

### Completed

- Restored the two requested UE4SS policy branches without ref/out calls,
  native hooks, raw offsets or production ASI changes.
- Preserved the dedicated module log and dynamic object discovery.

### Remaining / deferred / blocked

- User must test cinematic and dialogue behavior in-game.
- Visual framing, letterbox removal and post-exit restoration remain
  unconfirmed.
- UE4SS native reevaluation remains unproven; writes are best-effort reflected
  property/setter operations.

### Patch summary

Restored the UE4SS module's cinematic lifecycle/aspect handling and dialogue
zoom policy while keeping gameplay logic and production ASI untouched.

### Changelog summary

UE4SS test module now includes the previously disabled cinematic and dialogue
policy branches for runtime evaluation.

---

## 2026-09-10 — UE4SS dialogue/cinematic conflict correction

### Scope

- Correct the newly restored cinematic/dialogue branches after the first test
  log showed FOV oscillation and missing cinematic activation.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks-all-policies-test.zip`

### Validation and evidence

- The supplied log showed repeated dialogue writes of `110.791499` while the
  game interpolated the camera FOV, confirming a write loop.
- Dialogue application is now bounded to one write per detected transition;
  lifecycle changes reset dialogue state.
- Added a state fallback for cinematic paths that do not emit the reflected
  `SetCinematicMode` callback, restricted to a completed gameplay stage.
- Runtime retest remains pending.

### Completed

- Removed the known repeated-write conflict from the UE4SS dialogue branch.
- Added a non-invasive cinematic detection fallback.

### Remaining / deferred / blocked

- Cinematic visual framing and dialogue behavior still require a fresh in-game
  test.
- The fallback cannot prove native evaluation or guarantee all cinematic types.

### Patch summary

Bounded dialogue FOV writes to prevent oscillation and added fallback cinematic
state detection when the reflected lifecycle hook is silent.

### Changelog summary

UE4SS test module no longer continuously fights the game's dialogue FOV
interpolation.

---

## 2026-09-10 — UE4SS cinematic FOV baseline and write-loop correction

### Scope

- Correct the second runtime-test regression reported in the module log:
  exaggerated cinematic FOV and repeated cinematic/dialogue writes.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/README.md`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks-all-policies-test.zip`

### Validation and evidence

- The supplied log showed cinematic FOV `148.582718`, caused by using the
  transient `FieldOfView=120` instead of the authored
  `FirstPersonFieldOfView=90` baseline.
- The log also showed repeated cinematic writes every poll while the game
  continued its own camera transition.
- Cinematic baseline now prefers `FirstPersonFieldOfView` and cinematic policy
  writes are bounded to once per entry.
- Runtime retest remains pending.

### Completed

- Removed the observed `120 → 148.58` cinematic over-expansion source.
- Removed continuous cinematic writes that could fight dialogue/camera
  interpolation.

### Remaining / deferred / blocked

- Fresh gameplay, dialogue and cinematic runtime evidence is still required.
- No claim is made yet about final visual framing or dialogue behavior.

### Patch summary

Use the authored first-person FOV as the cinematic source and apply cinematic
state once per lifecycle entry to avoid FOV inflation and oscillation.

### Changelog summary

UE4SS cinematic test path now avoids transient gameplay FOV inflation and
repeated per-poll camera writes.

---

## 2026-09-10 — UE4SS double-FOV and transition misclassification correction

### Scope

- Correct the supplied runtime regression showing gameplay FOV corruption and
  cinematic over-expansion.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks-all-policies-test.zip`

### Validation and evidence

- The log showed `DIALOGUE_START baselineFOV=120` during the constrained
  gameplay aspect transition; this was a false dialogue detection.
- The log showed cinematic `90` being transformed to `148.582725`; the formula
  had applied the aspect conversion twice.
- Dialogue detection now ignores constrained camera lifecycle transitions.
- Cinematic FOV conversion now uses one aspect conversion and waits for the
  target aspect state before applying it once.
- Runtime retest remains pending.

### Completed

- Removed the observed false dialogue trigger during gameplay reevaluation.
- Removed the observed double aspect conversion in cinematic FOV.
- Preserved authored first-person FOV as the cinematic source.

### Remaining / deferred / blocked

- Fresh runtime evidence is required for gameplay, dialogue and cinematic
  visual behavior.

### Patch summary

Corrected the cinematic Hor+ calculation and prevented gameplay camera
transitions from being mistaken for dialogue zoom.

### Changelog summary

UE4SS test module no longer double-converts cinematic FOV or seeds dialogue
zoom from a constrained gameplay transition.

---

## 2026-09-10 — UE4SS authored cinematic FOV tracking

### Scope

- Preserve instant cinematic correction while handling later native authored
  FOV updates without a per-poll write loop.

### Changed paths

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/README.md`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks-all-policies-test.zip`

### Validation and evidence

- The supplied log showed native camera logic overwriting the one-shot
  cinematic result back toward 90 degrees.
- Cinematic tracking now compares each observed FOV with the last transformed
  value and transforms only a new observed input.
- The instant ENTER correction is retained.
- Runtime retest remains pending.

### Completed

- Added self-write suppression for cinematic FOV tracking.
- Preserved the existing gameplay and dialogue isolation rules.

### Remaining / deferred / blocked

- Fresh runtime evidence is required to confirm cinematic shot changes,
  dialogue behavior and post-exit restoration.

### Patch summary

Retained instant cinematic correction and added bounded authored-FOV tracking
for later native camera updates.

### Changelog summary

UE4SS cinematic handling now follows new authored FOV inputs instead of
repeating the same transformed write every poll.

---

## 2026-09-10 — Unified ASI build wiring correction

### Scope

- Connect the current unified gameplay/cinematic/dialogue source to the
  canonical build command.

### Changed paths

- `build.cmd`
- `STALKER2CameraTweaks.asi`
- `ASI_UNIFIED_BUILD_TASK_PLAN.md`

### Validation and evidence

- `build.cmd` now compiles
  `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- Bounded local build completed successfully with VS 2022.
- Output name is now `STALKER2CameraTweaks.asi`, matching README.
- Runtime validation and game installation were not performed.

### Completed

- Fixed the build wiring that previously compiled only the superseded
  gameplay-only source.

### Remaining / deferred / blocked

- The new unified ASI still requires in-game validation on the current game
  executable.
- No runtime compatibility claim is made for the post-update game build.

### Patch summary

Canonical build now produces the documented unified gameplay/cinematic/dialogue ASI.

### Changelog summary

Unified ASI source is now included in the default build command.

---

## 2026-09-10 — Current-patch cinematic EXIT resolver update

### Scope

- Adapt the unified cinematic resolver to the current game executable after
  the previous EXIT signature reported zero matches.
- Keep gameplay and dialogue implementation unchanged.

### Changed paths

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `STALKER2CameraTweaks.asi`
- `ASI_CINEMATIC_EXIT_UPDATE_TASK_PLAN.md`

### Validation and evidence

- Current executable SHA-256:
  `E7B481A97C02D80581FAB0BECE940214A88EBE30211088A00129845A039F9293`.
- Current ENTER topology resolves uniquely with one match.
- New indexed EXIT topology resolves uniquely with one match at the static
  scan level and preserves the validated consumer/vcall continuation shape.
- VS 2022 local build completed successfully.
- New ASI size: `1,095,680` bytes.
- Follow-up runtime log showed the pattern was found but rejected by an
  off-by-one decoded-instruction length/callsite check; corrected the indexed
  `MOVSS` length to 6 bytes and rebuilt the ASI.
- Corrected ASI SHA-256:
  `69021D8758069F7EFE098B3C562A41E326A6DB0BF4BA88B789FB38854217DFB2`.
- Runtime injection and in-game behavior after the update were not validated.
- The game-folder ASI was not installed or modified by this task.

### Completed

- Added the current indexed `movss` EXIT pattern and semantic Zydis validation.
- Preserved the previous EXIT pattern as a fallback for older compatible
  topology.
- Rebuilt the documented unified `STALKER2CameraTweaks.asi`.

### Remaining / deferred / blocked

- Fresh in-game validation is required on the current executable.
- No new compatibility guarantee is claimed until the ASI is injected and the
  runtime log confirms both cinematic hooks.

### Patch summary

Updated cinematic EXIT resolution for the post-update indexed camera sample
path while retaining safe uniqueness and decoded-instruction checks.

### Changelog summary

Cinematic FOV hook resolution now recognizes the current game patch's EXIT
instruction topology.
## 2026-09-11 — Release preparation v0.5.2

- Scope: prepared the unified ASI release archive for v0.5.2; no runtime logic
  changes were introduced. Non-goals were research modules, old artifact
  cleanup, commit/tag/publish and new gameplay validation.
- Paths changed: `src/experimental_cinematic_21_9_combined_fix_204.cpp`,
  release-facing Markdown files, `release-assets/STALKER2CameraTweaks.asi`,
  `release-assets/STALKER2CameraTweaks.ini`,
  `release-assets/README.md`, and the v0.5.2 release archive. Evidence is in
  `research/reports/RELEASE_PREPARATION_v0.5.2.md`.
- Git state: branch `main`; working tree contained pre-existing unrelated
  changes and research artifacts. No Git state-changing operation was run.
- Validation: build succeeded; production ASI SHA-256 is
  `F55B17768625549D96E033FD79340A79DCA27FD9697264C37F4E7A6D6BAEEFB4`;
  archive SHA-256 is
  `73DD26FCAA29732E951728FE5EF01417AEB539435AA377D89F3858AC9D89A433`;
  archive allowlist contains exactly five files. Supplied runtime evidence is
  for the preceding current-build binary, not a new post-build injection.
- Completed: v0.5.2 metadata, compatibility wording, production asset refresh
  and archive construction.
- Remaining: user review and any explicit publication/commit approval.
- Deferred: runtime regression of the newly rebuilt binary; older-patch runtime
  testing; all experimental camera research.
- Blocked: none for package construction.
- Not runtime-validated: the newly rebuilt v0.5.2 binary in this task.
- Patch summary: updated release metadata for Steam 2.0.5 and preserved the
  existing unified resolver implementation.
- Changelog summary: v0.5.2 package update for the current Steam build, with
  static resolver portability documented separately for Steam 2.0.2–2.0.4.

## 2026-09-12 — Post-EXIT PCM/ViewTarget/CameraCache topology audit 2.0.5

- Scope: one bounded, read-only Ghidra audit around the confirmed cinematic
  handoff and camera writer. Non-goals were runtime probes, source changes,
  production behavior, broad scans and guessed calls.
- Paths changed: archived task plan at
  `research/deferred/POST_EXIT_PCM_VIEWTARGET_CAMERACACHE_TOPOLOGY_AUDIT_205_TASK_PLAN.md`.
  Research script, runner, evidence and report are under the workspace-level
  `02-Research/` tree, outside the canonical Git repository.
- Git state: branch `main`; pre-existing dirty changes and research artifacts
  were preserved; no Git state-changing operation was run.
- Validation: current 2.0.5 identity passed in Ghidra with SHA-256
  `E7B481A97C02D80581FAB0BECE940214A88EBE30211088A00129845A039F9293`, image
  base `0x140000000` and `.text` size `0x7CCD000`. The tracked headless run
  completed and no canonical Ghidra lock remained.
- Completed: audited `FUN_14318DCD4`, `FUN_14366F9AA`, `FUN_1453A7C88` and
  `FUN_140A9EF7C`, including one-level direct callers and the requested PCM/cache
  offset neighborhood.
- Remaining: PCM/ViewTarget/cache blend ownership and the physical post-EXIT
  interpolation producer remain unresolved.
- Deferred: further ASI static expansion and runtime probing until a new
  concrete topology anchor is available.
- Blocked: no new PCM topology anchor was recovered within the approved scope.
- Not runtime-validated: no behavior or production binary was changed or tested.
- Patch summary: added a version-gated read-only topology audit and durable
  evidence report; archived the completed plan as deferred.
- Changelog summary: none; production behavior and release assets unchanged.

## 2026-09-12 — Post-cinematic gameplay replay defer research build

- Scope: compile-time research branch that defers the existing gameplay aspect
  replay until three stable samples from the same source after confirmed native
  cinematic recovery. Non-goals were production behavior, cinematic/dialogue
  logic, FOV writes/clamps and direct EXIT timers.
- Paths changed: `src/experimental_cinematic_21_9_combined_fix_204.cpp`,
  `build-artifacts/research/build-post-cinematic-gameplay-replay-defer-test.cmd`,
  and the separate research artifact
  `build-artifacts/research/STALKER2CameraTweaks-PostCinematicGameplayReplayDeferTest205.asi`.
- Git state: branch `main`; unrelated dirty changes and existing research
  artifacts were preserved; no Git state-changing operation was run.
- Validation: VS 2022 build succeeded with
  `POST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST` and a 120 ms post-stability
  delay. Research ASI size is 1,107,968 bytes; SHA-256 is
  `CBCF0F0387E71587923AE049ABC1AA73C789181FEBA9716B01FE823C7D750F9E`.
  No production binary was overwritten or installed.
- Completed: same-source gate, three consecutive stable FOV samples,
  coordinator/cinematic/dialogue/FOV/aspect invalidation, optional post-stability
  delay and defer telemetry markers.
- Remaining: clean in-game validation of recovery timing, replay timing and
  visual framing.
- Deferred: promotion to production and any change to the released ASI.
- Blocked: none for the research build.
- Not runtime-validated: behavior of the research ASI in-game.
- Patch summary: added an isolated state-based post-cinematic replay defer test
  while preserving the existing replay implementation as the only apply path.
- Changelog summary: none; research-only artifact, not a release change.

## 2026-09-12 — Combined atomic cinematic/gameplay handoff candidate

- Scope: separate build combining the runtime-validated gameplay atomic replay
  and cinematic RecoveryStart atomic handoff branches.
- Non-goals: full staged replay after cinematic EXIT, new writes, timers,
  dialogue/cinematic formula changes, production replacement and publishing.
- Paths changed: `build-artifacts/research/build-combined-atomic-cinematic-gameplay-handoff-candidate.cmd`,
  `backlog/COMBINED_ATOMIC_CINEMATIC_GAMEPLAY_HANDOFF_CANDIDATE_TASK_PLAN.md`,
  and separate artifact
  `build-artifacts/research/STALKER2CameraTweaks-CombinedAtomicCinematicGameplayHandoffCandidate205.asi`.
- Git state: branch `main`; unrelated dirty work was preserved; no Git
  state-changing operation was run.
- Validation: VS 2022 build succeeded. Candidate ASI SHA-256 is
  `788D98DCEFC6B268D83858CD5D92BFDABD64B5C07577EC33877013F56A7A361A`.
  Production binary was not overwritten or installed.
- Completed: combined candidate build using the two already tested atomic
  compile-time paths.
- Remaining: one combined in-game regression run.
- Deferred: production promotion and release-asset changes.
- Blocked: none for the build.
- Not runtime-validated: combined candidate behavior.
- Patch summary: prepared a single candidate for integrated gameplay and
  post-cinematic atomic handoff validation.
- Changelog summary: none; research-only candidate, not a release change.

## 2026-09-12 — Atomic gameplay mutation deduplication refactor

- Scope: behavior-preserving refactor of the combined candidate. Gameplay and
  cinematic RecoveryStart triggers now share `ApplyGameplayAspectFixAtomic`.
- Non-goals: trigger/state-machine changes, staged-state removal, cinematic or
  dialogue behavior changes, production replacement and release changes.
- Paths changed: `src/experimental_cinematic_21_9_combined_fix_204.cpp`,
  `backlog/ATOMIC_GAMEPLAY_MUTATION_DEDUP_REFACTOR_TASK_PLAN.md`, and the
  rebuilt combined candidate under `build-artifacts/research`.
- Git state: branch `main`; unrelated dirty work was preserved; no Git
  state-changing operation was run.
- Validation: VS 2022 combined candidate build succeeded. `AppliedConstrainPass`
  remains referenced by the legacy staged path; it was not removed. Production
  binary was not overwritten or installed.
- Completed: duplicated atomic mutation logic consolidated; independent gates
  remain in place.
- Completed: runtime regression check passed; gameplay and RecoveryStart
  atomic applies each occurred once and no legacy staged replay markers were
  observed.
- Deferred: production promotion and release-asset changes.
- Blocked: none for the build.
- Runtime validation: PASS for the tested gameplay and cinematic transition.
- Patch summary: centralized the already validated atomic aspect/flags write.
- Changelog summary: none; research-only refactor, not a release change.

## 2026-09-12 — Cinematic FOV transition bypass Pass 1 trace

- Scope: observation-only trace built on the validated combined atomic
  candidate. The known FOV consumer now records CinematicActive and
  CinematicExiting phases with incoming FOV, state fields and caller data.
- Non-goals: writes, suppression, clamping, transition bypass, duration/alpha
  changes, new hooks and production/release changes.
- Paths changed: `src/experimental_cinematic_21_9_combined_fix_204.cpp`,
  `build-artifacts/research/build-cinematic-fov-transition-bypass-pass1.cmd`,
  `backlog/CINEMATIC_FOV_TRANSITION_BYPASS_PASS1_TASK_PLAN.md`, and separate
  trace artifact
  `build-artifacts/research/STALKER2CameraTweaks-CinematicFovTransitionBypassPass1-Trace205.asi`.
- Git state: branch `main`; unrelated dirty work was preserved; no Git
  state-changing operation was run.
- Validation: VS 2022 build succeeded. Trace ASI SHA-256 is
  `60A2CBC20F3518809585EC382A0341FE95FE5001326B771F7DDFC5BB9BAA9647`.
  Production binary was not overwritten or installed.
- Completed: bounded ENTER/EXIT consumer instrumentation.
- Remaining: one runtime trace run and transition classification.
- Deferred: any FOV transition bypass implementation and production promotion.
- Blocked: none for the trace build.
- Not runtime-validated: trace behavior in-game.
- Patch summary: added observation-only cinematic phase/state telemetry.
- Changelog summary: none; research-only trace, not a release change.

## 2026-09-12 — Gameplay fix atomicity research build

- Scope: compile-time gameplay-only research branch replacing the staged
  aspect/flags replay with one final `1.77778/0x4` write.
- Non-goals: cinematic and dialogue changes, production artifact changes, new
  hooks, timers, FOV changes and release packaging.
- Paths changed: `src/experimental_cinematic_21_9_combined_fix_204.cpp`,
  `build-artifacts/research/build-gameplay-fix-atomicity-test.cmd`,
  `backlog/GAMEPLAY_FIX_ATOMICITY_TEST_TASK_PLAN.md`, and separate artifact
  `build-artifacts/research/STALKER2CameraTweaks-GameplayFixAtomicityTest205.asi`.
- Git state: branch `main`; unrelated dirty work was preserved; no Git
  state-changing operation was run.
- Validation: VS 2022 build succeeded. Research ASI SHA-256 is
  `BC4F2CBAEFA2F3FE8FB7B8D4B48DB92A6E0DB46567CC9ADFF7113726578D5751`.
  Production binary was not overwritten or installed.
- Completed: guarded one-shot atomic gameplay replay and dedicated telemetry
  marker; staged replay remains the default without the research flag.
- Remaining: clean in-game validation of gameplay framing and transitions.
- Deferred: production promotion and release-asset changes.
- Blocked: none for the research build.
- Not runtime-validated: in-game behavior of this artifact.
- Patch summary: added a gameplay-only atomic final-state replay candidate.
- Changelog summary: none; research-only artifact, not a release change.

## 2026-09-12 — Post-cinematic atomic gameplay replay at recovery start

- Scope: compile-time research branch that waits for the first confirmed
  downward FOV sample during final `CinematicExiting`, then applies the final
  gameplay aspect/flags state once as `1.77778/0x4`.
- Non-goals: production changes, cinematic ENTER changes, dialogue changes,
  timers, repeated clamps, new hooks and hard-coded gameplay FOV.
- Paths changed: `src/experimental_cinematic_21_9_combined_fix_204.cpp`,
  `build-artifacts/research/build-post-cinematic-gameplay-atomic-exit-handoff-test.cmd`,
  and separate artifact
  `build-artifacts/research/STALKER2CameraTweaks-PostCinematicGameplayAtomicExitHandoffTest205.asi`.
- Git state: branch `main`; unrelated dirty work was preserved; no Git
  state-changing operation was run.
- Validation: VS 2022 build succeeded. Research ASI SHA-256 is
  `0CF023173DCE9591677DE5996B35534BDBCD137872E5ED74519BFDA66C54E2B8`.
  Production binary was not overwritten or installed.
- Completed: first-downward-sample gate, same-source guard, dialogue/coordinator
  cancellation and `phase=RecoveryStart` telemetry.
- Remaining: one clean in-game comparison against the recovery-complete atomic
  artifact.
- Deferred: production promotion and release-asset changes.
- Blocked: none for the research build.
- Not runtime-validated: in-game behavior of this artifact.
- Patch summary: moved the one-shot atomic gameplay projection apply from the
  end of native recovery to the first confirmed native FOV descent.
- Changelog summary: none; research-only artifact, not a release change.

## 2026-09-12 — Post-cinematic gameplay replay at recovery research build

- Scope: separate compile-time research variant that applies the final gameplay
  aspect/flags state in the same writer invocation that confirms native FOV
  recovery. Non-goals were pre-recovery writes, timers, new hooks, FOV changes
  and production behavior.
- Paths changed: `src/experimental_cinematic_21_9_combined_fix_204.cpp`,
  `build-artifacts/research/build-post-cinematic-gameplay-replay-at-recovery-test.cmd`,
  and separate artifact
  `build-artifacts/research/STALKER2CameraTweaks-PostCinematicGameplayReplayAtRecoveryTest205.asi`.
- Git state: branch `main`; unrelated dirty work was preserved; no Git
  state-changing operation was run.
- Validation: VS 2022 build succeeded with the recovery-time research flags.
  Research ASI size is 1,108,992 bytes; SHA-256 is
  `1B03CEBA06E3D13632F90BA8D475AB78DD1FF678A3BE592AF0784F5A23BD731A`.
  Production binary was not overwritten or installed.
- Completed: recovery-complete atomic apply path and explicit phase telemetry.
- Remaining: one clean in-game comparison against the deferred atomic build.
- Deferred: production promotion and release-asset changes.
- Blocked: none for the research build.
- Not runtime-validated: in-game behavior of the recovery-time artifact.
- Patch summary: moved the research atomic apply to the earliest boundary where
  native FOV recovery is already confirmed.
- Changelog summary: none; research-only artifact, not a release change.

## 2026-09-12 — Post-cinematic gameplay replay atomicity research build

- Scope: compile-time research branch layered on the deferred handoff gate. It
  applies final aspect/flags `1.77778/0x4` in one writer invocation after three
  stable same-source samples and the existing 120 ms research delay. Non-goals
  were production changes, new hooks, FOV changes and renderer intervention.
- Paths changed: `src/experimental_cinematic_21_9_combined_fix_204.cpp`,
  `build-artifacts/research/build-post-cinematic-gameplay-replay-atomicity-test.cmd`,
  and separate artifact
  `build-artifacts/research/STALKER2CameraTweaks-PostCinematicGameplayReplayAtomicityTest205.asi`.
- Git state: branch `main`; unrelated dirty work was preserved; no Git
  state-changing operation was run.
- Validation: VS 2022 build succeeded with the defer and atomicity defines.
  Research ASI size is 1,108,992 bytes; SHA-256 is
  `BD506C52CE44919063C06EBC1FFF6458211BC24F2AE6813771C2C99E796DA7CA`.
  Production binary was not overwritten or installed.
- Completed: stable-sample counter now stops at `3/3`; atomic research path,
  final-state write and telemetry marker were added behind compile-time flags.
- Remaining: one clean in-game test is required to compare visual framing and
  confirm whether the intermediate projection jump disappears.
- Deferred: promotion to production and any release-asset change.
- Blocked: none for the research build.
- Not runtime-validated: in-game behavior of the atomicity artifact.
- Patch summary: added a one-invocation final gameplay aspect/flags test while
  preserving the existing deferred and production replay paths.
- Changelog summary: none; research-only artifact, not a release change.
