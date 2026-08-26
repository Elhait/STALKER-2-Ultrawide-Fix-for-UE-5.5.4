# ADS/Pause Shared Camera/Viewmodel Refresh Research Task Plan

Status: Closed — superseded by the bounded ADS runtime state-diff task after the candidate function paths produced zero runtime hits.

## Objective

Identify the shared camera/viewmodel mode-transition refresh mechanism activated by weapon aim/ADS and pause resume, and determine whether its missing or incomplete execution explains the incorrect weapon FOV immediately after cutscene exit.

## Established evidence and current state

- The stable gameplay ASI reapplies gameplay aspect correction after cutscene exit.
- The player's weapon FOV can remain visually incorrect until the pause menu is opened and closed.
- The gameplay transition tracer recorded 7,944 complete pre/post pairs; `FUN_140AF481A` never changed `outputFov`.
- The first-person reflection-derived accessor tracer installed successfully but recorded no accessor calls in the tested scenario.
- The accessor family is therefore rejected as the direct runtime path for this symptom in that scenario, while the pause-triggered refresh owner remains unresolved.

## Accepted phase boundary

- Current runtime evidence shows that gameplay aspect correction completes after cutscene exit while the weapon/viewmodel FOV remains incorrect.
- This does not prove that forced replay could never help under different timing; it does establish that forced gameplay replay is not justified without separate evidence.
- `FUN_140AF481A` and the tested first-person reflection accessor family remain excluded from the active causal hypothesis unless new evidence points back to them.
- The only reproducible corrective trigger currently established is the pause-menu open/close sequence.
- The same corrective behavior is also observed when entering the weapon aiming/ADS mode; pause is therefore not the only trigger.
- ADS is the primary diagnostic anchor because it is semantically closer to weapon/viewmodel state than the pause UI.
- The next research owner is the shared camera/viewmodel mode-transition refresh or rebuild path used by ADS and, if confirmed, pause resume—not another gameplay aspect replay.

## Current ADS-first static result

- `EActionType::HandleAimInput` at `0x149328743` and `EObjBoolParams::IsAimingOut` at `0x149329FFC` are metadata anchors without executable code xrefs; they are not hook candidates.
- `+0x262` writers at `FUN_140B9990A` and `FUN_140F5AD8E` are initialization/constructor paths and are rejected as ADS transition owners.
- `FUN_1408F3578` is a broad skeletal/render processing path and is rejected as direct ADS evidence.
- `FUN_14070EBBA` writes the same mode field while constructing/updating a skeletal/render object and calls `FUN_14070F684`; no ADS or weapon/viewmodel ownership is established, so it is rejected as a direct ADS candidate.
- The remaining inspected consumers (`FUN_14087367E`, `FUN_140A5A2CC`) have no ADS-specific structural evidence and remain unresolved rather than promoted.
- Result: this bounded ADS-first static pass found no candidate meeting the strong-candidate gate. Batch 2 runtime correlation remains gated.
- Status: **Batch 1 complete — no strong ADS static owner found. Batch 2 blocked pending a concrete runtime-visible weapon-state anchor.**
- The gate is now satisfied by the concrete `IA_OffsetAiming → FUN_1469135BE → FUN_14695C348` path: `FUN_14695C348` receives the requested `object+0x248` state and conditionally calls virtual slot `+0xA98`.
- Batch 2 tracer `STALKER2AdsViewmodelRefreshTrace.asi` was built successfully; build success is not runtime evidence.
- Manual runtime session produced only `TRACE hook installed: FUN_14695C348 / IA_OffsetAiming refresh path.` and zero `TRACE seq=...` records.
- This is a negative correlation for the tested session, not proof that every ADS refresh mechanism is absent; the upstream `FUN_1469135BE` handler or another observable ADS event may still be required.

## Approved scope

- Research-only static and targeted runtime analysis of ADS/pause-triggered refresh/rebuild behavior.
- Compare execution paths and state changes between normal gameplay, immediately after cutscene exit, after pause open/close, and on weapon aim/ADS entry.
- Investigate three candidate classes: camera/view-state invalidation or rebuild; projection/view-family recreation; weapon/viewmodel component refresh or render-state dirtying.
- Reuse existing read-only tracing infrastructure only where it does not alter stable gameplay behavior.

## Explicit non-goals

- Do not reinstall or replay the gameplay aspect hook.
- Do not modify `src/gameplay_aspect_fix.cpp`.
- Do not enable or modify the experimental letterbox ASI.
- Do not add delayed replay, hard-coded FOV multipliers or arbitrary memory writes.
- Do not claim a causal owner from static names, reflection metadata or generic renderer matches alone.
- Do not finalize `TASKLOG.md` until the scoped research and manual validation requirements are complete.
- Do not promote an ADS candidate to a strong causal candidate from ADS evidence alone; require evidence in at least two of the three scenarios, with ADS required as one of them.
- If ADS and pause use different refresh mechanisms, investigate their shared downstream projection invalidation or rebuild effect instead of forcing a shared-owner conclusion.

## Expected files and areas

- `02-Research/Ghidra/ghidra-scripts` for reusable read-only scripts.
- A separate diagnostic tracer source/build script only after a concrete ADS/pause refresh candidate is identified.
- This plan file for bounded findings and phase status.
- No stable gameplay source, letterbox source or release assets.

## Batches and validation

### Batch 1 — ADS-first static discovery

- Start from weapon aim/ADS transition entrypoints and identify the concrete camera/viewmodel refresh, invalidation or mode-transition owner.
- If direct ADS entrypoints are reflection/configuration-only or unresolved, pivot to concrete runtime-visible weapon-state transitions rather than broad renderer scans.
- Use pause-menu/resume entrypoints as a secondary path and check whether they call the same owner or an equivalent shared refresh routine.
- Identify candidate invalidation, view-family rebuild and weapon/viewmodel refresh calls.
- Trace candidate callers toward camera/projection or first-person component state.
- Classify candidates as `confirmed`, `likely`, `unresolved` or `rejected` with a reason.
- Validation: every retained candidate must have reproducible executable addresses, disassembly and evidence distinct from `FUN_140AF481A` and the rejected first-person accessor family.
- Current status: completed; the original semantic-marker search was negative, then a concrete ADS action-handler path was established.

### Batch 2 — Targeted runtime correlation

- Instrument only the smallest candidate set from Batch 1.
- Record bounded sequence numbers, caller addresses, object/context pointers and relevant state transitions across normal gameplay, ADS entry/exit, pause resume and post-cutscene exit.
- Validation: tracer must be read-only, rollback-safe and separate from stable ASI; no partial state mutation is permitted.
- Gate: do not create a runtime tracer until Batch 1 identifies at least one candidate with weapon/viewmodel-specific structural evidence and ADS as an observed or traceable transition anchor.
- If a bounded runtime-visible transition search does not produce a weapon-specific owner, do not return to a broad renderer scan. Stop the owner search and design only a minimal discovery tracer around a concrete observable ADS event.
- Current status: tracer built and ready for manual testing across normal gameplay, post-cutscene exit, ADS entry/exit and pause resume.
- Runtime result: no calls reached `FUN_14695C348` in the tested session; direct owner correlation is unresolved/negative for this candidate.
- `FUN_14695C348` is now rejected as the active ADS refresh owner for the tested runtime scenario.
- A minimal upstream tracer for `FUN_1469135BE` was built successfully as `STALKER2AdsOffsetAimingTrace.asi`; build success is not runtime evidence.
- Manual runtime session for `FUN_1469135BE` produced only `TRACE hook installed: FUN_1469135BE / IA_OffsetAiming handler.` and zero `TRACE seq=...` records.
- The full `IA_OffsetAiming → FUN_14695C348` branch is therefore rejected as the active player ADS path for the tested executable/scenario.
- Status: `IA_OffsetAiming → FUN_1469135BE → FUN_14695C348` rejected as the active player ADS refresh path by runtime evidence.
- Do not trace further upstream on this branch. The next tracer requires a new concrete observable ADS anchor (weapon pose/offset, camera transform, projection state, animation state or mesh state); semantic names alone are insufficient.
- If no such anchor is available, record the static/runtime ownership blocker rather than returning to broad renderer scanning.
- New bounded anchor: `FUN_140A5A2CC` consumes `+0x248`, `+0x262` and `+0x263` and forwards the captured render state to `FUN_140A5A3D8`; it is an observable state-consumer candidate, not yet an ADS owner.
- `STALKER2AimRenderStateTrace.asi` was built successfully for manual ADS/cutscene/pause correlation; build success is not runtime evidence.
- Manual runtime session produced only `TRACE hook installed: FUN_140A5A2CC aim/render state consumer.` and zero trace records.
- `FUN_140A5A2CC` is rejected as an observable ADS path for the tested executable/scenario; do not continue upstream or downstream on this branch without new evidence.
- Function-level static candidate chasing is paused after repeated zero-hit runtime results. The next task was `backlog/complete/ADS_STATE_DIFF_TASK_PLAN.md`: runtime state-diff/discovery before any new function-level hook.

### Batch 3 — Causal assessment

- Compare broken post-cutscene state with corrected post-ADS state and corrected post-pause state.
- Compare the same candidate across ADS, pause resume and cutscene exit where observable; treat both corrected states as control points.
- Treat a candidate as strong only when it has evidence in at least two scenarios, one of which is ADS, and its call/state/target changes correlate with the correction.
- If no common owner exists, assess whether separate ADS/pause paths converge on a common downstream projection invalidation or rebuild effect.
- If no candidate changes across the transition, reject it as the direct cause and report the blocker.

## Risks and safe failure

- Pause UI may dispatch indirectly through generic engine systems; do not equate proximity to a pause string with ownership.
- A candidate may affect global rendering rather than weapon/viewmodel projection; retain the distinction.
- Tracer installation must be all-or-nothing or roll back already-installed hooks.
- On ambiguous resolution, decode failure, invalid context or unsafe dereference, refuse safely and produce a diagnostic record only.

## Stop conditions and phase gates

- Stop on contradictory executable identity or evidence that the pause path is not represented in the analysed build.
- Do not implement a corrective hook until a candidate correlates with the broken-to-corrected transition.
- Do not return to the rejected accessor family without new evidence that those functions execute in the affected scenario.
- Perform a read-only post-change Git review after any tracked research-artifact change.
- Write the final task-closure `TASKLOG.md` entry only after the scoped research and manual validation are complete.
