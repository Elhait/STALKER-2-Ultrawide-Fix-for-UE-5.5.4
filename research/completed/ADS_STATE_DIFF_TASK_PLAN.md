# ADS Runtime State-Diff Discovery Task Plan

Status: Closed — marker-correlated runtime evidence rejected the gameplay-camera state branch as the causal owner.

## Objective

Identify a concrete runtime memory/state change that is reproducibly associated with ADS enter/exit and compare it with the broken post-cutscene and corrected post-pause states. Use that observable state as the anchor for any later ownership analysis.

## Established evidence and current state

- The gameplay aspect correction completes after cutscene exit while weapon/viewmodel FOV can remain visually incorrect.
- Pause resume and ADS have both been observed to correct the visual symptom.
- `FUN_140AF481A` did not change `outputFov` in 7,944 pre/post runtime pairs.
- The reflection-derived first-person accessor family recorded no calls in the tested scenario.
- `IA_OffsetAiming → FUN_1469135BE → FUN_14695C348` recorded zero calls during ADS testing and is rejected as the active path for that runtime context.
- `FUN_140A5A2CC` also recorded zero calls during the tested ADS scenario and is rejected as an observable active ADS path.
- Repeated function-level static candidate tracing has low yield; the next anchor must come from a runtime state difference, not another guessed function.

## Approved scope

- Research-only runtime discovery of state differences caused by ADS enter/exit.
- Compare a bounded set of observable camera, weapon/viewmodel, projection and transform state before and after ADS, then against post-cutscene and post-pause states.
- Use read-only sampling or a narrowly scoped diagnostic mechanism only after its memory scope and lifetime are established.
- Record executable identity, test sequence and exact state addresses/fields for every retained observation.

## Explicit non-goals

- Do not add another function-level hook based only on a static candidate address.
- Do not modify stable gameplay or experimental letterbox source.
- Do not replay the gameplay aspect transition, add delayed updates or use a hard-coded FOV multiplier.
- Do not write to game memory, invoke game functions or alter camera/viewmodel state.
- Do not claim ownership from a state diff alone; a changed value is an anchor, not proof of its writer or causal role.
- Do not return to broad renderer scanning if this discovery pass fails.

## Expected files and areas

- This plan file.
- A separate diagnostic sampler/tracer source and build script only if a concrete, bounded sampling method is defined.
- Runtime log in the game's Win64 directory, treated as external test output.
- No stable source, letterbox source or release assets.

## Batches and validation

### Batch 1 — Define observable state scope

- Use the already validated gameplay camera-writer context as the initial observable object; do not scan arbitrary process memory.
- Sample exactly these 12 values from that context: source `+0x230` primary FOV, `+0x234` secondary FOV, `+0x248`, `+0x254` aspect, `+0x259` flags, `+0x25C`, `+0x260`, `+0x261`, `+0x262`, `+0x263`, plus output `+0x30` FOV and `+0x5C` aspect.
- Treat `+0x248/+0x262/+0x263` as candidate mode/state bytes only; they are not assumed to be weapon-owned.
- Define sampling boundaries and a reproducible event sequence: normal gameplay → ADS enter → ADS exit → cutscene exit → pause resume.
- Read only when the existing validated gameplay writer supplies a live source/output context; reject invalid or unavailable reads.
- Validation: exact fields, object lifetime, executable identity and safe read boundaries are now defined before runtime collection.
- Status: Batch 1 complete. The sampler reuses the already validated gameplay-writer resolver/hook infrastructure and is limited to the 12 fields above.

### Batch 2 — Runtime state-diff collection

- Collect bounded samples before and after each transition, with sequence/event markers and no writes or game-function calls.
- Capture only changed values and enough context to distinguish camera objects from unrelated renderer state.
- Add only five manual event markers to the diagnostic tracer: F8 `cutscene-exit`, F9 `ads-enter`, F10 `ads-exit`, F11 `pause-open`, F12 `pause-close`. Each marker records the current trace sequence and does not alter game state.
- Validation: sampler is read-only, bounded, rollback-safe if hooked, and produces explicit no-data/ambiguous results.
- Status: Batch 2 complete. The marker-enabled sampler built as `STALKER2GameplayTransitionTrace.asi`, loaded successfully and recorded all five manual markers in the Win64 runtime log.

### Batch 3 — Anchor assessment

- Compare ADS-corrected, pause-corrected and broken post-cutscene states.
- Retain a state as an anchor only if it changes reproducibly on ADS and is informative for at least one second scenario, or if it is a clearly measurable downstream projection effect shared by the corrected paths.
- If no such anchor is found, stop and report the runtime discovery blocker; do not create another guessed function-level tracer.
- Status: Batch 3 complete with a negative result. Marker-correlated runtime evidence found no differing state pattern in the 12 sampled gameplay-camera values between cutscene exit, ADS enter/exit and pause open/close. This gameplay-camera branch is rejected as the causal owner for the weapon/viewmodel FOV symptom.

## Risks and safe failure

- Broad memory sampling can produce unrelated changes; keep the state scope small and classify unrelated changes as rejected.
- Object pointers may become invalid across transitions; every read must be guarded and lifetime failures must produce a safe no-data result.
- Timing around ADS/cutscene transitions may be ambiguous; record event order and do not infer causality from proximity.
- Any diagnostic installation failure must leave no partial hooks or state changes.

## Stop conditions and phase gates

- No runtime sampler is built until Batch 1 defines a bounded observable state scope.
- No new function-level hook is permitted without a new runtime state anchor.
- If ADS does not produce a reproducible state difference, stop this task and report the blocker.
- If ADS changes state but corrected pause/cutscene comparisons are inconclusive, retain it as an unresolved anchor and defer ownership tracing.
- Current phase gate: start the next task from a different weapon/viewmodel object or downstream projection context. Do not return to `[RBX+0x30]`, `+0x248`, `+0x25C` or `+0x260..263` without contradictory evidence.
- Final read-only Git review and factual `TASKLOG.md` entry occur after scoped manual testing and plan comparison; no runtime success claim follows from a build alone.
