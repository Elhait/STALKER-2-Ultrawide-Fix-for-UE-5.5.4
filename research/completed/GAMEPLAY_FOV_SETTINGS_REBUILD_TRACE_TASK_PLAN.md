# Gameplay FOV Settings-Rebuild Trace Task Plan

## Objective

Determine why a gameplay FOV changed from approximately 120 to 90 in the
settings UI is not observed on the monitored gameplay camera path, and whether
the unified gameplay replay restores a stale FOV during camera rebuild.

## Established evidence and current state

- The current renamed ASI loaded on Steam 2.0.4 with runtime identity
  `08C07F6398DF9D7BF562EAD97852EA6E3E145EBCB8280430F2073C04027F7E50`.
- The dialogue Candidate correction worked in the supplied trace: the first
  sample was provisional, and activation required confirmed descent.
- After cinematic recovery and a later settings change, the monitored camera
  path continued to report approximately `120.344` rather than approximately
  `90`.
- The log does not yet establish whether the game failed to apply 90, a camera
  rebuild restored 120, or the gameplay replay path reintroduced an old value.

## Approved scope

- Read-only inspection of the existing gameplay camera/replay path.
- A bounded differential runtime trace of settings FOV application without
  dialogue or cinematic activity.
- Compare settings/application observations with camera source, replay input,
  and monitored FOV output.
- Identify the narrowest next owner if the trace establishes one.

## Explicit non-goals

- No dialogue classifier changes.
- No dialogue optical math or lifecycle changes.
- No new dialogue/cinematic hook.
- No guessed writes to camera fields or FOV values.
- No rename, packaging, release or INI migration changes.
- No production fix until the loss/restoration point for FOV 90 is evidenced.

## Expected files or areas

- Existing unified source only for read-only review unless a minimal diagnostic
  trace is separately approved after the static review.
- A new runtime log supplied by the user.
- This plan and a factual result record if the bounded theory is resolved.

## Batches

### Batch 1 — Static owner review

- Inspect settings/config loading and runtime FOV policy paths.
- Inspect `ReplayManualTransitionOriginal` inputs and all FOV writes or replay
  calls around camera rebuild.
- Determine whether the current code caches a FOV value or only forwards the
  game's incoming `context.xmm0`.
- Do not edit source.

#### Batch 1 result

- The clean gameplay trace established that the monitored camera path can
  observe a live transition from approximately `120` to `90.6557` without a
  cinematic, dialogue or save-load event.
- The current camera writer receives the game's incoming FOV through
  `context.xmm0`; the replay path changes aspect/flags and does not assign a
  replacement FOV value.
- The existing `Camera mode` log is edge-triggered by mode/aspect/flags and is
  insufficient to reconstruct every Settings slider step.
- A bounded read-only diagnostic of incoming `primaryFOV` is therefore
  justified. It will be compiled only into the test ASI.

#### Batch 1 finding from the diagnostic trace

- The complex reproduction contains a material gameplay FOV jump on the same
  camera source: approximately `120.656 → 90.6557`, with no cinematic or
  coordinator transition at that moment.
- A provisional dialogue candidate was still alive when this jump occurred;
  the next dialogue then committed approximately `120.656` as its baseline.
- The narrow invalidation observable is therefore an existing gameplay camera
  writer context change: a camera-source replacement, or a discontinuous FOV
  jump greater than the bounded `5°` threshold. Smooth native dialogue samples
  remain below this discontinuity threshold and are not invalidated.

### Batch 2 — Differential runtime trace

- Start gameplay at FOV approximately 120.
- Remain in gameplay; do not enter dialogue or cinematic.
- Apply FOV 90 in the game settings and return to gameplay.
- Capture camera/replay events before and after Apply.
- Compare the first authoritative post-Apply FOV with the value before Apply.

#### Batch 2 preparation

- Add test-only logging for each meaningful `primaryFOV` change, including
  source pointer and coordinator/replay state.
- Do not alter `context.xmm0`, camera fields, dialogue state or replay logic.

#### Batch 2 preparation update

- The diagnostic trace identified a concrete invalidation owner. The source now
  resets non-inactive dialogue transient state at that bounded camera-context
  change and leaves the current sample native.
- The test-only FOV trace remains enabled for the next regression run.

#### Batch 2 result

- The supplied diagnostic runtime trace confirmed the problematic ordering on
  one stable camera source: a dialogue candidate/baseline near `120.607`, then
  a live FOV jump to `90.6557`, followed by a new dialogue.
- The new invalidation guard logged the context change and reset transient
  dialogue state before the next lifecycle.
- The next dialogue captured `baselineG=90.6364` and recovered to the 90-FOV
  gameplay baseline; the stale approximately-120 baseline was not reused.
- The reverse transition `90.6557 → 120.951` was also invalidated, and the
  following dialogue captured approximately `120.906`, confirming the guard
  works in both directions.
- The user confirmed the visual result was correct. No cinematic or unrelated
  lifecycle was required for this bounded FOV-context regression.

#### Batch 2 gate

- `120 → 90` stale-candidate reproduction: PASS.
- `90 → 120` reverse invalidation control: PASS.
- Same camera source in both transitions: PASS; source replacement is not
  required for invalidation.
- Dialogue recovery returns to the fresh baseline in both directions: PASS.
- Root cause is closed as stale provisional dialogue baseline surviving a
  material gameplay FOV context change.
- The diagnostic build is not release-approved; production rebuild without
  `FOV_SETTINGS_TRACE_DIAGNOSTIC` is required next.

### Batch 3 — Production-candidate regression

#### Batch 3 result

- Rebuilt without `FOV_SETTINGS_TRACE_DIAGNOSTIC` using the same invalidation
  logic.
- Production-style candidate SHA-256:
  `02B9267C4414BEE73351642584A42C3C649708D9E1A330E5D484A88D0B564FD0`.
- The supplied runtime log confirms cinematic ENTER/EXIT and native recovery
  remained intact before the dialogue checks.
- The `120 → 90` and `90 → 120` invalidation/re-baseline behavior remained
  visually correct in the production-style candidate, according to the user.
- Extended diagnostic FOV trace is absent from this candidate log.
- Release packaging and release approval remain outside this task.

### Batch 3 — Result gate

- Classify the loss point as one of: settings layer, native camera rebuild,
  gameplay replay, or unobserved/unknown.
- Stop if the trace does not identify a concrete owner.
- Only after a concrete owner is identified, create a separate implementation
  plan or amend this plan with explicit approval.

## Validation

- The trace must contain a clear pre-Apply approximately-120 observation and a
  clear post-Apply observation.
- Dialogue and cinematic lifecycle records must be absent from the test window.
- If a camera source changes, record both source identities and their FOV
  values.
- A successful trace is evidence only for the tested Steam 2.0.4 runtime.

## Risks and safe-failure

- Do not infer a stale replay write solely from a repeated 120 value.
- Do not change the dialogue baseline logic to compensate for an unresolved
  gameplay settings path.
- If the owner is ambiguous, preserve the current build and report the trace
  as inconclusive.

## Stop conditions and final review

- Stop before implementation when the loss point is not concrete.
- Preserve the current ASI and release artifacts unchanged.
- Perform a read-only Git review after any approved diagnostic change.
- Do not stage, commit, tag or push.
