# Cinematic ENTER Aspect Store Provenance 2.0.4 — Task Plan

Status: Complete — runtime provenance gate passed for the exact validated ENTER
aspect store. No game-state writes were performed.

## Objective

Runtime-correlate execution of current-build `RVA 0x6B7CB05` with the validated
cinematic authoritative object and pre-store state.

## Established evidence and current state

- Executable identity: 2.0.4, SHA-256
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Store bytes: `C7 80 54 02 00 00 39 8E E3 3F`.
- Semantics: `MOV [inner+0x254], 1.7777778f`.
- The exact store follows the native `+0x259` mode-bit mutation.
- Earlier PAGE_GUARD evidence observed the authoritative transition but its
  reported `rva=0` was hook-displaced and is not a native RIP.

## Approved scope

- One ordinary `SafetyHookMid` at `RVA 0x6B7CB05`.
- Validate executable memory and exact instruction bytes before installation.
- Log `context.rip`, `RAX`, pre-store `RAX+0x254`, `RAX+0x259`, thread and
  timestamp/sequence.
- Classify a hit only when the exact store context has the expected cinematic
  precondition: aspect approximately `3.5555556` and flags `0x05`.
- Read-only guarded memory inspection; no writes, no second hook and no caller
  reconstruction.

## Explicit non-goals

- No immediate patch, aspect write, FOV transform or production integration.
- No PAGE_GUARD, VEH, polling, timers, hotkeys or game calls.
- Do not modify stable gameplay/global ASI, Ghidra data or release assets.

## Batches and validation

### Batch 1 — Artifact and build

Create the isolated source/build script and compile the ASI. Inspect the
artifact source for one hook and zero game-state writes.

Result: source and build script created; compilation succeeded and produced
`build-artifacts/test-asi/STALKER2CinematicEnterAspectStoreProvenance204.asi`.
Static inspection confirms one hook at `RVA 0x6B7CB05`, exact-byte validation,
guarded reads only, and no writes or secondary hooks.

### Batch 2 — Runtime provenance gate

Run only this ASI through one natural cinematic ENTER. PASS requires a hit at
the exact store with `RAX` satisfying the authoritative inner-state contract,
pre-store aspect `3.5555556`, flags `0x05`, and valid module-relative RIP.

Result: the artifact installed and captured one hit with `storeRva=0x6B7CB05`,
`RAX=0x22291C00100`, pre-store aspect `3.5555556`, flags `0x05`, readable state
and `expectedState=true`. This confirms execution of the exact validated store
on the authoritative cinematic object for the tested lifecycle. `ripRva=0x0`
remains a SafetyHook context limitation already seen in the older displaced
trace; it does not override the exact-byte-validated hook address. The scene
rendered in native 16:9, as expected for a read-only provenance artifact that
did not replace the store immediate.

## Risks and safe failure

- Any executable byte mismatch refuses installation without patching.
- Ambiguous/unreadable `RAX` state logs no write and does not promote the hit.
- The artifact must remain diagnostic-only even if the gate passes.

## Stop conditions

- Stop after one clean provenance run.
- If the hook is unsafe or the exact RIP is displaced, record that result and
  do not add adjacent hooks.
- Immediate-operand patch feasibility requires a separate implementation plan.

## Final review

Compare changed paths with this plan, record runtime limits and perform a
read-only Git review before any later implementation phase.
