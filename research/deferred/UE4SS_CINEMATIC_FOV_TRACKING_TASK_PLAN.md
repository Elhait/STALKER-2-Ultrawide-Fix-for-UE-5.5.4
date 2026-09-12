# UE4SS Cinematic Authored-FOV Tracking — Task Plan

## Objective

Preserve the UE4SS module's instant cinematic correction at cinematic ENTER
while transforming later native authored FOV updates without repeatedly
reapplying the same self-written value.

## Established evidence and current state

- The runtime log showed the previous cinematic one-shot FOV being overwritten
  by native camera interpolation back toward 90 degrees.
- The previous implementation also had a double Hor+ conversion, now corrected.
- Gameplay must preserve the user's/native gameplay FOV.
- Dialogue must not classify the gameplay aspect transition as dialogue.

## Approved scope

- Update only the UE4SS module cinematic FOV lifecycle.
- Keep instant ENTER application.
- Track later CameraComponent FOV changes while cinematic is active.
- Suppress reprocessing of the module's own transformed value.
- Keep gameplay and dialogue behavior unchanged.

## Explicit non-goals

- No production ASI changes.
- No native hooks, raw offsets or ref/out reflected calls.
- No new dialogue detection strategy.
- No claim of runtime success before a fresh user test.

## Expected files or areas

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/README.md` if behavior notes change
- `backlog/TASKLOG.md`
- test ZIP under `build-artifacts/test-ue4ss`

## Batches and validation

### Batch 1 — implementation

- Add cinematic authored-FOV tracking and self-write suppression.
- Preserve instant ENTER aspect/FOV application.

Validation: static marker checks, source/archive hash comparison and diff
review.

### Batch 2 — packaging

- Rebuild the test ZIP with only the module files.

Validation: inspect archive contents and confirm no production ASI is present.

## Risks and safe failure

- If native FOV changes are not distinguishable from self-written values, do
  not apply another correction in that poll.
- If cinematic lifecycle is unavailable, retain the existing state fallback.
- Runtime visual behavior remains unvalidated.

## Stop conditions and phase gates

- Stop if implementation requires complex reflected calls or guessed memory.
- Stop at static validation; user must perform the runtime test.

## Final review

Perform a read-only Git path/status review against this plan and record the
implementation result in `backlog/TASKLOG.md`.
