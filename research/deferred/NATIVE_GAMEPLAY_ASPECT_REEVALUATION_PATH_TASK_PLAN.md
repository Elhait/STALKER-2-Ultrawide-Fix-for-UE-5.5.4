# Native Gameplay Aspect Reevaluation Path — Task Plan

Status: `BLOCKED / DEFERRED`  
Closure reason: the bounded native/Ghidra passes established the writer and
coordinator boundaries, but no recoverable owner/type association or
settings-side origin was found.

## Objective

Identify the concrete native operation, event boundary or callable path that
the game's settings-driven aspect transition uses to reevaluate camera/view
state. The target is the causal path from the observed gameplay transition to
the final correct state, not a replacement property value.

## Established evidence and current state

- Read-only UE4SS observation confirmed the same live `CameraComponent`,
  `Stalker2.CameraManager` references and `PlayerCameraManager` through one
  native gameplay transition.
- The tested sequence is:

  ```text
  A — 32:9 before:
      FOV=90, AspectRatio=3.555556, bConstrainAspectRatio=false
  B — 16:9 constrained:
      FOV=90, AspectRatio=1.777778, bConstrainAspectRatio=true
  C — return to 32:9 after reevaluation:
      FOV=90, AspectRatio=1.777778, bConstrainAspectRatio=false
  ```

- `AspectRatioAxisConstraint` remained `MaintainXFOV` and
  `bOverrideAspectRatioAxisConstraint` remained `false`.
- A and C share FOV=90 and `bConstrainAspectRatio=false` but have different
  visual results; a storage-only write is not considered equivalent.
- The production CameraTweaks cinematic branch is a proven workaround and is
  not the source of truth for this native path.

## Approved scope

- Read-only native tracing around the actual manual settings transition.
- Use the known camera ownership chain as the correlation anchor.
- Identify a concrete native writer/caller or reproducible owner/event
  correlation tied to A→B→C.
- Preserve timestamps, object identity, before/after values and runtime
  conditions in the evidence record.

## Explicit non-goals

- No property writes or Live View edits.
- No new FOV mathematics or guessed aspect/constraint values.
- No cinematic changes and no modification of production CameraTweaks.
- No broad renderer, matrix or projection scans.
- No new camera modifier investigation.
- No claim that final reflected storage state alone explains framing.

## Files or areas expected to be touched

- This task plan.
- Research-only native diagnostic source/build output if required.
- Durable research evidence after a reproducible result.
- No production source, release files or runtime installation files.

## Batches

### Batch 1 — transition-boundary instrumentation

Review the existing AutoDump output and preserve A/B/C markers, object identity
and state transitions without writes.

Result: PASS. A/B/C and same-object ownership were reproduced.

### Batch 2 — UE4SS callable boundary

Test only exact current-build UFunction names on the known camera chain.

Result: UFunction metadata and hook installation PASS, but no callbacks were
observed for `SetAspectRatio`, `SetConstraintAspectRatio` or `GetCameraView`.
This is a bounded negative for the UE4SS RegisterHook dispatch layer, not for
underlying native C++ participation.

### Batch 3 — native write/call provenance

Use the validated current-build camera-writer trace as a diagnostic-only native
boundary. Correlate writer entry/exit with A→B→C, source/output identity and
the observed camera state fields. Do not add property writes, broad hooks or
speculative addresses.

Result: writer entry was confirmed during A→B and B→C with the same source and
output identity. The settings-side caller remains unresolved.

### Batch 3.1 — Ghidra direct-caller provenance

Identity-gated read-only audit of the validated writer found one code caller,
`FUN_1453BA300` at callsite RVA `0x53BA322`. Its register flow establishes the
same source/output pair before the writer and the virtual `+0x638` dispatch
afterward. Data references were excluded. The settings-side caller remains
unresolved; see the durable evidence note in `research/evidence/`.

### Batch 3.3 — data-reference classification

The two references were classified in a read-only identity-gated pass:
`0x14B46D748` is `.pdata` unwind metadata for `FUN_1453BA300`; `0x1484A6728`
is in `.rdata` and belongs to a dense function-pointer sequence containing
the coordinator between neighboring functions. Indirect/function-table
reachability is strongly supported, but vtable identity and owner/type remain
unresolved. No broad `.rdata` scan is authorized.

### Batch 3.2 — coordinator caller audit

The one-level read-only audit above `FUN_1453BA300` found no direct code
callers, only two data references at `0x14B46D748` and `0x1484A6728`. This
prevents promotion of a direct upstream callsite and makes indirect/vtable
reachability the only bounded follow-up hypothesis. See the durable evidence
note in `research/evidence/`.

### Batch 4 — decision

Promote the path only if settings change, native reevaluation and C-state are
causally correlated through a concrete owner/caller. Otherwise preserve A/B/C
and the writer boundary as positive evidence and close the upstream provenance
branch as `BLOCKED / DEFERRED`.

## Validation

- Static review of diagnostic-only changes.
- User-run A→B→C settings sequence with the native trace.
- Correlate native trace timestamps with UE4SS snapshots.
- Confirm no property writes and no production ASI changes.
- Do not claim caller discovery without reproducible caller/owner evidence.

## Risks and safe failure

- A native writer trace may not expose its upstream caller.
- Short-lived writes may be sampled incompletely.
- Diagnostic failure must leave production CameraTweaks untouched.
- Rollback is removal of research-only diagnostic changes.

## Stop conditions

- Stop if progress requires blind writes, guessed pointers, broad renderer
  scanning or unbounded native hook experimentation.
- Stop and classify upstream provenance as `BLOCKED / DEFERRED` if no concrete
  caller/owner can be tied to A→B→C within the bounded pass.

## Expected final review

- Separate confirmed native transition/writer evidence from unresolved caller
  hypotheses.
- Record changed research paths and confirm production files were untouched.
- Perform a read-only path/diff review; do not run Git state-changing commands.
