# CINEMATIC_LETTERBOX_OBSERVER

Status: ACTIVE / BATCH 1

## Objective

Create a read-only UE4SS observer that records the first reflected state
changes around cinematic entry and their propagation through the known
CameraComponent → CameraManager → PlayerCameraManager chain.

## Established evidence and current state

- Native cinematic entry has been observed at FOV 90, aspect 1.777778 and
  constrained flags, producing side bars.
- A storage-only `bConstrainAspectRatio=false` write did not remove the bars;
  the game rewrote the state during cinematic evaluation.
- The same CameraComponent, Stalker2.CameraManager and PCM can be observed
  across gameplay/cinematic transitions.
- Existing general auto-dump output is too verbose and samples too slowly for
  precise first-change ordering.

## Approved scope

- New UE4SS Lua observer module only.
- Discover current objects by class/path without numeric IDs.
- Sample at a bounded interval and emit only ownership or state deltas.
- Record CameraComponent, CameraManager references, PCM Current, LastFrame and
  ViewTarget.POV relevant fields.
- Mark the first observed change for aspect, constrain and axis/override fields
  after an ownership reset.
- Support a low-conflict `End` marker without changing any object state.

## Explicit non-goals

- No property writes, Live View edits or manual slot calls.
- No UE4SS `RegisterHook`, function enumeration or native hooks.
- No ASI, production CameraTweaks or renderer changes.
- No full object dumps, broad UObject scans or renderer search.
- No claim that the first reflected change is the ultimate native causal owner.

## Expected files/areas

- `research/ue4ss/CinematicLetterboxObserver/main.lua`
- `research/ue4ss/CinematicLetterboxObserver/README.md`
- This task plan and no production files.

## Batches

### Batch 1 — observer implementation

- Add bounded discovery, delta-only state recorder and manual marker.
- Keep object identity/path and propagation source labels in every delta record.

Validation: Lua/source review and file/path review.

### Batch 2 — user runtime capture

- Run with cinematic CameraTweaks behavior disabled and UE4SS read-only.
- Capture at least two, preferably three, different cinematic entries.
- Compare first-change ordering and stable state.

Validation: user-supplied `UE4SS.log` evidence and visual correlation only.

## Risks and safe failure

- Missing or changing UE objects produce `OBJECTS_PENDING` and retry; no writes
  occur.
- Multiple candidates are classified by path and the observer uses the known
  Stalker2 ownership references when available.
- Sampling exceptions are contained per read and become `<unavailable>`.
- The observer is diagnostic only and has no failure path into camera logic.

## Stop conditions and phase gates

- Stop if acquisition requires hard-coded numeric IDs or broad scanning.
- Stop before any write or suppression experiment.
- Phase 1 passes only when deltas can be correlated across 2–3 cinematics.
- If ordering is inconsistent, classify the path as cinematic-type dependent;
  do not force a single owner conclusion.

## Final review

- Confirm only the planned research files changed.
- Report first-change ordering separately from causal-owner conclusions.
- Keep the plan active until runtime evidence is reviewed, then archive it as
  completed or deferred.
