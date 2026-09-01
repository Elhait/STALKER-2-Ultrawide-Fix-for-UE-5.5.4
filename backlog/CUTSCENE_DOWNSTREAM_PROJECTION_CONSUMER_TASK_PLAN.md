# Cutscene Downstream Projection Consumer Research Task Plan

Status: Batch 1 complete — bounded static pass did not establish a downstream projection-only consumer; Batch 2 remains blocked.

## Objective

Find the last cinematic projection consumer before render/view projection formation that receives the game-owned cinematic FOV and aspect, so a future Hor+ conversion can be applied transiently without changing durable camera-state FOV.

## Established evidence and current state

- Target executable: S.T.A.L.K.E.R. 2 version 2.0.3, analyzed in `Dump/STALKER2-Ghidra`.
- A/B letterbox hooks are runtime-confirmed to switch cinematic rendering to native `5120×1440` / `32:9`.
- Dynamic Hor+ conversion is visually correct during the cinematic, but transforming `90` to `126.869896` before the existing camera-state machinery causes a temporary post-cutscene FOV overshoot.
- The transformed value reaches durable `state + 0x54`; this state field and its handoff are confirmed, but no active downstream blend/projection consumer was established.
- `FUN_1431D2094` reads `state + 0x54` only for equality/update checking, and the runtime-derived virtual target at executable RVA `0x20939B8` does not read `+0x54`.
- The current working hypothesis is that Hor+ should affect projection interpretation transiently while the game-owned cinematic FOV remains unchanged.

## Approved scope

- Perform bounded, read-only Ghidra/source analysis starting from the established cinematic FOV call chain and its downstream consumers.
- Identify candidate functions that consume FOV together with aspect, projection scale, camera transform or matrix parameters near the render/view-projection boundary. A candidate may expose this indirectly through `tan(FOV/2)`, tan-half-FOV, reciprocal focal scale or another derived projection value rather than a literal FOV-plus-aspect pair.
- Prefer a concrete projection-only consumer that does not write transformed FOV back into durable camera state.
- Record whether the candidate operates on temporary view/projection data or on a persistent camera component/state object.
- Record executable-version-specific addresses, decoded instructions, caller/object context and confidence classification for each candidate.
- Use only the existing 2.0.3 Ghidra project and reusable research-script location.

## Explicit non-goals

- Do not reopen `state + 0x54` as a blend-owner hypothesis without new contradictory evidence.
- Do not modify A/B letterbox hooks or the stable gameplay ASI.
- Do not write aspect into an arbitrary camera object.
- Do not add a refresh call, delayed replay, timer, synthetic FOV write or hard-coded FOV multiplier.
- Do not create a new runtime tracer or implementation hook before a concrete downstream projection anchor is established.
- Do not claim runtime behavior, compatibility or final ownership from static evidence alone.

## Expected files and areas

- `02-Research/Ghidra/ghidra-scripts/` for one or more bounded read-only helpers if required.
- `Dump/STALKER2-Ghidra` as the preserved analysis project.
- This task plan for factual batch results.
- No stable source, ASI output or release asset changes are expected.

## Batches

### Batch 1 — Downstream call-chain mapping

- Start from the known cinematic FOV flow through `FUN_146B68976`, `FUN_1431D2094` and the established state update path.
- Follow only concrete downstream callers/consumers toward camera/view/projection formation.
- Record where FOV is copied, converted, combined with aspect or passed into matrix/projection setup.
- Include paths where FOV is first converted into tan-half-FOV, projection scale or an equivalent derived value and only later combined with aspect-dependent X/Y scale.
- Separate durable state updates, camera-state notifications and actual projection consumers.

Validation: read-only Ghidra output tied to executable 2.0.3 and exact decoded instructions.

Batch 1 result:

- `FUN_146B68976` resolves to a small state-dispatch wrapper: it obtains the camera-related object, preserves the incoming value as the second argument and calls `FUN_1431D2094`.
- `FUN_1431D2094` performs state selection/creation, compares the incoming value with `state + 0x54` and forwards updates to `FUN_1431D23AE`; this remains camera-state machinery rather than a projection-only consumer.
- The bounded caller review confirms the direct path into `FUN_1431D23AE`, whose known behavior is the durable `+0x54` write, related state refresh and indirect notification. No direct downstream view/projection formation path was established from this chain.
- No candidate using tan-half-FOV, projection scale, aspect-dependent X/Y scale or temporary view/projection data was established in this bounded pass.
- Result: the current chain does not provide a validated projection-only intervention point. Batch 2 is blocked pending a new concrete downstream anchor; no broad matrix/FOV scan is authorized.

### Batch 2 — Candidate classification

Classify candidates as `confirmed`, `likely`, `unresolved` or `rejected` using:

- evidence that the candidate is on the cinematic render path;
- evidence that it consumes FOV and relevant aspect/projection state;
- evidence that relevant values flow through a derived projection quantity such as tan-half-FOV or projection scale;
- evidence that it is projection-only or otherwise avoids durable FOV mutation;
- evidence that it operates on temporary view/projection data rather than mutating persistent camera state;
- caller/object context and continuation safety.

A function shared by gameplay and cinematic paths is not automatically rejected. Accept it only if the cinematic path reaches it with a distinguishable context/state and a transient modification can be restricted to the cinematic case. Do not promote a generic renderer or matrix helper without cinematic-path or shared-final-projection evidence.

Validation: compare each candidate with the ownership criteria and document rejected candidates with reasons.

### Batch 3 — Architecture gate

- If a concrete projection-only consumer is established, prepare a separate implementation plan for transient live-FOV Hor+ conversion.
- If no projection-only consumer is established, evaluate camera/aspect refresh ownership only as a separate future research task.
- If neither path can be supported by bounded evidence, stop and report the blocker without a workaround.

Validation: plan comparison and read-only Git review for changed research artifacts. Update `backlog/TASKLOG.md` only after the scoped research result and any required manual validation are complete.

## Risks and rollback / safe failure

- Generic renderer and matrix functions may be shared by gameplay, UI and cinematic paths; shared code alone is insufficient ownership evidence.
- A static candidate may receive a transient FOV value without being the final projection owner; keep this distinction explicit.
- Research scripts must be read-only and must not modify the Ghidra project or executable state.
- If a candidate's calling convention, object lifetime or continuation cannot be established, classify it as unresolved and do not hook it.

## Stop conditions and phase gates

- Stop if the analysis becomes a broad FOV/projection scan without a concrete downstream path.
- Stop if only generic renderer candidates are found without cinematic ownership evidence.
- Stop if the only available intervention changes durable FOV state rather than projection interpretation.
- Do not implement or build an ASI from this task plan alone.

## Expected final Git review

After the bounded research result, inspect Git status, relevant diff and recent commits; compare actual changed paths with this plan and classify completed, remaining, deferred, blocked and not-runtime-validated items. Final `TASKLOG.md` entry remains deferred until the scoped research/manual validation is complete.
