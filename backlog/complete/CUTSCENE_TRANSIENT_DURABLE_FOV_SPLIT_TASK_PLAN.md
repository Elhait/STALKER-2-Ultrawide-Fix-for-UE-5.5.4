# Cutscene Transient/Durable FOV Split Task Plan

Status: Closed/Rejected — Variant B removed the gameplay-visible `126.87` state but did not preserve cinematic Hor+ framing.

## Objective

Determine whether Hor+ cinematic framing can be preserved using transient converted FOV while the original game-owned FOV is written to durable `state + 0x54`, preventing the converted value from entering the subsequent camera-state handoff. The preferred source is the user's selected gameplay FOV, if the executable's cinematic source path is proven to use that setting.

## Established evidence and current state

- Target executable: S.T.A.L.K.E.R. 2 version 2.0.3, analyzed in `Dump/STALKER2-Ghidra`.
- A/B letterbox hooks are runtime-confirmed to produce native `5120×1440` / `32:9` cinematic rendering.
- Current dynamic Hor+ conversion changes game-owned cinematic FOV `90` to approximately `126.869896`; framing is visually correct during the cinematic.
- The transformed value reaches `FUN_1431D2094 → FUN_1431D23AE` and is written to durable `state + 0x54`.
- After the cinematic, a different state object receives a gameplay-like FOV around `90.65574` and the camera visibly returns from the overshoot.
- The exact downstream projection consumer and refresh owner remain unresolved. This experiment tests whether preventing the specific transformed durable write removes the visible jump without requiring either owner.
- The current `90` value is only the first tested scene's observed input, not a matching requirement; other cinematic entries may use different game-owned FOV values.

## Approved scope

- Add a diagnostic-only Variant B to the experimental cutscene ASI or its dedicated test build.
- Preserve the transient converted value for the validated cinematic call path.
- For only the matching cinematic conversion event, write the original source FOV to `state + 0x54` instead of the converted value.
- Log source FOV, converted transient FOV, durable-write value, state identity, sequence and cinematic/exit markers.
- Compare Variant A and Variant B in the same 2.0.3 / `5120×1440` manual scenario.

## Explicit non-goals

- Do not modify `STALKER2GameplayAspectFix.asi` or stable gameplay behavior.
- Do not permanently force the original FOV for every `state + 0x54` update.
- Do not alter later gameplay-transition updates such as the observed `90.65574` write.
- Do not change A/B letterbox hooks, the Hor+ formula or live-aspect calculation in Variant A.
- Do not add timers, delayed replay, frame-count gates, synthetic refresh calls or hard-coded FOV multipliers.
- Do not claim that Variant B is a final fix before manual comparison and Git review.

## Expected files and areas

- `src/cutscene_letterbox_fix.cpp` under the existing diagnostic/experimental build boundary.
- Existing diagnostic build script/output location under `build-artifacts/test-scripts/` and `build-artifacts/test-asi/`.
- This task plan and, after completion, the factual `backlog/TASKLOG.md` entry.
- No stable release asset or release documentation changes are expected.

## Batches

### Batch 1 — Split-state implementation contract

- Define the exact event/data-flow matching condition for the converted cinematic event: source FOV `X`, converted FOV `Y`, live aspect and active cinematic context as available from the validated path. Matching must not depend on `source == 90` or any other numeric constant.
- Preserve the converted value in the transient register/value consumed by the cinematic call.
- Create a pending split event only after the validated cinematic conversion; consume it exactly once at the corresponding durable `state + 0x54` write and clear it immediately afterward.
- Substitute source `X` only at the corresponding durable write for that pending event. All writes not causally correlated with a validated cinematic Hor+ conversion event retain original behavior, regardless of their numeric FOV value.
- Establish the lifetime and identity correlation from the call/data-flow contract, object/context and expected continuation; do not use a timer or indefinite pending state.
- Verify whether the cinematic source `X` follows the user-selected gameplay FOV across at least one changed settings value; do not assume the current cinematic `90` is the gameplay setting.
- Verify register, continuation and return semantics; the diagnostic branch must not invoke downstream functions or replay the state update.
- Keep Variant A available for direct A/B comparison.

Validation: static source review, exact 2.0.3 instruction/decode validation, event-correlation review and successful diagnostic build.

Batch 1 result:

- Added a diagnostic-only split-state branch under `CUTSCENE_FOV_SPLIT_STATE`; the normal experimental and stable gameplay builds remain unchanged.
- The branch records a thread-local validated cinematic conversion event (`source X`, `converted Y`) and consumes it at the first matching durable state-writer encounter.
- On a matching event, transient `Y` remains in the cinematic path while the durable `state + 0x54` write receives source `X`; all non-matching writes pass unchanged.
- Pending state is consumed immediately on the first state-writer encounter. A mismatch becomes a safe no-op rather than an indefinite wait or timer-based replay.
- Built `build-artifacts/test-asi/STALKER2ExperimentalLetterboxFovSplitTrace.asi` successfully with the diagnostic and split-state defines.
- Rebuilt the Variant A diagnostic comparison ASI `build-artifacts/test-asi/STALKER2ExperimentalLetterboxTransitionTrace.asi` successfully without the split-state define.
- Batch 1 build validation is complete; Batch 2 manual runtime comparison is complete and recorded below.

### Batch 2 — Manual A/B runtime comparison

Run the same new-game cinematic scenario at native `5120×1440`:

- Variant A: transient and durable values both follow the current implementation (`90 → 126.87`).
- Variant B: transient path receives `Y`, matching durable write receives the corresponding source `X`.

Record:

- cinematic framing during the scene;
- whether the post-cutscene FOV jump remains;
- transition duration and visible return toward gameplay FOV;
- state pointer and durable-write values;
- whether the later gameplay-owned write (for example `90.6557`) remains unchanged;
- any gameplay or later camera-state regression.

Validation: user-observed manual runtime behavior and diagnostic logs for both variants.

Batch 2 result:

- Variant B matched the validated conversion event: transient FOV remained `126.869896`, while the corresponding durable `state + 0x54` write received source `90`.
- The subsequent gameplay camera log recorded `primaryFOV=90` rather than `126.869896`, confirming that the transformed value no longer leaked into the observed gameplay camera path.
- Manual testing showed that Variant B's cinematic framing was no longer the same correct Hor+ framing as Variant A; it visually resembled gameplay without `STALKER2GameplayAspectFix`, with a vertically constrained view. The transition behavior was reduced to a roughly normal-range change, but the required cinematic framing was lost.
- The split-state mechanism therefore removes the visible high-FOV contamination but fails the primary requirement of preserving cinematic Hor+ framing.

### Batch 3 — Decision gate

- If Variant B preserves Hor+ framing and removes or materially reduces the jump without later regression, prepare a separate bounded implementation plan for the experimental feature.
- If Variant B loses Hor+ framing, reject the split-state hypothesis and retain the current experimental limitation.
- If Variant B changes unrelated state writes or creates instability, roll it back and reject the design.

Validation: compare results with this plan, perform read-only Git review and record completed, remaining, deferred, blocked and not-runtime-validated items.

Batch 3 result:

- Variant B is rejected as a final architecture because it cannot preserve the required cinematic Hor+ framing while keeping the durable state at the original FOV.
- The visual result indicates that the transient converted value alone does not reach the effective cinematic projection path; the transformed value must pass through the camera-state machinery for the observed Hor+ framing.
- The visual result indicates that the transient converted value alone does not reach the effective cinematic projection path; the transformed value must pass through the camera-state machinery for the observed Hor+ framing.
- The visual result indicates that the transient converted value alone does not reach the effective cinematic projection path; the transformed value must pass through the camera-state machinery for the observed Hor+ framing.
- The experiment provides strong causal evidence that the transformed durable value participates in cinematic framing, even though the exact downstream consumer remains unidentified.
- Keep the diagnostic implementation as research evidence until the next approved task decides whether to archive or remove it; do not use this diagnostic ASI as a release build.

## Risks and rollback / safe failure

- The transformed value may be required by the game-owned state machinery for cinematic projection; Variant B must fail closed if the specific match cannot be established.
- Matching must be event-specific and data-flow-correlated, not a global `state + 0x54` rewrite. The source/converted relationship, cinematic context and corresponding state object must be validated before substitution.
- A pending split event must be consumed by exactly one corresponding durable write and cleared; if correlation is ambiguous or the write does not arrive through the expected path, Variant B must fail/no-op rather than wait indefinitely.
- If either the transient path or the corresponding durable write cannot be distinguished safely, do not install Variant B.
- Any partial hook installation must roll back before reporting failure.
- Revert to Variant A or remove the diagnostic branch if manual testing shows framing loss, state corruption, instability or unrelated gameplay changes.

## Stop conditions and phase gates

- Stop before runtime testing if Variant B cannot preserve the original machine/control-flow contract.
- Stop if the implementation would affect non-matching `state + 0x54` writes.
- Stop if the cinematic source cannot be related to the user-selected gameplay FOV when that behavior is part of the intended contract.
- Stop if the test requires timing, delayed replay or a hard-coded compensation value.
- Do not promote Variant B to a release build without separate manual validation and an updated implementation decision.

## Expected final Git review

After implementation and manual comparison, inspect Git status, relevant diff and recent commits; compare actual changed paths with this plan and classify completed, remaining, deferred, blocked and not-runtime-validated items. Update `backlog/TASKLOG.md` only after the manual test and plan comparison are complete.
