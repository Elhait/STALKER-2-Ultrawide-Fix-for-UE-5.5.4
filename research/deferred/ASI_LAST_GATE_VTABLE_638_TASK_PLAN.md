# ASI_LAST_GATE_VTABLE_638

## Objective

Classify the virtual call immediately after the confirmed current-build
camera writer and determine whether `[RDI.vtable + 0x638]` has camera/view or
projection reevaluation semantics.

## Established evidence and current state

- Current runtime evidence confirms `FUN_140AF4022` as the camera-writer
  boundary for the native gameplay A→B→C transition.
- Static evidence identifies `FUN_1453BA300` as the coordinator calling the
  writer at the current-build callsite.
- The coordinator then performs a virtual call through `[RDI.vtable + 0x638]`.
- The owner/type of that virtual slot and its semantics are not established.
- An existing bounded Ghidra report in `02-Research/reports/` already covers
  this gate for the canonical Steam 2.0.4 image.

## Approved scope

- Verify current executable identity before static analysis.
- Inspect only the callsite and virtual slot `[RDI.vtable + 0x638]`.
- Recover concrete target(s), strongest owner/type evidence, argument roles,
  and whether the writer-produced output is consumed.
- Classify direct camera/view/projection reevaluation semantics if supported.

## Explicit non-goals

- No runtime invocation, writes or new hooks.
- No production ASI or stable gameplay source changes.
- No broad xref sweep, neighboring-slot exploration or settings-system scan.
- No semantic naming based only on nearby function names.

## Files or areas expected to be touched

- `ASI_LAST_GATE_VTABLE_638_TASK_PLAN.md` during execution.
- One static-analysis report under `research/reports/` if identity passes.
- Ghidra project under `Dump/STALKER2-Ghidra` may be read, never treated as
  disposable output.

## Implementation batches

### Batch 1 — executable identity gate

Record SHA-256, `.text` size, image base and section layout for the current
game executable. Confirm one or two known runtime anchors before inspecting the
slot. Any identity mismatch stops the pass.

Existing evidence records this gate as `Identity: PASS` for SHA-256
`2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`, image
base `0x140000000`, and the recorded `.text` layout.

### Batch 2 — bounded slot classification

Inspect the single virtual callsite after `FUN_140AF4022`, recover target/type
evidence and argument flow, and stop at this boundary.

### Batch 3 — evidence report and disposition

Write a concise report separating confirmed facts, supported inference,
unresolved ownership and the ASI gate disposition.

## Validation

- Identity values must be recorded before any Ghidra conclusion.
- Confirm the known writer/coordinator anchors resolve in the same image.
- Verify the virtual call instruction and target table location.
- Review the final report against this plan and perform read-only Git review.

## Risks and rollback / safe failure

- A stale or mismatched Ghidra image can produce invalid RVAs and false owner
  conclusions.
- A virtual slot may be a function table without recoverable type metadata.
- On mismatch, ambiguity or timeout, stop and preserve the evidence as
  unresolved; do not alter the project or source.

## Stop conditions and phase gates

- Any SHA-256, `.text` size, image-base or section-layout mismatch: STOP.
- Known runtime anchor mismatch after identity match: STOP and reconcile.
- If the slot does not yield clear camera/view reevaluation semantics in this
  pass: classify the ASI cinematic route as `CLOSED / BLOCKED`.
- Do not inspect neighboring slots or climb another caller level in this task.

## Disposition

The existing bounded pass completed with a partial result: the virtual call
preserves the source/output pair, but no trusted concrete target or owner/type
association was recovered. The ASI cinematic route remains blocked at this
gate; no runtime feasibility test is authorized from this evidence.

## Expected final Git review

Compare changed paths with this plan, preserve unrelated user changes, and
report completed, remaining, deferred, blocked and not-runtime-validated items.
Move this plan to `research/completed` or `research/deferred` after final
disposition; do not leave it in the repository root.
