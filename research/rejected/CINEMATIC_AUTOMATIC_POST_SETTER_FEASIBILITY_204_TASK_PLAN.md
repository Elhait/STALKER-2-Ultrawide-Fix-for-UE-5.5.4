# Cinematic Automatic Post-Setter Feasibility 2.0.4 — Task Plan

## Objective

Test whether the manually validated combined cinematic state can be applied automatically at the exact current-build post-ENTER-setter boundary and cleaned up by native EXIT lifecycle plus one FOV restore.

## Established evidence and current state

- 2.0.4 ENTER setter is validated and writes native aspect `1.7777778` while preserving game-owned flags.
- Manual probe showed correct cinematic framing with `FOV=127.3927` and `aspect=3.5555556`; FOV→aspect and aspect→FOV were visually equivalent.
- The setter body has a validated post-write epilogue immediately after the aspect store.
- Native EXIT callback and gameplay handoff are validated; aspect/mode cleanup remains game-owned.
- The unsafe generic `CALL RAX + PAGE_GUARD/VEH` tracer is closed and must not be reused.

## Approved scope

- Create one automatic, read-only-diagnostic experimental ASI with only the two explicit camera writes required by this test.
- Hook the validated post-setter epilogue, after native aspect write and before return.
- Capture FOV baseline dynamically before the probe-owned FOV write.
- Write `+0x230=127.3927` and `+0x254=3.5555556` once per cinematic ENTER.
- Hook validated EXIT only to restore the saved `+0x230` FOV once; do not write `+0x254` or `+0x259` on EXIT.
- Log pre/post state and thread/object identity.

## Explicit non-goals

- No generic dispatch hook, PAGE_GUARD, VEH, single-step, timers, polling or hotkeys.
- No repeated writes or FOV enforcement during cinematic.
- No flags writes.
- No changes to the stable gameplay ASI or release artifacts.

## Expected files or areas

- New research source under `src/`.
- New build helper under `build-artifacts/test-scripts/`.
- New experimental `.asi` under `build-artifacts/test-asi/`.
- This task plan and scoped runtime evidence.

## Batches

### Batch 1 — Automatic post-setter artifact

- Validate the full ENTER signature and post-setter `C3` epilogue.
- Install post-setter and EXIT hooks only.
- Build and inspect the artifact.

Batch 1 result: created `src/cinematic_automatic_post_setter_feasibility_204.cpp` and `build-artifacts/test-scripts/build-cinematic-automatic-post-setter-feasibility-204.cmd`. The artifact validates the current ENTER signature, the `C3` epilogue immediately after the native aspect store, and the confirmed EXIT callback bytes. It performs one FOV/aspect pair after the native setter body, captures the dynamic FOV baseline before the pair, and restores only FOV at EXIT. It has no generic dispatch hook, PAGE_GUARD, VEH, timers, polling, hotkeys or flag writes. The build succeeded and produced `build-artifacts/test-asi/STALKER2CinematicAutomaticPostSetterFeasibility204.asi`. Runtime evidence is pending.

Validation correction: the current validated setter is the `B` variant, so the bytes after the aspect store are `B0 01 C3` rather than an immediate `C3`. The first artifact correctly failed closed on that mismatch; validation was corrected to require `B0 01 C3`, and the artifact rebuilt successfully. The hook remains at the first byte after the native aspect store, before the short return-value epilogue. No runtime evidence is promoted yet.

### Batch 2 — Isolated runtime feasibility

- Enable only this ASI; disable stable gameplay and all diagnostics.
- Run the established natural cinematic transition at ultrawide resolution.
- Verify post-setter pre/post state, visual framing and EXIT FOV-only cleanup.

Batch 2 runtime result: the artifact installed and captured `post-enter` on the same target with `beforeFov=90.65574`, `beforeAspect=1.7777778`, `beforeFlags=0x05`, then wrote `FOV=127.3927` and `aspect=3.5555556` successfully. The user reports that the cinematic remained at the wrong FOV despite rendering at 32:9. Therefore the immediate post-setter boundary is after the native aspect write but still too early for the effective cinematic FOV consumer; the direct field write is visible in state but does not control final framing at that point. The `exit-fov-restore` log's before-aspect/flags fields are placeholder arguments from the diagnostic logger and are not evidence of native EXIT state. Result: immediate post-setter FOV application is rejected, while a later deterministic automatic boundary remains open. Manual F3/F4 combined writes remain valid evidence that the same values can work at a later cinematic phase. No production integration is authorized.

### Batch 3 — Decision

- PASS only if automatic framing matches manual F3/F4, writes occur once, state survives without enforcement, native EXIT restores aspect/mode, and gameplay handoff is clean.
- Otherwise close this automatic model without modifying production code and classify the failing edge.

## Validation

- Batch 1: build success, unique ENTER signature, expected post-setter `C3`, validated EXIT bytes, and no unrelated hooks.
- Batch 2: runtime logs plus user visual observation; build success alone is not runtime proof.

## Risks and safe failure

- A wrong setter epilogue or object register must refuse installation.
- Writes are limited to the explicitly approved FOV/aspect fields and are restored only for the probe-owned FOV.
- If baseline or target identity is unreadable, no write occurs.

## Stop conditions and phase gates

- Stop after one isolated runtime test.
- Do not add repeated writes, dispatch hooks or fallback offsets after a negative result.
- Do not merge into the global/stable ASI before separate regression review.

## Final review

- Classify automatic ENTER and native EXIT behavior as confirmed, inconclusive or rejected.
- Compare changed paths with this plan and perform read-only Git review.
