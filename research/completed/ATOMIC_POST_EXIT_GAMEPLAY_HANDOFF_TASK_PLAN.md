# ATOMIC_POST_EXIT_GAMEPLAY_HANDOFF

Status: Closed — the isolated atomic B/C experiment passed mechanically but
failed to remove the visible post-EXIT seam. No production policy was adopted.

## Objective

Determine whether the existing gameplay two-pass correction can be scheduled at
the validated native EXIT recovery boundary without exposing the intermediate A
state, while preserving the existing gameplay algorithm and native cinematic
recovery.

## Established evidence and current state

- Cinematic ENTER aspect and live-FOV mechanisms are independently validated on
  the 2.0.4 executable.
- Native EXIT FOV recovery is game-owned and converges to approximately
  `90.6557`; it must remain untouched.
- At `A_RECOVERY_COMPLETE`, gameplay is visually too narrow when the existing
  gameplay replay is consumed.
- The existing gameplay correction requires B/C: constrained pass followed by
  Auto restore. Consuming B/C leaves A visibly incorrect.
- The current stable gameplay algorithm is accepted outside cinematic handoff
  and must not be rewritten.

## Approved scope

- Read-only/static audit of the isolated global coordinator and its copied
  gameplay state machine.
- Determine B-to-C state and invocation dependencies.
- Determine how to prevent duplicate replay after an atomic handoff.
- Select a scheduling model only when justified by source-level evidence.
- If justified, implement and build a separate experimental artifact only;
  stable gameplay source and release assets remain untouched.

## Explicit non-goals

- No changes to cinematic ENTER, live-FOV transform, aspect immediate patch or
  native EXIT recovery.
- No changes to `src/gameplay_aspect_fix.cpp`.
- No new renderer/presentation search, camera hooks, timers, polling or guessed
  frame delays.
- No manual FOV restore or alternative projection mechanism.
- No production release or replacement of the stable gameplay ASI.

## Expected files and areas

- `src/cinematic_global_coordinator_integration_204.cpp`
- `src/post_exit_handoff_state_classification_at_a_204.cpp`
- `src/gameplay_aspect_fix.cpp` (read-only reference only)
- `build-artifacts/test-scripts/` and `build-artifacts/test-asi/` only if a
  later implementation batch is approved.

## Batches and gates

### Batch 1 — static dependency audit

Inspect `ReplayManualTransitionOriginal`, coordinator transitions, B/C writes,
state updates, and the next-writer re-arm condition. Produce a source-backed
contract for whether B and C can run in one invocation, require two invocations,
or require a native propagation boundary.

Validation: source inspection only; no build, game launch or runtime hook.

Gate: B/C dependency and duplicate-replay prevention must be understood. If not,
stop before implementation.

#### Batch 1 result

- B is the `WaitingForAutomaticUpdate` branch. It requires the observed wide
  aspect with flags `0x04`, writes the constrained state (`aspect=3.5556`,
  `flags=0x05`) and sets `g_state=AppliedConstrainPass`.
- B returns immediately after that write. C is the following
  `AppliedConstrainPass` branch and reads the camera fields again before writing
  the native aspect and gameplay flags (`aspect=1.7778`, `flags=0x04`) and
  setting `g_state=Complete`.
- The source audit found no explicit dependency on a separate native return,
  output propagation step or another invocation-local value between B and C.
  C's required inputs are the state-machine state and the source fields that B
  itself updates.
- Therefore same-invocation B-then-C replay is statically justified as the
  primary scheduling model for the isolated experiment. It must call the
  existing B/C logic in order and re-read the source between phases.
- After C, the source is already `1.7778/0x04`, so the normal wide/`0x04`
  re-arm predicate does not match. The implementation must still keep an
  explicit one-handoff guard and log it to prevent accidental duplicate replay.

Batch 1 gate: PASS for a separate isolated implementation batch. Runtime proof
is still required; this source result does not establish visual atomicity.

### Batch 2 — isolated implementation, only if Batch 1 passes

Create a separate experimental coordinator artifact implementing the justified
scheduling model. Preserve the copied B/C operations exactly, add explicit
single-handoff bookkeeping, and keep native recovery untouched.

Validation: compile the isolated artifact and inspect the diff against the plan.

#### Batch 2 result

- Created isolated source:
  `src/atomic_post_exit_gameplay_handoff_204.cpp`.
- At validated native recovery completion, the artifact performs the existing
  gameplay replay B, re-reads the source and replay state, then performs C only
  when B established `AppliedConstrainPass`.
- The native EXIT recovery path remains untouched. Cinematic FOV/aspect
  mechanisms are copied unchanged into the experimental artifact.
- Added a one-shot handoff guard, explicit B/C result logging and fail-closed
  behavior when B's contract is not established.
- Created build script:
  `build-artifacts/test-scripts/build-atomic-post-exit-gameplay-handoff-204.cmd`.
- Build passed and produced:
  `build-artifacts/test-asi/STALKER2AtomicPostExitGameplayHandoff204.asi`.
- Stable `src/gameplay_aspect_fix.cpp` was not edited by this batch. The working
  tree already contains unrelated/pre-existing staged and untracked research
  artifacts; they were preserved.

Batch 2 gate: PASS for bounded runtime validation. Build correctness is not
runtime or visual proof.

### Batch 3 — bounded runtime validation, only if Batch 2 passes

Run one `cinematic -> gameplay` test with only the isolated artifact enabled.
Verify recovery completion, B/C ordering, no duplicate replay, correct final
gameplay projection and absence of the exposed A seam.

Validation limits: a successful build is not runtime proof; user visual
observation and the artifact log are required.

#### Batch 3 result

- The atomic artifact reached native recovery completion and executed B and C
  at the same timestamp and recovery boundary.
- B and C ordering and one-handoff bookkeeping were correct in the log.
- Visual result: the user still observed a brief FOV/projection shrink followed
  immediately by expansion.
- Therefore atomic same-invocation scheduling did not remove the visible seam.
  This rejects the hypothesis that the seam is caused only by the gap between
  separate writer invocations.
- The result is consistent with a hidden dependency or downstream presentation
  propagation between the intermediate states, even within one hook invocation.
  No new renderer search or unbounded instrumentation is authorized by this
  plan.

Batch 3 gate: FAIL for visual seamlessness. The native recovery and B/C
execution mechanics remain functionally intact, but the atomic handoff is not a
production solution.

## Risks and safe failure

- Running B/C before native recovery completes could corrupt the handoff; gate on
  the validated recovery endpoint.
- Running B/C twice could recreate the existing seam or alter gameplay state;
  use one-shot handoff bookkeeping and fail closed on ambiguity.
- If B/C requires separate native writer invocations, do not force synchronous
  execution; stop and document the dependency.
- Any install/signature mismatch must refuse safely without writes.
- Rollback is removal of the isolated experimental artifact; stable files are
  not modified.

## Stop conditions and phase gates

- Stop if the B/C dependency cannot be proven from source.
- Stop if atomic scheduling requires a new timer, polling loop or guessed delay.
- Stop if the implementation would alter stable gameplay behavior outside the
  cinematic handoff.
- Stop on contradictory executable identity, signature or runtime evidence.

## Final review requirements

- Perform read-only Git status and diff review after every implementation batch.
- Compare changed paths with this plan and identify intentionally untouched
  stable files.
- Report completed, remaining, deferred, blocked and not-runtime-validated
  items.
- Do not call the phase complete until the isolated artifact and its runtime
  behavior have been reviewed against the acceptance criteria.
