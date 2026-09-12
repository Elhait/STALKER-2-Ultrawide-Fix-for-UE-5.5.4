# POST_EXIT_INTERPOLATION_OWNER_AUDIT

## Objective

Identify the native state/function that produces the post-EXIT per-frame FOV
interpolation observed in Steam 2.0.5, without changing the production ASI.

## Established evidence

- Current 2.0.5 identity passed static validation.
- Final EXIT produces `nativeTargetFov=90`, while the writer immediately receives
  `126.87` and then a smooth stream to approximately `90` over ~1 second.
- Writer `XMM0` and `CameraComponent +0x230` match throughout the stream.
- The stream is visible before `CinematicExiting` recovery completes.
- `FUN_14366F9AA` is the current EXIT handoff boundary. Its downstream branch
  invokes virtual slots `+0x880/+0x868` or `+0x878/+0x860`, then
  `FUN_1426E0E5E` and finalization logic.

## Approved scope

- Read-only Ghidra audit for the current 2.0.5 program only.
- Inspect the four virtual-slot callsites and directly called helper functions.
- Record concrete targets where recoverable, argument flow, state fields,
  interpolation/lerp helpers, and direct callers.
- Preserve identity header and stop at the first downstream layer.

## Explicit non-goals

- No runtime hooks, calls, writes or production source changes.
- No broad vtable/rdata scan.
- No direct-caller expansion above `FUN_14366F9AA`.
- No renderer-wide search or FOV formula changes.

## Batches and validation

1. Run one headless read-only script against the 2.0.5 program after identity
   gating.
2. Classify each downstream slot/helper as transition-related, camera/view
   related, auxiliary, or unresolved only when directly supported.
3. If no transition state is exposed, stop and use the already validated
   runtime writer stream as the anchor for a separate producer trace.

## Risks and safe failure

- Static conclusions are version-specific and remain research evidence.
- A missing or ambiguous target is recorded as unresolved; no guessed virtual
  invocation is permitted.
- Headless Ghidra PID and project locks must be tracked and released.

## Stop conditions and final review

- Stop after this downstream layer. Do not expand automatically.
- Perform read-only Git path/diff review after the script/report batch.
