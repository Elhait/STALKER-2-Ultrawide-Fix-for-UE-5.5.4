# ADS output+0x38 downstream-use audit — task plan

## Objective

Identify a concrete current Steam 2.0.4 local consumer of the validated
presentation output field `output+0x38`, with first-person/viewmodel presentation
semantics, without expanding into generic renderer analysis.

## Established evidence and current state

- Canonical Steam 2.0.4 identity is known: SHA-256
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`,
  `.text` size `130818560`, image base `0x140000000`.
- `FUN_140AF4022` copies the camera world FOV into output `+0x30`, selects a
  first-person/world value and copies it into output `+0x38`, then calls
  `FUN_140AF4FA4` at `RVA 0xAF42A7`.
- `FUN_140AF4FA4` was rejected as the causal owner for the captured weapon
  correction because its entry predicate stayed on the early path throughout
  the runtime ADS sequence.
- The downstream owner after `output+0x38` remains unresolved.

## Approved scope

1. Identity-gate the canonical 2.0.4 Ghidra program.
2. Inspect the local caller tail after `RVA 0xAF42A7`.
3. Record direct uses of the output object and `+0x38` in that bounded tail.
4. Rank only direct callees and obvious local consumers for first-person,
   viewmodel, camera or presentation semantics.
5. Produce one static report with concrete evidence or an explicit unresolved
   result.

## Explicit non-goals

- No generic renderer or all-executable `+0x38` scan.
- No new runtime tracer in this task.
- No camera, weapon, primitive, render-state or production ASI changes.
- No dialogue-zoom research in this task.

## Batches and validation

### Batch 1 — identity gate

Verify SHA-256, `.text` size, image base/section layout and current runtime
anchors. Stop on any mismatch.

### Batch 2 — bounded caller-tail audit

Inspect the instructions after `CALL 0xAF42A7` through the end of
`FUN_140AF4022`, recording output-object uses, `+0x38` accesses, calls and
obvious pointer forwarding.

### Batch 3 — direct-callee ranking

Inspect only direct callees discovered in Batch 2. Promote a candidate only if
it has concrete first-person/viewmodel/presentation ownership evidence.

### Batch 4 — final review

Run `git diff --check`, review changed paths and verify no runtime or production
files changed. The bounded theory was tested and stopped as `UNRESOLVED`; record
the factual result in the report, plan and TASKLOG.

### Result

The local tail contained no direct `RBX+0x38` use and no concrete first-person or
viewmodel presentation owner. The `RAX/RCX+0x38` accesses were not tied to the
validated output object. The approved stop condition is satisfied; no generic
renderer/data-flow expansion was performed.

## Risks and safe failure

High-fan-out or generic callees are not promoted. If the local chain ends without
a specific owner, record `UNRESOLVED` and stop. The audit is read-only and has no
runtime side effects.

## Stop condition

If direct or obviously local consumers of the validated output object/`+0x38`
do not produce a concrete current-2.0.4 first-person/viewmodel presentation
owner, close this audit as `UNRESOLVED` and do not expand into generic renderer
or whole-program data-flow analysis.

## Expected final Git review

Confirm only this plan, the bounded helper and the static report changed. No
production source, ASI or runtime tracer may be modified.
