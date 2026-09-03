# Dialogue Adaptive Zoom Feasibility — Task Plan

## Objective

Evaluate an optically consistent dialogue-zoom model on the existing live
dialogue FOV sample boundary before changing production behavior.

The test model is calibrated from the native 90° → 70° dialogue transition:

```text
Z = tan(90° / 2) / tan(70° / 2)
A(G) = 2 * atan(tan(G / 2) / Z)
R(G) = 2 * atan(tan(G / 2) / sqrt(Z))
```

Where `G` is the captured gameplay baseline, `A` is Adaptive and `R` is the
optical half-strength Reduced target.

For a live native sample `N`, preserve transition progress in projection space
instead of replacing it with an endpoint. With `D=70°`, define:

```text
p = (tan(N / 2) - tan(G / 2)) / (tan(D / 2) - tan(G / 2))
T(P) = 2 * atan(tan(G / 2) + p * (tan(P / 2) - tan(G / 2)))
```

The test output is `T(A)` for Adaptive and `T(R)` for optical Reduced. The
mapping must be clamped to the valid native transition interval and must pass
through unchanged outside an active valid lifecycle.

## Established evidence and current state

- The current 2.0.4 dialogue boundary is the validated unique call boundary at
  RVA `0xD20F77`.
- `XMM1` carries a live native blend stream, not a one-shot target assignment.
- The current production policies are `Native`, linear-sample `Reduced` and
  `Disabled`; all have passed the bounded 2.0.4 runtime matrix.
- Cinematic isolation and ADS specificity have passed in the tested scenarios.
- Production `Reduced` must remain unchanged until this feasibility pass is
  complete.

## Approved scope

- Create a test-only Adaptive policy on the existing dialogue boundary.
- Evaluate live-sample progression rather than replacing every sample with a
  fixed endpoint.
- Evaluate optical half-strength `OpticalReduced` separately from the current linear
  production implementation.
- Use the test hotkey cycle to select `Native`, `Adaptive`, `OpticalReduced`
  and `Disabled` during one game session.
- Use controlled gameplay FOV values `90°`, `110°` and `120°`.
- Preserve the existing native lifecycle, baseline capture, recovery and
  cinematic isolation behavior.
- Produce a bounded report comparing endpoints, stream shape and visual/runtime
  results.

## Explicit non-goals

- Do not change production `Reduced` behavior during the feasibility pass;
  optical half-strength uses the test-only name `OpticalReduced`.
- Do not make `Adaptive` the default or add it to the public INI yet.
- Do not alter `Native` or `Disabled`.
- Do not add a new resolver, hook, camera write, timer or renderer path.
- Do not change hotkeys, persistence, cinematic logic or the stable gameplay
  aspect implementation.
- Do not claim optical equivalence until the `G=90°` sanity check passes.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `DIALOGUE_ZOOM_ADAPTIVE_FEASIBILITY_TASK_PLAN.md`
- test ASI output and a bounded runtime log under `build-artifacts`
- a research report under `02-Research/Ghidra/reports` or the repository
  research area, without overwriting existing production reports

## Batches

1. **Model and test-contract review**
   - Confirm the exact endpoint and live-sample transformation model.
   - Keep current production policy behavior intact.

2. **Test-only implementation**
   - Add the experimental `Adaptive` and `OpticalReduced` policies and
     diagnostics behind the existing dialogue boundary.
   - Extend the test-only F10 cycle to
     `Native → Adaptive → OpticalReduced → Disabled → Native`.
   - Keep all invalid/failure paths native pass-through.

3. **Controlled runtime matrix**
   - Run full dialogue cycles at `G=90°`, `110°` and `120°`.
   - Compare Native, Adaptive, OpticalReduced and Disabled endpoints.
   - Confirm recovery and absence of stale transient state.
   - For Adaptive and OpticalReduced, capture a bounded EXIT neighborhood:
     the last 12 pre-exit samples and the first 24 post-exit samples, including
     incoming/output values, deltas and normalized native progress.

4. **Bounded review**
   - Record whether live transition shape is preserved.
   - Verify that `G=90°` makes the Adaptive mapping an identity transform for
     every observed native sample, within the documented numeric tolerance.
   - Decide whether Adaptive or optical Reduced is a candidate for a separate
     production-design task; do not implement that promotion here.

## Validation

- Build the test ASI with the existing unified build script.
- Inspect the diff and run `git diff --check`.
- Verify runtime identity and the existing dialogue-boundary contract remain
  unchanged.
- Validate `G=90°`: Adaptive must match Native through the transition, not only
  at steady state.
- Validate `G=110°` and `G=120°`: endpoint values must match the formulas within
  a documented tolerance.
- Confirm Native/Disabled behavior and cinematic/ADS isolation do not regress.
- Keep runtime claims limited to the actual 2.0.4 test matrix.

## Risks and rollback / safe failure

- The boundary receives intermediate native samples; an endpoint-only rewrite
  could distort timing or curve shape. The test must preserve native progress.
- Invalid baseline, non-finite sample, invalid range or contract mismatch must
  pass the original sample through unchanged.
- If the model cannot preserve the `G=90°` native-equivalence sanity check,
  reject it without changing production code.
- Rollback is removal of the test-only policy/diagnostics; no stable production
  policy is migrated in this task.

## Stop conditions and phase gates

- Stop immediately on resolver/instruction identity mismatch.
- Stop if the live-sample mapping cannot be defined without a new broad search.
- Stop and reject the model if `G=90°` Adaptive differs materially from Native.
- Stop if the test changes cinematic behavior, ADS specificity or recovery.
- Stop after the bounded report and promotion recommendation; do not expand into
  general camera or renderer research.

## Expected final Git review

Compare the actual changed paths with this plan, identify intentionally
untouched production behavior, record build/runtime limits, and classify the
result as `PASS`, `REJECTED` or `INCONCLUSIVE`. If implementation and runtime
validation both pass, prepare a separate promotion plan rather than silently
merging the experimental policy into the stable release path.
