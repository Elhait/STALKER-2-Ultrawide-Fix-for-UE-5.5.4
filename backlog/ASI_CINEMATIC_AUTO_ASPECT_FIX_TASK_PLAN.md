# Task Plan — Cinematic Auto Aspect Resolution

## Objective

Fix `Cinematics=Auto` so it resolves the physical game viewport aspect rather
than reusing the game's current cinematic camera aspect.

## Established evidence and current state

- User runtime log on a 5120x1440 display recorded `Cinematics=Auto` resolving
  `2.38889` (approximately 21:9), producing FOV `106.688` and visible side
  bars.
- The current source resolves Auto from `g_lastObservedAspect`, which is read
  from the game's camera state and can remain 21:9 during cinematic entry.
- The desired Auto source is the actual game client viewport aspect, with a
  display-mode fallback when the client rectangle is unavailable.

## Approved scope

- Change only Auto aspect-source resolution in
  `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- Preserve explicit `Native`, `16:9`, `21:9` and `32:9` policies.
- Preserve FOV math, transition latch, gameplay replay, dialogue and guarded
  signature validation.
- Build a new ASI for user runtime testing.

## Explicit non-goals

- No new cinematic FOV formula.
- No changes to gameplay or dialogue behavior.
- No change to the fixed aspect policies.
- No installation into the game directory.
- No release archive replacement until runtime validation passes.
- No commit, tag or publication.

## Files or areas expected to be touched

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `backlog/TASKLOG.md`
- `research/reports/ASI_CINEMATIC_AUTO_ASPECT_FIX.md`

## Batches

### Batch 1 — Source correction

Add bounded client-viewport/display fallback resolution and make only Auto use
it. Keep invalid/unavailable measurements fail-safe at native 16:9.

### Batch 2 — Build and static review

Build the unified ASI, run diff/syntax checks and verify explicit policies and
existing resolver code are unchanged.

### Batch 3 — User runtime validation

User tests Auto on the actual 32:9 viewport and supplies the module log plus
visual result. Only then decide whether to refresh release assets.

## Validation

- Source review confirms Auto no longer selects `g_lastObservedAspect`.
- Build succeeds and the expected ASI is produced.
- Log includes the resolved Auto aspect source/value.
- Runtime target on 5120x1440 is approximately `3.555556`, with cinematic FOV
  approximately `126.869896` for authored FOV 90.
- No claim is made until the user tests the rebuilt binary in-game.

## Risks and rollback / safe failure

- Multi-monitor or unusual window layouts may make display mode differ from
  the game client; client viewport is preferred and display mode is fallback.
- If both measurements are unavailable or invalid, use native 16:9 rather than
  an unsafe guessed aspect.
- Rollback is the source diff reversal; no existing release files are deleted.

## Stop conditions and phase gates

- Stop if the client/display measurement cannot be made safely.
- Stop release packaging if Auto still resolves to the camera's cinematic
  aspect or if the runtime result is not tested.
- Keep the prior release archive untouched until runtime PASS.

## Expected final Git review

Inspect status, relevant diff/statistics, changed paths and recent commits.
Report completed, remaining, deferred, blocked and not-runtime-validated
items. Do not commit or publish.
