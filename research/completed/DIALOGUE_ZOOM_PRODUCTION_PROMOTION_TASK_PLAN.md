# Dialogue Zoom Production Promotion — Task Plan

## Objective

Promote the validated dialogue sample-transform model into the production
dialogue subsystem for v0.5.0.

The public policies will be:

```text
Native → Adaptive → Reduced → Disabled → Native
```

`Reduced` will use the validated optical half-strength model formerly named
`OpticalReduced` in the feasibility build.

## Established evidence and current state

- The current 2.0.4 dialogue boundary at RVA `0xD20F77` is uniquely resolved
  and instruction-validated.
- Native dialogue FOV uses a live stream whose EXIT begins with a large native
  sample discontinuity.
- The validated EXIT anchor/recovery transform removes the visible jump while
  preserving recovery to the actual captured gameplay baseline `G`.
- Adaptive and optical half-strength endpoint calculations match runtime
  observations at gameplay baselines near `90.9`, `110.9` and `120.9`.
- Native, Disabled, sequential dialogue cycles, cinematic isolation and ADS
  specificity have already passed their bounded tests.
- Feasibility reported a small Adaptive first-EXIT delta of approximately
  `-0.138°`; this is retained as an observation, not a blocker.

## Approved scope

- Add public `Adaptive` dialogue policy.
- Promote feasibility `OpticalReduced` behavior to public `Reduced`.
- Promote the validated EXIT anchor/recovery logic.
- Keep `Native` pass-through and `Disabled` behavior unchanged.
- Update config synchronization, defaults and comments:
  `Zoom=Reduced`.
- Update the dialogue hotkey cycle to
  `Native → Adaptive → Reduced → Disabled → Native`.
- Keep default `DialogueCycle=F10`; do not document F8 as a production
  binding.
- Run the bounded production runtime regression matrix.

## Explicit non-goals

- No new reverse engineering, resolver or hook boundary.
- No changes to cinematic isolation, ADS specificity, gameplay aspect or
  cinematic subsystems.
- No changes to the hotkey architecture or persistence mechanism.
- No new camera writes, timers, renderer hooks or fallback RVAs.
- No claim of runtime compatibility for 2.0.2/2.0.3 from static portability
  evidence alone.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `build-artifacts/test-scripts/build-experimental-cinematic-21x9-combined-fix-204.cmd`
- managed INI template/config documentation
- relevant README/release documentation
- a bounded runtime log and production-promotion research note

## Batches

1. **Production policy migration**
   - Add public `Adaptive`.
   - Rename the promoted optical policy to `Reduced`.
   - Preserve public default `Zoom=Reduced` and default `DialogueCycle=F10`.

2. **Lifecycle promotion**
   - Move the validated EXIT anchor/recovery behavior into the production
     dialogue path.
   - Preserve fail-closed native pass-through for invalid samples, baselines,
     coordinator state or resolver contracts.

3. **Configuration and documentation**
   - Update managed INI descriptions and policy cycle comments.
   - Ensure user-selected values survive template synchronization.
   - Do not mention the temporary F8 test binding.

4. **Build and regression validation**
   - Build the production candidate.
   - Test all four policies, high-FOV ENTER/EXIT behavior, two sequential
     dialogue cycles and cinematic-to-dialogue isolation.

5. **Final review**
   - Compare actual changed paths with this plan.
   - Record completed, remaining, deferred and not-runtime-validated items.
   - Prepare the v0.5.0 release wording only after the regression matrix passes.

## Validation

- Unique dialogue signature and expected instruction contract remain unchanged.
- Native: one complete dialogue cycle with no transformation.
- Adaptive: high-FOV cycle with smooth ENTER/EXIT and recovery to `G`.
- Reduced: optical half-strength cycle with smooth ENTER/EXIT and recovery to
  `G`.
- Disabled: FOV held at `G` through dialogue and restored cleanly.
- Two sequential dialogue cycles without stale policy or baseline state.
- Cinematic → gameplay recovery → dialogue baseline remains uncontaminated.
- F10 cycle selects all four policies in order and persistence remains intact.
- Build, `git diff --check`, relevant diff review and runtime identity logging.

## Risks and rollback / safe failure

- Production `Reduced` changes from the previously validated linear-degree
  behavior to the validated optical half-strength behavior. If regression
  fails, revert policy promotion while retaining the completed feasibility
  evidence.
- Any signature, instruction, sample or baseline failure must preserve native
  behavior and must not partially transform a dialogue cycle.
- Existing user INI values must be preserved during template migration.
- Rollback is limited to the dialogue policy/lifecycle changes; gameplay and
  cinematic hooks remain untouched.

## Stop conditions and phase gates

- Stop on resolver or instruction-contract mismatch.
- Stop if Native or Disabled regress.
- Stop if Adaptive or Reduced shows a visible EXIT discontinuity or stale state.
- Stop if F10 does not cycle all four policies or persistence changes unrelated
  settings.
- Stop after the bounded regression report; do not reopen feasibility or broad
  camera research in this task.

## Expected final Git review

Inspect status, relevant diff and changed paths against this plan. Explicitly
separate production-promoted behavior from pre-existing working-tree changes,
and report runtime coverage and limitations before considering v0.5.0 ready.
