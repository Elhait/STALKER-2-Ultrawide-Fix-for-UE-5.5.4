# Task Log

This document records evidence-backed research outcomes, completed implementations and exceptional repository housekeeping. Ordinary README, release-text, index and link-maintenance edits are intentionally not logged here.

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
