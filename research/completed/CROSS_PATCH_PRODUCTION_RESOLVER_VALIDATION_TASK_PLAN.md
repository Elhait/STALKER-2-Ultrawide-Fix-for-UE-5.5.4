# Cross-Patch Production Resolver Validation — Task Plan

Status: Completed — Gate A passes for Steam 2.0.2, 2.0.3 and 2.0.4

## Objective

Determine whether the unchanged `0.4.0` production resolver contracts would
resolve and safely install on the canonical Steam `2.0.2`, `2.0.3` and `2.0.4`
images. This validates portability of the current signatures and validation
logic; it does not generalize or modify any resolver.

## Established evidence and current state

- The current production package targets Steam `2.0.4` / UE 5.5.4.
- The canonical Ghidra project is `Dump/STALKER2-Ghidra` and contains the
  retained `2.0.2`, `2.0.3` and `2.0.4` entries.
- Current runtime identity for the control image is SHA-256
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`,
  `.text` size `130818560`, image base `0x140000000`.
- Known current runtime anchors include gameplay writer, cinematic aspect
  setter, cinematic ENTER/EXIT boundaries and ADS anchors. These are expected
  values for reconciliation, not automatic PASS values for another image.

## Approved scope

1. Establish a full identity header for each canonical image.
2. Apply the exact current production signatures and validation contracts to
   all three images without changing patterns or constants.
3. Check gameplay writer uniqueness, instruction decode and operand contract.
4. Check cinematic aspect setter uniqueness and its immediate/mode-state
   contract.
5. Check cinematic ENTER and EXIT signature uniqueness and instruction/call
   contracts.
6. Record per-image `RESOLVES`, `FAILS` or `FAIL CLOSED` results and an overall
   Gate A outcome.

## Explicit non-goals

- Do not alter production source, signatures, constants or the ASI.
- Do not create a generalized compatibility patch (Gate B is out of scope).
- Do not port fixed RVAs by arithmetic.
- Do not build, inject, launch the game or claim runtime compatibility.
- Do not create a new runtime tracer.
- Do not expand into weapon/viewmodel research.

## Files or areas expected to be touched

- This task plan.
- One report under `02-Research/Ghidra/reports`.
- Existing read-only Ghidra validation helpers only if a helper requires a
  provenance guard; no production files.
- One final `backlog/TASKLOG.md` entry after review.

## Batches and validation

### Batch 1 — Identity gate

Record program name, executable SHA-256, `.text` size and SHA-256, image base,
section layout and the exact production-anchor expectations. Any mismatch in
the identity tuple stops interpretation for that image.

Result: Identity `PASS` for all three target images. Their `.text` sizes and
hashes differ, confirming that the matrix was not a same-layout shortcut.

### Batch 2 — Gate A resolver simulation

Run the unchanged production signature/validation contracts against each image:
gameplay writer, cinematic aspect setter, ENTER and EXIT. Record match counts,
decoded instruction contracts, expected-byte checks and fail-closed behavior.

Result: Every production contract matched uniquely on all three images. The
gameplay match decoded to `MOVSS [RBX+0x30], XMM0` on each image. Aspect,
ENTER and EXIT contracts passed their expected local pattern checks.

### Batch 3 — Matrix and conclusion

Produce the `2.0.2 / 2.0.3 / 2.0.4` matrix, classify each hook and determine
`WOULD INSTALL`, `PARTIAL INSTALL` or `FAIL CLOSED` for Gate A. Do not infer
Gate B from a failure.

Result: `WOULD INSTALL` for 2.0.2, 2.0.3 and 2.0.4. The unchanged production
resolver is statically cross-patch portable across this tested set.

### Batch 4 — Final review and task log — completed

Reviewed report provenance, changed paths and limits. Added one factual
task-log entry after the matrix and Git review were complete.

## Risks and safe-failure behavior

- A helper pattern mismatch is recorded as a result; it is not repaired during
  this task.
- An anchor mismatch or incomplete identity is `STOP/UNKNOWN` for that image.
- A static match without decode and semantic validation is not a resolver PASS.
- No production behavior changes because source/build/runtime are excluded.
- Agent-owned headless Ghidra sessions must be verified exited and canonical
  project locks verified released after every run.

## Stop conditions

- Stop interpretation for an image if SHA, `.text` size/SHA, image base or
  section layout is inconsistent or cannot be established.
- Stop promotion if any current production contract cannot be simulated
  without changing the current pattern or validation logic.
- Stop after the matrix, report and one task-log entry; Gate B or source work
  requires a separate approved task.

## Expected final Git review

Confirm only this plan, the cross-patch report, the read-only helper and one
factual task-log entry were changed by this task. Confirm no production source,
ASI, signature or historical research conclusion was silently rewritten.
