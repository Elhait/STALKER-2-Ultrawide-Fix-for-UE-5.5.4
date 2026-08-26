# Task Log

This document records changes and completed work in the project. It serves as a changelog, including research, documentation and organizational tasks that do not necessarily modify the mod.


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
