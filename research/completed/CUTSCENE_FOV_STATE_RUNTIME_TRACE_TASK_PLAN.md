# Cutscene FOV State Runtime Trace Task Plan

Status: Bounded research complete — durable state and handoff are confirmed, but no usable downstream blend/projection consumer was established.

## Objective

Determine whether the durable cutscene FOV state at `state + 0x54` is read or propagated through the post-write virtual update path during the observed post-cutscene FOV overshoot and subsequent return to gameplay FOV.

## Established evidence and current state

- Target executable: S.T.A.L.K.E.R. 2 version 2.0.3, analyzed in `Dump/STALKER2-Ghidra`.
- The transformed cinematic FOV reaches `FUN_1431D23AE` and is written to `state + 0x54`.
- Related fields `state + 0x50` and `state + 0x58` are refreshed from camera/player objects.
- `FUN_1431D21E4` is an FOV equality predicate, not a blend owner.
- `FUN_1431D224D` is the state creation path, not a blend owner.
- The post-write notification uses an indirect virtual dispatch through the state subobject at `state + 0x28`, slot `+0x10`; its concrete runtime target is unresolved.
- Static evidence has not yet shown a consumer that reads `+0x54` and drives the visible transition.

## Approved scope

- Add or adapt one diagnostic-only tracer for the already-established state/update path.
- Correlate state-update/virtual-dispatch activity with user-visible markers: cutscene active/end, broken post-cutscene FOV, ADS correction and pause correction where reproducible.
- Log only the minimum safe context needed to distinguish the state object, `+0x54` value and concrete virtual target.
- Keep all diagnostics version-specific to executable 2.0.3 and read-only with respect to game state.

## Explicit non-goals

- Do not change stable gameplay code or normal experimental behavior; diagnostic-only changes to `src/cutscene_letterbox_fix.cpp` are allowed under `CUTSCENE_TRANSITION_TRACE` and must not affect the non-diagnostic build.
- Do not alter the Hor+ formula, A/B letterbox hooks or gameplay FOV behavior.
- Do not add a timer, delayed replay, synthetic FOV write or workaround.
- Do not hook arbitrary camera/renderer candidates or perform a global `+0x54` scan.
- Do not make runtime claims from tracer installation alone.

## Expected files and areas

- `02-Research/Ghidra/ghidra-scripts/` for a bounded helper only if static preparation is required.
- A diagnostic source/build script under the existing test-artifact workflow only after this plan is approved for implementation.
- Game-directory diagnostic log as disposable runtime output; do not add it to release assets.

## Batches

### Batch 1 — Diagnostic contract

- Prefer the already validated caller immediately before the indirect dispatch through `state + 0x28 / slot +0x10`; do not require a concrete virtual target to be known in advance.
- Define safe logging of the `state` pointer, the `state + 0x28` subobject pointer, the actual vtable function pointer at slot `+0x10`, fields `+0x50/+0x54/+0x58` and sequence/marker context.
- Treat object identity changes as first-class evidence: record whether the state or subobject is replaced across cinematic exit and natural return.
- Define install failure and rollback behavior before any runtime hook is installed.

Validation: static review of the diagnostic boundary against the 2.0.3 Ghidra evidence.

Batch 1 result:

- The diagnostic hook resolves a unique 2.0.3 state-writer entry pattern and decodes `MOVSS [RCX+0x54], XMM1` before installation.
- The callback records state identity, `state + 0x28` subobject identity, vtable slot `+0x10`, fields `+0x50/+0x54/+0x58`, input FOV and sequence context without modifying game state or registers.
- A second diagnostic observation point runs immediately after the validated `MOVSS [RCX+0x54], XMM1`, allowing pre-write and post-write state values to be compared without replaying or replacing the game update.
- The diagnostic ASI built successfully as `build-artifacts/test-asi/STALKER2ExperimentalLetterboxTransitionTrace.asi`.
- Batch 2 runtime testing is now unblocked; load/runtime behavior remains unvalidated.

### Batch 2 — Targeted runtime trace

- First priority is one continuous scenario: cinematic active, cinematic exit, visible overshoot and natural smooth return to gameplay FOV.
- Add ADS and pause only as control scenarios if the first cinematic trace does not distinguish the state path.
- Compare whether `+0x54` is read/updated after the cinematic write and whether the concrete target or state values differ at correction points.
- Preserve the distinction between correlation and causal proof.

Validation: bounded logs with event markers and no game-state mutation by the tracer.

Initial runtime result:

- The tracer captured a pre/post pair writing `126.869896` to `state + 0x54`; the pre-write value was `90` and the post-write value was `126.869896`.
- A later pre/post pair used a different `state` pointer and wrote `90.65574` to `state + 0x54`; this is temporally consistent with a later camera transition but is not conclusively tied to cinematic exit because no F7/F8 marker was recorded in this run.
- Both state objects exposed the same `state + 0x28` vtable pointer and the same executable slot `+0x10` target (`0x7FF695EF39B8` in this ASLR session), making it a concrete runtime virtual-owner candidate rather than an unknown target.
- The logged `+0x50` and `+0x58` values remained `0` and `-1` in both samples; this run does not establish that all three fields participate in one blend operation.
- The durable-write hypothesis has runtime support, but active read/propagation as a blend source remains unresolved. Repeat with F7/F8 markers if exact transition correlation is required.

Marker-correlated runtime result:

- `cinematic-fov` converted `90 → 126.869896` at `03:27:24`, followed immediately by a post-write `state + 0x54 = 126.869896`.
- `F7` was recorded at `03:27:26`; `F8` started the observation window at `03:28:00`.
- At `03:28:04`, approximately 4.36 seconds after `F8`, the same concrete virtual target was reached with a new state object and input FOV `90.65574`; the post-write value became `state + 0x54 = 90.65574`.
- The state pointer changed between the cinematic write and the post-`F8` write, while the vtable and slot `+0x10` target remained stable. This supports a natural state transition/update path and weakens the hypothesis that the overshoot is simply a stale value in one unchanged state object.
- The trace still observes writes and virtual-target identity, not a read of `+0x54` inside that target; blend-source/consumer semantics remain unresolved.

Follow-up preparation:

- The diagnostic log now records the executable module base, module-relative target RVA and whether the virtual target belongs to the game executable.
- The next run is intended to normalize the runtime target for Ghidra lookup; no new hook target will be selected from the absolute ASLR address alone.

Normalized-target static result:

- Runtime module base `0x7FF693E60000` and target `0x7FF695EF39B8` normalize to executable RVA `0x20939B8`, corresponding to Ghidra VA `0x1420939B8` in the 2.0.3 program.
- The exact target disassembly reads unrelated object/container offsets such as `+0x18`, `+0x1C`, `+0x28` and `+0x2C`; it does not read the FOV state field `+0x54`.
- Classify this concrete virtual target as `rejected` as the direct FOV/blend consumer. Its repeatability across state objects supports a shared notification/container path only; it does not establish that the post-write dispatch consumes the cinematic FOV.
- The durable state write remains confirmed, while the actual read/propagation path for `state + 0x54` remains unresolved.

Final ownership-bounded reader pass:

- The established state-dispatch function `FUN_1431D2094` reads `state + 0x54` only to compare the existing value with the incoming FOV before deciding whether to call the state writer. This is an equality/check reader, not a downstream blend or projection consumer.
- The established state writer `FUN_1431D23AE` writes `state + 0x54`, refreshes `+0x50/+0x58`, and dispatches the same notification path; its bounded caller/state-family inspection did not establish another method that reads `+0x54` as an active transition source.
- The runtime-derived virtual target at executable RVA `0x20939B8` was already rejected as a direct consumer because its observed instructions read unrelated container offsets and do not access `+0x54`.
- Result: durable state and state handoff remain confirmed, but persistence as an active blend source is not established. The state path is therefore not a safe implementation basis and this research branch stops without a workaround.

### Batch 3 — Decision gate

- If a concrete state consumer and transition relationship are established, update the implementation plan with a minimal safe intervention contract.
- If the state path is not active or remains non-causal, classify it as rejected/unresolved and stop without a workaround.

Diagnostic interpretation rule: a durable write to `state + 0x54` alone is not evidence that the field is an active blend source. If it is not subsequently read or propagated in the observed transition, reject the persistence hypothesis for this path.

Validation: compare the result with this plan; perform read-only Git review for changed research artifacts. Update `backlog/TASKLOG.md` only after the scoped research/manual validation is complete.

Batch 3 result:

- No validated native downstream consumer or blend relationship was established from the state family.
- The `state + 0x54` persistence/blend hypothesis is rejected as an implementation basis for this path; the remaining state-handoff correlation is unresolved rather than causal proof.
- No implementation, workaround, delayed replay, synthetic FOV write or additional broad scan is authorized by this task.

## Risks and rollback / safe failure

- The virtual target is indirect and may vary by runtime object; unresolved target identity must be logged as unresolved.
- The tracer must refuse installation if the validated observation boundary cannot be resolved uniquely.
- Any partial diagnostic installation must roll back before reporting failure.
- The tracer must not write `state + 0x54` or invoke update methods itself.

## Stop conditions and phase gates

- Stop if the trace requires broad renderer scanning or guessed function hooks.
- Stop if the state object cannot be identified safely at runtime.
- Stop if the trace shows only correlation without a usable downstream state relationship.
- Do not implement a fix until a concrete consumer/transition contract is established.

## Expected final Git review

After the scoped research/manual validation, inspect Git status, relevant diff and recent commits; compare actual changes with this plan and classify completed, remaining, deferred, blocked and not-runtime-validated items. Final `TASKLOG.md` entry remains deferred until the manual validation and plan comparison are complete.
