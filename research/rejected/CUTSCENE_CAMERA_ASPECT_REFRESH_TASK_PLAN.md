# Cutscene Camera Aspect Refresh Research Task Plan

Status: Blocked/Closed — Batch 1 completed with no validated refresh owner; Batch 2 not opened.

## Objective

Find a game-owned mechanism that reapplies native aspect/projection semantics during a camera transition or rebuild without changing the durable game-owned cinematic FOV state.

## Established evidence and current state

- Target executable: S.T.A.L.K.E.R. 2 version 2.0.3, analyzed in `Dump/STALKER2-Ghidra`.
- A/B letterbox hooks are runtime-confirmed to switch cinematic rendering to native `5120×1440` / `32:9`.
- Dynamic Hor+ conversion produces correct cinematic framing, but applying it before the established FOV state machinery temporarily contaminates the post-cutscene camera transition.
- The known cinematic FOV chain ends in camera-state machinery: `FUN_146B68976 → FUN_1431D2094 → FUN_1431D23AE → durable state`.
- The `state + 0x54` field is a confirmed durable write target, but its active blend/projection role was not established. The known equality reader and runtime-derived virtual target are not usable refresh owners.
- A bounded downstream search did not establish a projection-only consumer, tan-half-FOV path, projection scale, temporary view/projection structure or aspect-dependent X/Y projection stage.
- The remaining hypothesis is that a native camera/aspect refresh or rebuild mechanism can restore projection semantics without writing transformed FOV into durable camera state.

## Approved scope

- Perform bounded, read-only Ghidra/source analysis of known camera transition, view rebuild, aspect invalidation and projection-refresh mechanisms.
- Start from already established camera/cinematic context and known user-observable transitions, including cutscene exit and other native camera-mode transitions only when they provide concrete evidence.
- Use ADS transition and pause resume only as native camera-transition anchors for intersecting call paths, not as a return to the deferred weapon-FOV ownership search.
- Identify methods or state transitions that consume the native aspect and rebuild view/projection state without synthetic FOV mutation.
- Record executable-version-specific addresses, decoded instructions, object/context evidence and confidence classification.
- Use only the existing 2.0.3 Ghidra project and reusable research-script location.

## Explicit non-goals

- Do not reopen `state + 0x54` as a blend owner without new contradictory evidence.
- Do not repeat the known downstream projection search or perform a broad matrix/FOV scan.
- Do not modify A/B letterbox hooks or the stable gameplay ASI.
- Do not write aspect into an arbitrary camera object.
- Do not add timers, delayed replay, synthetic FOV writes, fixed multipliers or guessed hooks.
- Do not create an implementation hook or diagnostic tracer before a concrete game-owned refresh anchor is established.
- Do not claim that a candidate fixes the issue from static evidence alone.

## Expected files and areas

- `02-Research/Ghidra/ghidra-scripts/` for bounded read-only helpers only if required.
- `Dump/STALKER2-Ghidra` as the preserved analysis project.
- This task plan for factual batch results.
- No stable source, ASI output or release asset changes are expected.

## Batches

### Batch 1 — Refresh-context discovery

- Identify concrete camera/view state objects and transitions already associated with cutscene exit, camera-mode changes or projection rebuilds.
- Prefer methods with explicit invalidation, rebuild, recalculate, aspect, viewport or view-family behavior over generic renderer helpers.
- Establish object ownership, calling convention and the state fields consumed or updated.
- Confirm that the candidate does not require changing durable cinematic FOV.
- Prioritize candidates that read or reapply the native `3.5556` aspect while leaving cinematic/global FOV unchanged.
- Compare the bounded call paths for cutscene exit, ADS transition and pause resume when concrete cross-path evidence is available.

Validation: read-only Ghidra evidence tied to executable 2.0.3 and exact decoded instructions.

Batch 1 result:

- The `OnSequencePaused` anchor resolves to a reflection/name-registration helper, not a runtime camera transition or refresh owner.
- The pause-menu dispatcher resolves to UI action/event registration and dispatch. Its concrete instructions do not establish native aspect, view-family, projection rebuild or camera-state refresh semantics.
- The inspected concrete ADS action handler resolves to input/action state preparation and dispatch; it does not establish a camera/aspect refresh or projection rebuild path.
- No shared camera/view refresh function intersecting the inspected pause and ADS anchors with the cinematic path was established.
- Result: this bounded anchor set is `rejected` as a refresh-owner source. Batch 2 is blocked; no runtime tracer or implementation hook is justified from these candidates.

### Batch 2 — Candidate classification

Classify candidates as `confirmed`, `likely`, `unresolved` or `rejected` using:

- concrete camera/view ownership;
- evidence of native aspect/projection refresh semantics;
- evidence that the operation is game-owned and not a synthetic FOV workaround;
- evidence that the candidate can be restricted to the cinematic transition without changing stable gameplay behavior;
- calling convention, object lifetime and continuation safety.

Do not promote a generic renderer, UI refresh or unrelated camera-mode helper without transition/context evidence.

Validation: document each rejected candidate with the specific reason and preserve unresolved edges without guessing.

### Batch 3 — Architecture gate

- If a game-owned refresh mechanism is established, prepare a separate implementation plan for an isolated experimental integration.
- If only generic or unresolved refresh candidates are found, stop without a tracer or workaround.
- If no safe refresh mechanism can be established, record the blocker and leave the current experimental behavior unchanged.

Validation: compare the result with this plan and perform read-only Git review for changed research artifacts. Update `backlog/TASKLOG.md` only after the scoped research result and any required manual validation are complete.

## Risks and rollback / safe failure

- Camera refresh methods may be shared by gameplay, pause, ADS and cinematic transitions; shared naming or call proximity is insufficient ownership evidence.
- A refresh may implicitly reset FOV or other camera state even if it does not expose an obvious FOV argument; inspect side effects before considering it safe.
- Research scripts must be read-only and must not modify the Ghidra project or executable state.
- If ownership, lifetime or continuation cannot be established, classify the candidate as unresolved and do not hook it.

## Stop conditions and phase gates

- Stop if the search becomes a broad renderer/matrix/FOV scan.
- Stop if the only candidates mutate durable FOV or require timing-based replay.
- Stop if no candidate has concrete camera/view ownership and native aspect/projection refresh evidence.
- Do not implement or build an ASI from this task plan alone.

## Expected final Git review

After the bounded research result, inspect Git status, relevant diff and recent commits; compare actual changed paths with this plan and classify completed, remaining, deferred, blocked and not-runtime-validated items. Final `TASKLOG.md` entry remains deferred until the scoped research/manual validation is complete.
