# Post-EXIT Handoff State Classification at A — Task Plan

Status: Planned — no implementation or runtime artifact started.

## Objective

Determine which gameplay state must exist at the validated
`A_RECOVERY_COMPLETE` boundary to avoid exposing a visible post-cinematic
projection seam, using only the already validated EXIT, recovery and gameplay
transition mechanisms.

## Established evidence and current state

- Cinematic ENTER aspect immediate patch and live FOV transform are runtime
  validated at executable 2.0.4.
- Native EXIT FOV recovery is game-owned and converges from approximately
  `126.87` to `90.6557` without manual FOV restoration.
- At `A_RECOVERY_COMPLETE`, the same source object is observed with FOV about
  `90.6557`, aspect `3.5556` and flags `0x04`.
- The existing gameplay transition then performs constrained pass `B`
  (`flags 0x04 -> 0x05`) and Auto restore `C` (`aspect 3.5556 -> 1.7778`,
  flags `0x05 -> 0x04`).
- The A/B/C writer-output trace did not expose a meaningful change in its
  tracked output fields, while the user observed a visible post-EXIT FOV/
  projection seam.
- The downstream candidate `FUN_140AF4FA4` was rejected as presentation
  causal; downstream consumer search is closed for the current evidence set.

## Approved scope

- Classify the handoff state at A using controlled one-cycle evidence.
- Compare A, B and C with the existing validated source/output observations
  and user-visible result.
- Permit an isolated diagnostic artifact to consume the ordinary post-recovery
  B/C replay for one handoff solely to expose naturally reached A; this
  diagnostic behavior is not a production policy.
- Consider only bounded policy changes built from existing native/stable
  primitives after classification.
- Keep the stable gameplay implementation and frozen cinematic mechanisms
  unchanged during classification.

## Explicit non-goals

- No renderer, projection-matrix or downstream-consumer search.
- No new FOV/aspect mechanism or new camera hook.
- No direct restore of the old gameplay FOV.
- No production suppression policy is authorized during classification.
- No modification of the coordinator or stable gameplay source.
- No timer, sleep, polling, frame counter or guessed delay.
- No changes to `src/gameplay_aspect_fix.cpp`, release artifacts or the frozen
  coordinator before an outcome gate authorizes a separate implementation
  batch.
- No second-cinematic validation in this task.

## Expected files or areas

- This task plan only before the classification decision.
- If a diagnostic artifact is authorized, it must be isolated under `src/`
  and `build-artifacts/test-scripts/`, with a separate test `.asi`.
- Existing coordinator, stable gameplay source and prior feasibility artifacts
  are reference inputs and must remain untouched.

## Batches

### Batch 1 — Handoff-state classification design

Define the A/B/C observations and the three outcome gates without changing
runtime behavior.

Validation: plan review against the established A/B/C log and visual report.

### Batch 2 — One-cycle controlled classification

Prepare an isolated diagnostic artifact that, for exactly one validated
post-recovery handoff, consumes the ordinary B/C replay without writing FOV,
aspect, flags, output state or invoking alternative camera/projection logic.
This holds the naturally reached A state long enough for visual observation.
Then run one `cinematic -> gameplay` cycle and classify:

```yaml
Outcome A:
  held A is visually correct
  B/C replay is redundant for this handoff

Outcome B:
  held A is visually wrong
  ordinary B/C produces correct gameplay
  replay is required; seam is handoff exposure/atomicity

Outcome C:
  A is not an acceptable steady gameplay state
  and ordinary B/C cannot provide an acceptable handoff without exposing an
  intermediate projection state
  existing primitives need a dedicated handoff policy
```

Validation: user visual observation plus reproducible runtime log evidence.
The artifact must be disabled after the single classification cycle and must
remain separate from production coordinator behavior.

Implementation status: the isolated diagnostic artifact was built as
`STALKER2PostExitHandoffStateClassificationAtA204.asi`. It arms only after the
validated native recovery endpoint, consumes the post-recovery gameplay replay
for the handoff, logs `consumeCount=1` with `B_CONSTRAIN=false`,
`C_RESTORE=false` and `cameraWrites=0`, and keeps the behavior separate from
production policy. Runtime validation is pending.

### Batch 3 — Policy decision

Choose at most one bounded policy experiment based on the classified outcome.
No code change is authorized merely from a timestamp or source-field change.

Validation: separate plan/update before any implementation; fail closed if the
policy cannot preserve native EXIT recovery and the accepted gameplay contract.

## Risks and rollback / safe-failure behavior

- Consuming required B/C replay may leave gameplay in an incorrect projection
  state; the one-cycle experiment must be isolated and reversible.
- Replaying too early may expose the same seam; do not add a guessed delay.
- If A cannot be visually classified independently, mark the result
  unresolved rather than inferring it from source fields alone.
- Disable only the experimental artifact on regression; leave stable gameplay
  and release files unchanged.

## Stop conditions and phase gates

- Do not reopen closed downstream-consumer or renderer searches.
- Do not promote Outcome A/B/C from logs without the corresponding visual
  observation of held A where applicable.
- Do not treat diagnostic one-cycle B/C consumption as authorization for
  production suppression.
- If no existing boundary can distinguish the handoff policy safely, stop with
  the handoff state unresolved; do not broaden instrumentation.
- Any implementation requires a new approved batch after classification.

## Expected final Git review

Compare actual changed paths with this plan, confirm that no stable gameplay,
release or frozen cinematic source changed, record runtime limits, and separate
completed, remaining, deferred and blocked outcomes.
