# Cinematic ENTER Aspect Write-Origin Recovery 2.0.4 — Task Plan

Status: Complete — the exact current-build ENTER aspect store is recovered from
the validated signature and existing runtime evidence; runtime RIP provenance
and an enclosing named caller remain unavailable. No new ASI is authorized by
this pass.

## Objective

Recover and classify the already observed native ENTER transition
`3.5555556 -> 1.7777778` for the verified 2.0.4 executable, without creating a
new runtime tracer or modifying game state.

## Established evidence and current state

- Executable: `Stalker2-Win64-Shipping.exe`, SHA-256
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Runtime captured one authoritative cinematic `inner` and the ENTER change
  from `3.5555556 / 0x05` to `1.7777778 / 0x05`.
- The prior page-guard trace reported `rva=0` for the ENTER write. That value
  is hook-displaced/trampoline context and is not an originating instruction.
- The validated signature statically resolves the store at RVA `0x6B7CB05`:
  `C7 80 54 02 00 00 39 8E E3 3F`, i.e. `MOV [inner+0x254], 1.7777778f`.
- The same local setter contract derives `inner` through `item+0x18` and
  `inner+0xF8`, sets bit `0x01` at `inner+0x259`, performs the aspect store,
  returns `1`, and has no live-FOV/XMM flow.
- No reliable Ghidra function boundary or direct code caller is available for
  this polymorphic/table-driven setter neighborhood.

## Approved scope

- Compare the existing runtime write-origin record with the validated static
  setter signature and current executable identity.
- Record the exact store instruction, operand semantics, local context and
  provenance limits.
- Classify whether an ordinary hook boundary is established by this evidence.
- Keep stable gameplay code, global experimental ASI, Ghidra project and
  release assets unchanged.

## Explicit non-goals

- No new ASI, runtime hook, write-test or game launch.
- No PAGE_GUARD, VEH, watchpoint, random sampling or broad aspect scan.
- No guessed caller reconstruction from `RSP`, displaced RIP or table
  proximity.
- No manual invocation of the setter or callback.
- No direct `inner+0x254` write by timing heuristic.

## Batches

### Batch 1 — Existing-evidence recovery

- Validate the executable identity and map the signature to the store.
- Separate recovered instruction identity from unavailable runtime RIP/caller
  identity.

Validation: existing reports, source signature/operand contract and read-only
static evidence.

Result: complete; exact store recovered at `RVA 0x6B7CB05`; enclosing named
function and runtime caller remain unresolved.

## Risks and safe failure

- The runtime `rva=0` must not be promoted as a code address.
- The setter has lifecycle side effects beyond the aspect store; instruction
  identification alone does not establish a safe intervention point.
- If executable identity or bytes differ, stop and refuse all promotion.

## Stop conditions and phase gates

- Stop this recovery pass after the exact store is mapped; do not reopen broad
  writer/caller searches.
- A runtime/write feasibility artifact requires a separate approved plan and a
  concrete ordinary hook contract that preserves the setter's original
  control flow.
- Until that gate passes, the ENTER aspect write is identified but not
  production-safe.

## Final review

- Compare changed paths with this plan.
- Record completed, remaining, deferred and not-runtime-validated outcomes.
- Leave the stable gameplay ASI and all release assets untouched.
