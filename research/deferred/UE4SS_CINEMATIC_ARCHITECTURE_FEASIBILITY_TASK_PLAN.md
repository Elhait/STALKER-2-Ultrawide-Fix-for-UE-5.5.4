# UE4SS_CINEMATIC_ARCHITECTURE_FEASIBILITY

## Objective

Assess whether the installed UE4SS environment provides a viable architecture
for a future cinematic subsystem without relying on undocumented complex
ref/out invocation or replacing the stable gameplay ASI.

## Established evidence and current state

- UE4SS safely observes live `CameraComponent`, `Stalker2.CameraManager` and
  `PlayerCameraManager` state transitions.
- Reflected metadata exposes relevant camera candidates, but safe Lua invocation
  of `GetCameraView` and `BlueprintUpdateCamera` is blocked by unresolved
  complex ref/out ABI rules.
- `CallFunctionsByHandle` is available only through an opaque wrapper with no
  source-backed packing/lifetime/readback contract.
- ASI cinematic research is blocked at the unresolved `vtable +0x638` gate.
- The production Camera/FOV ASI remains stable and must not be modified.

## Approved scope

- Read-only inventory of UE4SS capabilities relevant to cinematic control.
- Safe reflected property read/write coverage using documented or already
  proven helpers.
- Reliable lifecycle observation and callback/timer boundaries.
- Value-only callable functions only where their arguments and side effects are
  understood.
- Determine whether UE4SS can trigger native reevaluation or only mutate stored
  camera fields.

## Explicit non-goals

- No replacement module implementation.
- No runtime camera writes or suppression tests.
- No undocumented `CallFunctionsByHandle` invocation.
- No production ASI changes, packaging or migration.
- No broad renderer, projection or executable scan.

## Expected files or areas

- Installed UE4SS modules and their local scripts/readme files.
- `research/ue4ss/` and existing evidence reports for reference only.
- One final audit report under `research/reports/` if the capability inventory
  reaches a stable disposition.

## Batches

### Batch 1 — capability matrix

Record available safe read, write, observation, callback and value-only call
mechanisms. Mark each as confirmed, undocumented or unavailable.

### Batch 2 — architecture decision gate

Compare the confirmed capability matrix with the minimum requirements for a
cinematic solution: lifecycle detection, deterministic state restoration,
native reevaluation access and safe failure.

### Batch 3 — disposition

Classify UE4SS cinematic architecture as feasible, conditionally feasible or
blocked. Do not implement unless a separate implementation plan is approved.

## Validation

- Evidence must come from installed scripts, documented APIs or prior runtime
  logs; no assumptions from function names alone.
- Separate observation capability from mutation capability and both from native
  reevaluation capability.
- Preserve the distinction between metadata presence and runtime behavior.

## Risks and safe failure

- A convenient property write may not rebuild downstream camera/projection
  state.
- An undocumented helper may hide unsafe parameter/lifetime behavior.
- If minimum capabilities cannot be proven, classify the architecture as
  blocked rather than creating a guessed prototype.

## Stop conditions

- Stop if the audit requires undocumented ref/out calls or broad reverse
  engineering of UE4SS internals.
- Stop before any runtime write or replacement implementation.
- A positive result must identify a concrete safe capability set, not merely
  reflected metadata.

## Disposition

Batch 1 capability matrix, Batch 2 architecture decision gate and Batch 3
disposition are complete. UE4SS is feasible as an observer/state-access
platform, but the required native-like cinematic reevaluation and downstream
projection rebuild remain blocked or unproven. Production migration is
deferred until a genuinely new evidence class appears.

## Disposition

Batch 1 capability matrix, Batch 2 architecture decision gate and Batch 3
disposition are complete. UE4SS is feasible as an observer/state-access
platform, but the required native-like cinematic reevaluation and downstream
projection rebuild remain blocked or unproven. Production migration is
deferred until a genuinely new evidence class appears.

## Final review

Compare the audit report and changed paths with this plan. Preserve unrelated
working-tree changes and report completed, remaining, deferred, blocked and
not-runtime-validated items.
