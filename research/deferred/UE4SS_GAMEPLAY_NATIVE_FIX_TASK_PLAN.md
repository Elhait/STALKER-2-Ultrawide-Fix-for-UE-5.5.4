# UE4SS Gameplay Native-Like Fix — Task Plan

## Objective

Re-analyze the confirmed UE4SS gameplay A→B→C evidence and replace the
failed one-step gameplay storage mutation in the parallel
`STALKER2CameraTweaks` UE4SS module with a bounded native-like two-pass
sequence for gameplay only.

## Established evidence and current state

- Native gameplay control preserves authored `FOV=90`.
- Confirmed sequence is:
  `3.555556/0x04 → 3.555556/0x05 → 1.777778/0x05 → 1.777778/0x04`.
- Stable correct gameplay C-state is `FOV=90`, `AspectRatio=1.777778`,
  `Constrain=false`, with the physical 32:9 view correctly framed.
- The previous UE4SS port only attempted the final storage state and did not
  reproduce the two-pass transition; gameplay therefore remains unvalidated.
- UE4SS complex ref/out invocation and native projection refresh remain
  unavailable/unproven.

## Approved scope

- Modify only the UE4SS prototype's gameplay path.
- Keep dynamic CameraComponent/CameraManager/PCM discovery.
- Implement a bounded state machine that detects wide unconstrained gameplay and
  applies the confirmed order through simple value-only reflected setters:
  constrained bit on, native aspect while constrained, constrained bit off.
- Preserve the current gameplay FOV and avoid cinematic/dialogue writes.
- Log each phase, setter route, accepted/blocked result and observed Camera/PCM
  state so the next runtime test can distinguish mutation from visual success.
- Update the UE4SS README/config comments to reflect gameplay-only validation.

## Explicit non-goals

- Do not modify or rebuild the production ASI.
- Do not modify cinematic or dialogue behavior in this batch.
- Do not call `GetCameraView`, `BlueprintUpdateCamera` or any ref/out function.
- Do not use raw offsets, guessed pointers, native hooks or broad renderer work.
- Do not claim native equivalence or visual success before in-game evidence.
- Do not deploy files into the installed game directory automatically.

## Files/areas expected to be touched

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/STALKER2CameraTweaks.ini`
- `research/ue4ss/STALKER2CameraTweaks/README.md`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks.zip`
- `backlog/TASKLOG.md`

## Batches and validation

### Batch 1 — A/B/C implementation correction

Replace the one-step gameplay write with the bounded two-pass state machine.
The state machine must re-arm after camera object replacement and must stop
after reaching the final C-state or after a bounded retry limit.

Validation: source inspection, structural checks, and `git diff --check`.

### Batch 2 — Package and static review

Rebuild the test ZIP with only the UE4SS module files. Confirm cinematic and
dialogue mutation paths are disabled for this gameplay test build.

Validation: archive content listing, relevant-path Git review and no runtime
claim until the user supplies a game log/visual result.

## Risks and rollback

- Setter acceptance may still update storage without triggering downstream UE
  projection rebuild. The module must log this limitation and stop retrying.
- A constrained gameplay setting can resemble cinematic entry. This batch
  uses only the gameplay-wide trigger (`Aspect > 16:9`, unconstrained) and does
  not classify constrained states as gameplay targets.
- If a setter fails or an object becomes invalid, clear the state machine and
  leave the game untouched. Rollback is disabling the UE4SS module.

## Stop conditions and phase gates

- No cinematic or dialogue validation until gameplay A/B/C passes separately.
- Stop after one bounded two-pass attempt per detected camera identity; do not
  add frame-delay loops or additional property variants in this batch.
- If the final state is reached in fields but visual framing remains wrong,
  classify the UE4SS gameplay path as `storage-only / native reevaluation
  missing`, not as a working fix.

## Final review

- Inspect changed paths against this plan.
- Confirm production ASI/source files are untouched.
- Report implementation status separately from runtime validation status.
- Archive this plan after the static review; retain runtime follow-up as a
  separate task if required.
