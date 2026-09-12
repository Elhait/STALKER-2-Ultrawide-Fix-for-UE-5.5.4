# Camera Evaluation Automatic UE4SS Dump — Task Plan

## Objective

Create a read-only UE4SS Lua diagnostic mod that automatically records the
live `CameraComponent`, `Stalker2.CameraManager` and `PlayerCameraManager`
state around gameplay aspect transitions without requiring manual object-ID
lookup.

## Established evidence and current state

- UE4SS is already installed and working in the game.
- Object IDs change after a reload, so hard-coded individual dump names are
  unsuitable.
- The relevant live ownership chain is character `CameraComponent` →
  `Stalker2.CameraManager` → `PlayerCameraManager`.
- The gameplay fix uses a native two-pass aspect transition; the diagnostic
  must observe that transition, not emulate it.

## Approved scope

- UE4SS Lua diagnostic mod only.
- Locate live objects by class/path, not by numeric object ID.
- Poll at a bounded interval and emit snapshots only when tracked state
  changes or when explicitly requested by a hotkey.
- Record camera-relevant reflected fields and object paths/references.
- Keep a short lifecycle history: baseline, transition states and stable
  state.

## Explicit non-goals

- No property writes or Live View edits.
- No calls to `DumpAllObjects`, `.jmap`, `.usmap` or full object dumping.
- No ASI changes, camera hooks, renderer hooks or gameplay changes.
- No broad `ForEachUObject` scan on every tick.
- No assumption that a particular aspect/FOV/constraint value is correct.

## Expected files/areas

- `research/ue4ss/CameraEvaluationAutoDump/main.lua`
- `research/ue4ss/CameraEvaluationAutoDump/README.md`
- No production source or release files.

## Batches

### Batch 1 — bounded diagnostic implementation

Implement object discovery, state sampling, change detection, console/log
output and manual snapshot hotkey.

### Batch 2 — user runtime validation

Run with gameplay fix enabled. Capture before, during and after one gameplay
aspect transition. Verify that the mod finds current objects after reloads and
does not write values.

## Validation

- Lua syntax/static review.
- Confirm only the planned research files are created.
- Runtime validation is user-performed in the game; this task does not claim
  runtime success until the generated log is inspected.

## Risks and safe failure

- `FindAllOf` may be slow, so discovery is throttled and cached.
- Multiple camera objects may exist; ambiguous matches are logged and no
  property is modified.
- Missing objects produce a status line and retry later.
- Exceptions in sampling are contained per object where supported by Lua.

## Stop conditions and phase gates

- Stop if locating the relevant objects requires hard-coded IDs or broad
  renderer/static scanning.
- Stop before any write or hook implementation.
- Promote results only after snapshots identify the same live objects across
  the complete gameplay transition.

## Final review

- Confirm no production files changed.
- Confirm the diagnostic records observed values separately from hypotheses.
- Perform a read-only path/diff review; do not run Git state-changing commands.
