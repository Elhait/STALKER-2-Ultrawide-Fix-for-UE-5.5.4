# Cinematic State Transition Probe 2.0.4 — Task Plan

## Objective

Compare the validated cinematic camera state immediately after ENTER, at a user-marked steady cinematic moment, and after the already-known manual combined FOV/aspect write using one combined experimental ASI with a single ENTER hook.

## Established evidence and current state

- 2.0.4 cinematic ENTER and native EXIT lifecycle anchors are runtime-confirmed.
- The immediate post-setter write can change aspect but does not make `+0x230` FOV visually effective.
- The same combined FOV/aspect write becomes visually effective later during active cinematic state.
- The previous automatic post-enter anchor search is closed for the currently available candidates.
- The remaining question is whether a state/ownership transition occurs between those two moments.

## Approved scope

- Create one combined 2.0.4 transition/manual probe with a single ENTER hook.
- Capture the validated `inner` object and a bounded state region around `+0x200..+0x2D0` at ENTER.
- Provide a manual `MARK_READY` action that records the same state and up to two already-readable pointer-linked objects.
- Provide explicit manual controls for the known combined write and baseline restore in the same artifact, so no second ASI hooks the ENTER setter.
- Report pointer, bit/flag and float changes while suppressing unchanged values and obvious high-frequency noise.
- Stop after one A/B/C sequence per run.

## Explicit non-goals

- No broad memory diff or executable scan.
- No FOV ownership search, new post-enter anchor search or legacy offset recovery.
- No gameplay ASI changes.
- No automatic FOV/aspect writes, timers, polling, VEH, PAGE_GUARD or debugger sampling; writes occur only from explicit manual hotkeys.
- No interpretation of a changed value as causal without repeated evidence.

## Expected files or areas

- `src/cinematic_state_transition_probe_204.cpp`.
- `build-artifacts/test-scripts/build-cinematic-state-transition-probe-204.cmd`.
- `src/cinematic_combined_transition_manual_probe_204.cpp`.
- `build-artifacts/test-scripts/build-cinematic-combined-transition-manual-probe-204.cmd`.
- Experimental output under `build-artifacts/test-asi/` only.
- This task plan.

## Batches

### Batch 1 — Read-only probe implementation

- Reuse only validated 2.0.4 ENTER/EXIT identity and safe snapshot helpers.
- Add bounded snapshots and manual mark controls.
- Validation: compile success, unique current-build anchor validation, no camera writes and fail-closed setup.

Batch 1 result: created `src/cinematic_state_transition_probe_204.cpp` and `build-artifacts/test-scripts/build-cinematic-state-transition-probe-204.cmd`. The probe validates the current 2.0.4 ENTER signature and EXIT bytes, captures a bounded `inner+0x200..0x2D0` snapshot, preserves ENTER identity for manual comparisons, and registers read-only F9–F11 markers to avoid the manual probe's F1–F8 controls. A local review removed an accidental instruction-skip from the first draft so the native ENTER setter executes normally. The corrected build succeeded and produced `build-artifacts/test-asi/STALKER2CinematicStateTransitionProbe204.asi`. No runtime or visual validation has been performed.

The separate two-ASI arrangement was rejected after runtime validation showed that the manual probe patched the shared ENTER setter before the state probe validated it, producing `ENTER bytes mismatch`. The combined artifact is now the approved implementation for the same A/B/C experiment.

Combined implementation result: created `src/cinematic_combined_transition_manual_probe_204.cpp` and `build-artifacts/test-scripts/build-cinematic-combined-transition-manual-probe-204.cmd`. The artifact uses one validated ENTER hook, one EXIT hook, bounded state snapshots and explicit F1–F8 manual controls. It builds successfully as `build-artifacts/test-asi/STALKER2CinematicCombinedTransitionManualProbe204.asi`. Runtime and visual validation remain pending.

Hotkey diagnostic correction: the first combined runtime log showed valid ENTER/EXIT hooks but no manual events. The artifact now logs the result and error code for each `RegisterHotKey` call; camera logic and explicit key assignments are unchanged. The rebuilt artifact compiles successfully. Runtime validation remains pending.

Input-layer correction: runtime showed all `F1–F8` global hotkey registrations failing with Windows error `1409`, while the game directory contained only the combined ASI. The combined artifact now uses `WH_KEYBOARD_LL` only to enqueue F1–F8 command IDs; a worker thread performs the existing snapshot/manual logic. The keyboard callback performs no camera writes. The artifact was rebuilt successfully; runtime validation of the new input path remains pending.

Batch 2 runtime result: the combined artifact installed successfully with `keyboard-hook-installed=true`. The same `inner=0x12A1E9AD9C0` was retained through ENTER, manual snapshots, the explicit `F3` write and EXIT. Commands `F1`, `F2`, `F3`, `F6` and `F7` were all received. Between the ENTER snapshot (`fov=90.65574`, `aspect=3.5555556`, `flags=0x05`) and the first manual snapshot, the native state had transitioned to `fov=90`, `aspect=1.7777778`, `flags=0x05`; the changed bytes at `+0x200..+0x206` appear pointer/animation noise, while `+0x230` and `+0x254` changed as meaningful scalar state. The explicit `F3` write produced `fov=127.3927`, `aspect=3.5555556`, `flags=0x05` on the same inner and was logged as successful. `F6` restored `fov=90.65574` but also restored the pre-setter wide aspect because the ENTER snapshot precedes the native aspect commit; this confirms the previously observed F6 semantic issue but does not invalidate the transition evidence. No causal owner is promoted from this single run; repeatability and the exact transition timing remain open.

### Batch 2 — Isolated runtime A/B/C capture

- Run with only this probe enabled.
- Capture ENTER, user-marked steady state, and post-manual-write state.
- Validation: same inner identity, pointer relationships, bounded changed-field report and user visual notes.

### Batch 3 — Evidence classification

- Classify changes as repeatable transition evidence, inconclusive noise or no structural transition.
- Do not create a write-test or implementation hook from this probe alone.

## Risks and safe-failure behavior

- Refuse snapshots when the captured item/inner identity is invalid or unreadable.
- Never dereference unvalidated pointers; limit pointer traversal to known readable relationships and bounded counts.
- Manual controls must only snapshot and log; no camera fields are written.
- Disable all other ASIs during runtime capture.

## Stop conditions and phase gates

- Stop after one bounded capture unless a repeat is needed to test a specific contradictory result.
- Stop immediately if pointer identity cannot be preserved safely or snapshot logic risks game execution.
- Do not promote fields based on one changed value or numeric similarity alone.
- If no repeatable A→B structural transition appears, close this state-diff method without broadening the region.

## Final review requirements

- Perform read-only Git status and diff review after implementation changes.
- Compare actual paths with this plan and classify completed, remaining, deferred, blocked and not-runtime-validated items.
- Keep final TASKLOG update deferred until the scoped runtime result and plan comparison are complete.
