# Cinematic Native Two-Pass Aspect Transition Experiment

Status: Blocked/Closed — Batch 1C completed. The native gameplay two-pass can be triggered on the shared cinematic object, but correct cinematic FOV remains coupled to the constrained `16:9` render state; aspect-only restoration to native `32:9` does not preserve it. Batch 2 automatic implementation is not justified.

## Objective

Determine whether the native gameplay two-pass aspect/mode transition can be reproduced for the cinematic camera so the game recalculates cinematic FOV itself, without manually converting FOV or writing a transformed value into durable camera state.

## Established evidence and current state

- Current executable is game version 2.0.3, SHA-256 `81961B7281C7CF528CE49C549CE086FCC684BD676F32FAF042BC743D939E3C69`.
- `STALKER2GameplayAspectFix.asi` hooks the validated gameplay camera-writer path at the `MOVSS [RBX+0x30], XMM0` boundary.
- The stable hook observes gameplay source fields at `source+0x254` and `source+0x259`, then uses a two-pass state machine across subsequent camera-writer calls.
- Observed gameplay state values include aspect `32:9` and mode/flag values `0x4`/`0x5`; their exact UI labels remain observed evidence, not a fully proven semantic naming contract.
- The stable gameplay hook does not calculate or overwrite player FOV. Runtime logs show `secondaryFOV=90` when the game setting is 90, while `primaryFOV` is a transient/current camera value.
- Experimental cinematic A/B setters are separate hook sites and already provide native `32:9` cinematic rendering.
- Manual dynamic cinematic Hor+ conversion (`90 → 126.869896`) produces correct cinematic framing but causes a visible post-cutscene transition; the transient/durable split experiment removed Hor+ framing and was rejected.
- The cinematic object has a confirmed aspect field at `+0x254`; equivalence of its nearby mode/flag fields to gameplay `+0x259` is not established.
- The goal is to reproduce native transition semantics in the cinematic object, not to copy offsets or assume identical field meaning.

### Batch 1 preliminary result

- Source re-audit confirms that the stable gameplay hook does not call a UI setter or calculate FOV. It modifies gameplay source state at the validated camera-writer boundary and relies on subsequent native camera updates.
- The current cinematic A/B implementation confirms an aspect write to cinematic `+0x254`, but does not yet establish a cinematic companion mode/flag field or equivalent update cadence.
- Gameplay `+0x259` semantics must not be transferred to the cinematic object by offset similarity alone.

## Approved scope

- Research and, only after Batch 1 approval, a bounded experimental diagnostic implementation for cinematic native two-pass aspect transition.
- Permit one controlled aspect-only manual probe using the already validated cinematic object and `+0x254`, without assuming or writing a cinematic companion mode field.
- Compare gameplay and cinematic state fields, write order, object ownership and update-call behavior.
- Remove or disable the manual cinematic `90 → 126.87` conversion in any diagnostic variant used for this experiment.
- If the contract is proven, test a manual cinematic transition using original game-owned update cycles and validated cinematic object fields.
- Keep stable gameplay behavior and the confirmed A/B native-render path unchanged.

## Explicit non-goals

- No changes to `STALKER2GameplayAspectFix.asi` behavior.
- Read-only diagnostic logging in the stable gameplay hook is permitted for object-identity correlation; it must not alter its transition behavior.
- No reuse of gameplay offsets in the cinematic object solely because the numeric offsets match.
- No hard-coded FOV, fixed multiplier, FOV replay, delayed timing, frame counter or post-cutscene correction.
- No direct writes to unvalidated cinematic fields.
- No UI automation or assumption that the pause/settings menu is available during a cinematic.
- No release integration, compatibility claim or replacement of the stable gameplay ASI.

## Expected files or areas

- `src/gameplay_aspect_fix.cpp` — read-only reference during Batch 1 unless a separate approved change is required.
- `src/cutscene_letterbox_fix.cpp` — experimental diagnostic implementation only after the static gate passes.
- `02-Research/Ghidra/ghidra-scripts/` — reusable bounded research scripts, if needed.
- `build-artifacts/test-scripts/` — diagnostic build script, if needed.
- `build-artifacts/test-asi/` — disposable diagnostic ASI output.
- This task plan and bounded research notes.

## Batches

### Batch 1 — Static state-contract comparison

- Document the exact gameplay writes to `+0x254/+0x259`, their preconditions, order and subsequent camera-writer calls.
- Identify the exact cinematic object and confirmed writes performed by A/B.
- Determine whether the cinematic object has a validated mode/state field that participates in an analogous transition.
- Determine whether the cinematic A/B setters are called repeatedly during one cinematic lifecycle or only during setup.
- Confirm that the cinematic object receives at least two semantically valid update opportunities during the same cinematic lifecycle, sufficient to express the validated two-pass transition without synthetic timing.
- Keep gameplay `0x4/0x5` labels provisional unless independent evidence proves their UI semantics.

Validation: Ghidra/static evidence tied to the 2.0.3 executable; no source edits, build or runtime claim. If no compatible cinematic state contract or sufficient cinematic update cadence is found, close the experiment as blocked.

### Batch 1A — Aspect-only manual probe

- This is a diagnostic exception to the full two-pass gate, not approval for automatic implementation.
- Use only a cinematic object pointer captured through a validated A/B path and verify that it remains readable and belongs to the current cinematic context.
- Do not read or write an unvalidated companion field and do not enable the manual `90 → 126.87` conversion.
- Provide two manual triggers, subject to confirming they do not conflict with existing markers: `F6` writes the native cinematic baseline aspect `16:9` and the experimentally selected observed mode value `0x4`; `F11` restores the live display aspect (`32:9` at `5120x1440`) without changing the flag. If `F11` remains assigned to an existing marker, use another free key instead.
- Log object identity, old/new `+0x254`, cinematic global FOV, and any subsequent A/B or camera-state activity.
- If the object becomes unreadable, changes identity or the lifecycle is no longer cinematic, refuse the write. Restore the live aspect before leaving the diagnostic context when safe.

Validation: manual runtime observation only. A successful visual response is evidence that aspect-only mutation has an effect, not proof of a safe automatic two-pass contract.

#### Batch 1A implementation result

- Added a separate `CINEMATIC_ASPECT_PROBE` diagnostic build path.
- The probe disables the manual cinematic FOV hook and captures the latest validated B-setter cinematic object.
- Probe hotkeys are sequentially numbered to avoid confusion: `F6` marks cinematic start, `F7` records the one-shot trigger arm marker, `F8` restores only the captured object's native display aspect after the two-pass has visibly produced the constrained state, and `F9` starts the 10-second observation window. The probe no longer writes flags; the separate gameplay diagnostic ASI owns the boundary write. The restore does not modify flags or FOV.
- Build succeeded with output `build-artifacts/test-asi/STALKER2ExperimentalCinematicAspectProbe.asi`.
- Manual runtime probe completed at `5120x1440`: `F6` successfully produced the constrained `16:9`/letterbox presentation, while `F11` restored native `32:9` rendering but visually matched gameplay without the stable aspect module rather than producing cinematic Hor+ framing.
- The probe therefore confirms that cinematic `+0x254` is writable and affects the render aspect, but an aspect-only write does not by itself trigger the gameplay-style native FOV recalculation for the cinematic camera.
- Correlated logs show the manual `16:9` and live `32:9` writes were followed by gameplay camera-writer observations with `primaryFOV=90`, `secondaryFOV=90`, and `flags=0x5`; the aspect changed, but no native FOV transition was observed. This supports a combined mode/flag or companion-state requirement while not proving that the cinematic and gameplay objects are identical.
- The follow-up flag probe wrote cinematic `+0x259` from `0x5` to `0x4` successfully, but the gameplay writer observed `flags=0x5` immediately afterward for the `16:9` state. No FOV change or native recalculation was correlated with either manual trigger. This rejects the direct `cinematic +0x259` write as an effective reproduction of the gameplay transition; object identity/propagation remains unresolved.
- No companion mode/state field or two-pass cinematic update cadence was established. Automatic two-pass implementation remains blocked.

### Batch 1B — Gameplay/cinematic object correlation

- Add read-only logging at the existing gameplay camera-writer mode-change observation.
- Log gameplay source pointer, raw `source+0x254`, raw `source+0x259`, primary FOV, secondary FOV and selector alongside the existing aspect/flag values.
- Compare the logged gameplay source pointer with the captured cinematic A/B object pointer from the aspect-only probe.
- Do not modify gameplay aspect/FOV logic, transition state or write behavior.

Validation: one manual run using the stable gameplay ASI and the diagnostic cinematic probe. The result may establish object identity/propagation correlation only; it does not authorize a new write experiment.

Current implementation status: read-only source-pointer and raw gameplay-state logging was added to `LogCameraModeChange`; stable build succeeded. The hook now also emits paired `PRE`/`POST` snapshots with one sequence per mode-changing invocation and the internal replay state, so mutations by this hook can be separated from mutations before or after it. Runtime correlation is pending.

Latest runtime result: the cinematic A/B object and stable gameplay source were identical (`0x2314BA76700`). The probe then changed only that object's flags from `0x5` to `0x4` while preserving native `32:9`. The stable hook observed `32:9/0x4` in `PRE`, wrote `32:9/0x5` and entered `AppliedConstrainPass` in `POST`, then on the next invocation restored `16:9/0x4` and entered `Complete`. This confirms the stable gameplay two-pass machinery can be activated on the cinematic camera object through the correct initial state, without manual FOV conversion. Visual cinematic outcome is still pending separate confirmation.

### Batch 2 — Diagnostic two-pass implementation, only if Batch 1 passes

- Create a separate experimental diagnostic variant with the manual cinematic Hor+ FOV conversion disabled.
- Implement only the validated cinematic aspect/mode transition using the proven object, fields and original update path.
- Use manual hotkeys that do not conflict with existing markers and allow each confirmed transition phase to be initiated and observed separately. The number and role of triggers must follow the Batch 1 contract; do not assume two independent setters or two keys if the native path uses one trigger followed by a normal update.
- Install only after all required targets and field/context checks pass; refuse safely on ambiguity.
- Do not modify stable gameplay logic or the confirmed A/B setter contract.

Validation: source review, build validation and diagnostic logging. Build success is not runtime evidence.

### Batch 1C — One-shot camera-boundary trigger probe

- Build a separate gameplay diagnostic variant in which `F7` only arms an atomic one-shot trigger.
- On the first validated camera-writer invocation with native `32:9`, `flags=0x5` and `WaitingForAutomaticUpdate`, change only `source+0x259` to `0x4`, clear the arm, and continue through the existing gameplay state machine.
- Do not use a timer, repeated flag writes, manual FOV conversion or aspect writes. This diagnostic ASI is separate from the stable gameplay output.
- Keep the cinematic probe's `F8` as an aspect-only restore after the two-pass visibly reaches the constrained state; do not modify flags during that restore.

Validation: build the separate diagnostic gameplay ASI and run one manual comparison with the cinematic probe. This probe tests boundary synchronization only and does not authorize release integration.

Runtime result: one-shot `F7` was consumed at the validated camera-writer boundary on the same captured source object. The gameplay hook observed `32:9/0x4`, completed the constrained pass, then completed the Auto restore on the next invocation. The probe's later `F8` changed only `+0x254` from `16:9` back to `32:9`; the gameplay hook then observed `32:9/0x5` without reopening the state machine. The logs confirm the intended state sequence; visual cinematic framing and post-cutscene behavior still require user observation.

Visual result: the cinematic initially rendered natively at `32:9` without side bars. After the one-shot two-pass, `F7` visibly changed it to `16:9` with side bars. `F8` restored the `32:9` render aspect, but the cinematic FOV remained incorrect. This rejects the aspect-only restore as a complete solution: restoring `+0x254` after `Complete` does not trigger the required cinematic FOV/projection recalculation.

Future diagnostic requirement: record the actual game render resolution alongside aspect/state markers where a reliable game-owned or external telemetry source is available. The observed controls are `16:9 = 2560x1440` and `32:9 = 5120x1440`; desktop/display resolution must not be mislabeled as render resolution.

### Batch 3 — Manual cinematic comparison

- Test at `5120x1440` with stable gameplay ASI and the experimental diagnostic ASI.
- Compare vanilla, current exact-Hor+ behavior and the native two-pass experiment where practical.
- Confirm whether cinematic framing becomes correct without a synthetic FOV value.
- Confirm whether post-cutscene gameplay avoids the `126.87` overshoot.
- Record failures, object/state values and whether the transition must be rejected.

Validation: manual runtime evidence only. Do not claim compatibility beyond the tested executable identity and scenario.

## Risks and rollback or safe failure

- A cinematic field may look structurally similar to gameplay state while having different ownership or lifetime.
- A partial transition may leave cinematic camera state inconsistent. Resolve and validate the complete candidate contract before installation.
- If the first pass succeeds and the restore pass cannot be installed or verified, do not leave a partial transition active; disable the diagnostic feature and restore the original A/B-only behavior.
- Unknown object, invalid pointer, unexpected field value or missing continuation must result in a no-op/refusal, not a guessed write.
- If cinematic framing is unchanged or gameplay regresses, discard the diagnostic variant without changing stable gameplay code.

## Stop conditions and phase gates

- No automatic two-pass source edit or build before Batch 1 establishes a cinematic mode/state contract. Batch 1A/1C diagnostic probes were permitted only under their narrower safety contracts.
- Stop if only `+0x254` is confirmed and no safe companion state/transition semantics can be established.
- Stop if the only viable approach is manual FOV conversion, fixed baseline, timing workaround or direct unvalidated memory writes.
- Stop if the transition cannot be rolled back atomically or cannot be limited to cinematic context.
- Do not promote the experiment to a release feature without manual runtime evidence and a separate implementation decision.

Final phase result: the shared-object and boundary-trigger hypotheses were confirmed, but the complete cinematic result was rejected. The two-pass mechanism produces a real `16:9`/`2560x1440` constrained render; restoring `32:9`/`5120x1440` through `+0x254` alone restores the wrong cinematic FOV. Reopen only with new contradictory or external structural evidence.

## Expected final Git review

After scoped implementation and manual testing, inspect Git status, relevant diff and recent commits; compare changed paths with this plan and classify completed, remaining, deferred, blocked and not-runtime-validated items. Update `backlog/TASKLOG.md` only after the scoped validation and plan comparison are complete.
