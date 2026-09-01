# GAMEPLAY_21_9_REBUILD_REGRESSION

## Objective

Determine why the stable gameplay aspect correction works after a manual
`16:9 -> 21:9` change but does not automatically reapply at startup or after a
death/respawn camera rebuild, then implement the smallest general lifecycle fix
that preserves dynamic aspect handling and existing 32:9 behavior.

## Established evidence and current state

- The stable gameplay fix preserves the game's FOV and uses an observed two-pass
  aspect lifecycle: constrained pass followed by Auto restore.
- Manual `16:9 -> 21:9` can trigger the expected correction.
- Startup and death/respawn rebuilds can leave gameplay in an incorrect
  `1.77778`/FOV state until the user repeats the manual workaround.
- Cinematic FOV/aspect research is separate experimental work and is frozen for
  this phase.

## Approved scope

- Diagnostic comparison of startup, successful manual 21:9 transition and
  death/respawn camera rebuild using the stable gameplay path.
- Inspect source and runtime logs for source identity, FOV, aspect, flags,
  selector, state-machine phase, rebuild detection and replay decisions.
- Implement only a general lifecycle/re-arm correction in the stable gameplay
  source if Batch 1 identifies a clear cause.
- Preserve dynamic display aspect and the player's FOV.

## Explicit non-goals

- No cinematic hooks, cinematic FOV transform or aspect immediate patch changes.
- No hard-coded `if 21:9` behavior or fixed aspect value.
- No FOV multiplier or manual FOV restore.
- No renderer/presentation search, timers, polling or guessed delays.
- No changes to experimental cinematic artifacts.
- No release packaging or runtime guarantee beyond the tested executable.

## Expected files or areas

- `src/gameplay_aspect_fix.cpp`
- existing gameplay diagnostic source/logs, if needed for comparison
- `backlog/TASKLOG.md` only after scoped validation and Git review

## Batches and validation

### Batch 1A — diagnostic sufficiency audit

Determine whether the existing stable logs distinguish source-state mismatch,
camera rebuild detection failure, state-machine re-arm failure and replay
condition rejection. Treat death/respawn as a possible camera rebuild until
pointer/runtime evidence confirms it.

Existing logs are insufficient when they omit the exact decision reason, so no
stable-source correction is authorized on their basis. Prepare one isolated
read-only observer if needed; it must not change aspect, FOV, flags, replay
decisions or cinematic behavior.

Validation: source/log inspection and observer build only; no stable behavior
changes.

### Batch 1B — three-scenario runtime comparison

Collect three separate clean sessions with only the stable gameplay ASI enabled
unless a read-only observer is explicitly substituted:

1. startup directly at 21:9;
2. manual `16:9 -> 21:9` successful correction;
3. death/respawn while remaining at 21:9.

Each scenario must record source/output identity when naturally available,
PrimaryFOV, SecondaryFOV, aspect, flags, selector, state before each decision,
rebuild detection and reason, re-arm considered/accepted/rejected and exact
reason, B/C execution and final state.

Validation: runtime logs and user visual observation only. Do not implement a
correction until the three traces identify one concrete lifecycle cause.

Gate: a concrete lifecycle cause must be identified before implementation.

#### Batch 1A result

- The existing stable gameplay log records source state and replay outcomes,
  but does not record exact re-arm/replay rejection reasons or distinguish a
  same-object native reset from a camera replacement.
- Created a separate observer-only artifact that preserves the decision shape
  for logging but suppresses all gameplay state writes. It logs source identity,
  state, aspect/flag predicates, re-arm decisions and would-write operations.
- Built:
  `build-artifacts/test-asi/STALKER2Gameplay21x9LifecycleObserver204.asi`.
- Stable gameplay source and cinematic artifacts were not modified.

Batch 1A gate: PASS. Batch 1B runtime collection remains pending.

#### Initial Batch 1B evidence — startup session

- On direct startup at 21:9, the observed source was `2.38889` (3440x1440)
  with flags `0x04` and `primaryFOV=90.6557`.
- The observer's state was `WaitingForAutomaticUpdate`; the constrained
  predicate reported `aspectMatch=false`, `flagsMatch=true`, `accepted=false`.
- This confirms that the current 32:9-specific predicate does not accept the
  observed 21:9 source state. It does not yet establish the complete fix because
  the observer intentionally performs no writes.
- Later manual aspect changes in the same process produced additional native
  states, including `2.38889/0x05` and `1.77778/0x04`, but this mixed session is
  not accepted as the clean manual comparison required by Batch 1B.

Status: Complete for the tested 2.0.4 candidate. Startup, manual aspect
transition, death/load rebuild and 32:9 regression passed runtime validation;
weapon/viewmodel FOV remains separate.

### Batch 2 — minimal stable-source correction

Modify only the stable gameplay lifecycle logic necessary to handle the proven
rebuild condition, without hard-coding 21:9 and without changing FOV ownership.

Validation: compile the stable ASI, inspect signatures and diff, and verify
fail-closed behavior remains intact.

#### Batch 2 preparation result

- The startup/death evidence identifies the current hard-coded 32:9 predicate
  as the first concrete cause: 21:9 arrives as `2.38889/0x04` and is rejected
  while the state remains `WaitingForAutomaticUpdate`.
- Updated only `src/gameplay_aspect_fix.cpp` so ultrawide detection accepts a
  finite aspect above native 16:9 and the constrained pass preserves the actual
  observed source aspect. The existing two-pass state machine and FOV handling
  remain otherwise unchanged.
- Created a separate build script and candidate binary:
  `build-artifacts/test-asi/STALKER2GameplayAspectFix21x9Candidate204.asi`.
- Build passed. This is a candidate, not a release artifact; runtime regression
  validation is still required.

### Batch 3 — bounded runtime regression validation

Validate startup, manual 21:9, death/respawn and a 32:9 regression case on the
identified 2.0.4 executable. Confirm automatic correction, preserved FOV and no
cinematic artifact interaction.

Validation limits: successful build is not runtime proof; runtime logs and user
visual observation are required.

#### Batch 3 result

- Candidate runtime test passed visually at 21:9.
- Startup reached `2.38889/0x04` and automatically executed B with the same
  source aspect, followed by C and `Complete`.
- Manual `21:9 -> 16:9 -> 21:9` produced no unwanted visual change.
- After death/load, the camera source changed and again reached `2.38889/0x04`;
  the candidate detected the rebuild, executed B/C and returned to `Complete`.
- No repeated B/C cycle was observed after completion, and FOV remained
  preserved at the gameplay value.
- The separate weapon/viewmodel FOV behavior after loading remains a known
  21:9-specific issue and is deferred to its own task.

Batch 3 gate: PASS for the tested 21:9 gameplay regression candidate. Release
promotion remains pending full stable-source diff review and final packaging;
weapon/viewmodel FOV is not part of this fix.

## Risks and safe failure

- Re-arming too broadly could replay the two-pass transition repeatedly; retain
  the existing state-machine completion guard.
- Changing the stable source can affect supported gameplay aspect behavior;
  fail closed when the validated camera-writer signature is absent or ambiguous.
- If startup/death state differs from the known manual trigger in a way that
  cannot be handled generally, stop without adding a 21:9-specific branch.
- Rollback is restoration of the stable source change; no cinematic artifact is
  modified.

## Stop conditions and phase gates

- Stop if Batch 1 cannot establish a concrete rebuild/re-arm cause.
- Stop if the proposed fix requires a hard-coded aspect or FOV value.
- Stop if the change alters cinematic behavior or the player's FOV contract.
- Stop on contradictory executable identity or signature evidence.

## Final review requirements

- Perform read-only Git status and diff review after each implementation batch.
- Compare changed paths with this plan and identify intentionally untouched
  cinematic/research artifacts.
- Report completed, remaining, deferred, blocked and not-runtime-validated
  items.
- Do not call the regression fixed until startup, manual 21:9, death/respawn and
  32:9 behavior have been reviewed against the acceptance criteria.
