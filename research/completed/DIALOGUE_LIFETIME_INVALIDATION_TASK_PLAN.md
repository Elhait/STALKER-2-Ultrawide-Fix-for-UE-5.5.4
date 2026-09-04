# Dialogue Lifetime Invalidation Task Plan

## Objective

Prevent the dialogue FOV classifier from starting a false lifecycle after
cinematic recovery and from reusing a captured gameplay baseline after the
authoritative camera/FOV context has changed.

## Established evidence and current state

- The renamed ASI loaded successfully on Steam 2.0.4 and installed the
  validated dialogue boundary.
- In the supplied runtime log, cinematic recovery completed at
  `nativeTargetFov=120.967`, immediately followed by
  `Dialogue lifecycle started: baselineG=120.967` without confirmed dialogue.
- A later camera rebuild/settings save produced gameplay FOV near `90.3443`,
  while the transient dialogue state still held `baselineG=120.967`.
- The later exit record used the stale baseline, explaining the observed FOV
  escalation/recovery error.
- Rename and removal of legacy INI migration are not established as the cause;
  the failure is assigned to dialogue transient-state lifetime.

## Approved scope

- Introduce a provisional dialogue-candidate state before `DialogueActive`.
- Promote the candidate only after a confirmed descending native sample.
- Define and implement the narrowest reliable observable for invalidating a
  candidate/active baseline when its authoritative gameplay camera context is
  no longer current.
- Reset transient dialogue state on invalidation and preserve native
  pass-through for the ambiguous sample/cycle.
- Preserve current production policy names and semantics:
  `Native`, `Adaptive`, `Reduced` and `Disabled`.
- Add the reproduced high-FOV/cinematic/settings-change scenario to bounded
  runtime regression.

## Explicit non-goals

- No changes to dialogue optical math or projection-space formulas.
- No changes to the `0xD20F77` hook boundary, signature resolver or instruction
  contract.
- No changes to cinematic coordinator ownership or cinematic FOV logic.
- No timers, guessed delays or broad camera heuristics.
- No rename, filename, packaging or release-identity work.
- No automatic migration of previous INI settings.
- No production release promotion until the regression gate passes.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- Directly relevant test/build files only if required for bounded validation.
- This plan and the factual task log/report after validation.

## Implementation batches

### Batch 1 — State and invalidation design review

- Inspect the current dialogue state fields and all reset paths.
- Identify observable camera/coordinator/FOV context already available at the
  boundary.
- Select the narrowest invalidation signal supported by existing evidence.
- Do not edit production source until the signal and state transitions are
  explicitly recorded.

#### Batch 1 result

- Existing `CoordinatorState::CinematicExiting` recovery completion is an
  authoritative invalidation point: when native cinematic recovery reaches its
  target, all dialogue transient state is reset before returning to Gameplay.
- The existing gameplay camera rebuild observation in
  `ReplayManualTransitionOriginal` is the second bounded invalidation point;
  it resets dialogue transient state before re-arming the gameplay transition.
- No dialogue-boundary camera pointer or broad FOV heuristic was added. The
  boundary continues to use only its established incoming sample contract.
- The first boundary sample will become `Candidate` and remain native
  pass-through. Only a later confirmed descending sample promotes it to
  `Active` and commits the baseline.
- This design is bounded to existing coordinator/camera lifecycle signals and
  does not introduce timers, new hooks or changes to dialogue math.

### Batch 2 — Minimal classifier correction

- Add `DialogueCandidate` or equivalent provisional state.
- Capture provisional `G` without applying a transform.
- Promote only on confirmed native descent.
- Reset candidate/active state when the selected context invalidation signal is
  observed; pass the current sample through natively.
- Keep the existing transform and EXIT anchor/recovery logic unchanged.

#### Batch 2 result

- Added `DialoguePhase::Candidate`; the first valid gameplay boundary sample
  captures a provisional baseline and remains native pass-through.
- A candidate promotes to `Active` only after the next confirmed descending
  native sample. Existing Adaptive/Reduced/Disabled transforms and EXIT
  recovery formulas were not changed.
- Centralized transient-state reset and invoked it on non-Gameplay coordinator
  observations, Native policy pass-through, cinematic native recovery
  completion and the existing gameplay camera rebuild observation.

### Batch 3 — Build and static review

### Batch 3 — Build and static review

- Build the bounded production candidate.
- Confirm only the intended classifier/lifetime code changed.
- Verify the dialogue boundary and resolver contract are unchanged.
- Confirm no timer, alternate hook or diagnostic-only behavior was introduced.

#### Batch 3 result

- Build succeeded with the existing production compile configuration
  (`/DNDEBUG`, no feasibility diagnostic defines).
- New candidate ASI:
  `build-artifacts/test-asi/STALKER2CameraTweaks.asi`
- SHA-256:
  `08C07F6398DF9D7BF562EAD97852EA6E3E145EBCB8280430F2073C04027F7E50`
- Static diff is limited to the dialogue phase/reset handling and this plan;
  the resolver signature, dialogue boundary and optical transform functions
  remain unchanged.
- No timer, alternate hook or new diagnostic instrumentation was introduced.
- Runtime validation is still pending; this candidate is not release-approved.

### Batch 4 — Runtime regression

- Run the reproduced high-FOV scenario and collect the new log.
- Run ordinary dialogue at approximately 90 FOV.
- Run ordinary dialogue at approximately 110/120 FOV.
- Run two sequential dialogue cycles.
- Run cinematic → gameplay recovery → dialogue.
- Confirm no stale baseline recovery and no false lifecycle after cinematic
  recovery.

### Batch 5 — Final review

- Compare actual files and behavior with this plan.
- Record completed, remaining, deferred, blocked and not-runtime-validated
  items.
- Update `backlog/TASKLOG.md` only after implementation and relevant validation
  are complete.
- Resume the paused product-identity rename task only after this plan passes.

## Validation and acceptance criteria

1. The first boundary sample after `Gameplay` becomes provisional only; no
   transform is applied.
2. The candidate becomes active only after confirmed descending native
   progression.
3. Candidate/active state cannot survive a material change in the authoritative
   gameplay camera context captured with `G`.
4. Ambiguous or invalid sequences use native pass-through and reset; no partial
   recovery targets a stale baseline.
5. The reproduced sequence passes:

   `FOV ~120 → cinematic → recovery → gameplay/settings FOV ~90 → dialogue`

   The final dialogue recovery returns to the current ~90 baseline, never the
   stale ~120 baseline.
6. Existing Native/Adaptive/Reduced/Disabled behavior remains intact in the
   bounded regression matrix.
7. No false dialogue lifecycle starts at cinematic recovery.
8. No timer or unrelated camera hook is introduced.

## Risks and rollback / safe-failure

- Requiring a too-specific context signal could miss invalidation; requiring a
  broad signal could reset valid dialogue state. Prefer existing authoritative
  coordinator/FOV evidence and record the tradeoff.
- If lifecycle confirmation is ambiguous, pass through natively and reset rather
  than transforming against an uncertain baseline.
- If the regression fails, retain the prior source/build artifacts for evidence,
  do not promote the candidate, and return to this plan without touching rename
  work.

## Stop conditions and phase gates

- Stop if no reliable invalidation observable can be established without a new
  broad runtime research branch.
- Stop if the fix requires changing dialogue math, hook ownership, cinematic
  coordinator logic or resolver semantics.
- Stop on any false-positive lifecycle, stale baseline recovery or regression in
  Native behavior.
- Production promotion and product-identity rename remain blocked until all
  bounded acceptance criteria pass.

## Expected final Git review

- Report exact production source/test files changed.
- Report the selected invalidation signal and why it is bounded.
- Report build identity and runtime log identity.
- Report each regression scenario and its limits.
- Confirm rename files/artifacts were untouched by this task.
- Confirm no commit, tag or push was performed without separate approval.
