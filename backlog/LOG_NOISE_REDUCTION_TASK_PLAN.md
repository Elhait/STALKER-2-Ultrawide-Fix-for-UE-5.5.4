# LOG_NOISE_REDUCTION

## Objective

Remove repetitive coordinator informational messages emitted for every
gameplay-writer observation during `CinematicActive` and every intermediate
native recovery observation, while preserving lifecycle, recovery and replay
evidence.

## Established evidence and current state

The unified ASI is runtime-validated on 2.0.4. The messages
`Global coordinator: gameplay writer observed during CinematicActive; replay
suppressed.` and `Global coordinator: native recovery observed; replay
suppressed.` repeat at high frequency and do not add causal information.

## Approved scope

- Remove only the repeated `CinematicActive` and intermediate-recovery log calls.
- Preserve suppression behavior and all other gameplay/cinematic logs.
- Rebuild the unified `STALKER2UltrawideFix.asi`.

## Explicit non-goals

- No hook, resolver, state-machine or configuration changes.
- No cinematic/gameplay behavior changes.
- No release packaging or Git state changes.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- rebuilt `build-artifacts/test-asi/STALKER2UltrawideFix.asi`

## Batches and validation

### Batch 1 — log-noise reduction

Remove the repetitive log statements and retain the existing early returns and
recovery completion event.

Validation: build succeeds; source whitespace check passes; affected source
area contains no matching spam message.

## Risks and rollback or safe-failure behavior

The change is logging-only. Coordinator suppression and recovery detection
remain active because the state branches and early returns are unchanged.
Rollback is limited to restoring the removed informational log statements if
needed.

## Stop conditions and phase gates

Stop on build failure, unexpected source changes or any hook/state diff.
Runtime behavior is not revalidated by this logging-only build; the next user
launch should confirm the spam line is absent while normal lifecycle lines
remain present.

## Final Git review

Perform a read-only status/diff review and confirm only the approved source
and task-plan paths changed; do not stage or commit.
