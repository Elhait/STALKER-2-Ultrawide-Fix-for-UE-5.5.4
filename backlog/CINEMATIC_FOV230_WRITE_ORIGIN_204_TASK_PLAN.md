# Cinematic +0x230 Write-Origin 2.0.4 — Task Plan

## Objective

Identify, using bounded current-build static evidence, ordinary-hook candidates that can write the confirmed cinematic camera field `inner+0x230`, with special attention to the observed native transition `90.65574 → 90.0` after ENTER.

## Established evidence and current state

- 2.0.4 cinematic ENTER and native EXIT anchors are runtime-confirmed.
- The same authoritative `inner` object was observed before and after cinematic initialization.
- Between ENTER and steady cinematic state, `inner+0x230` changed from `90.65574` to `90.0`, while `inner+0x254` changed from `3.5555556` to `1.7777778` and flags remained `0x05`.
- A later manual write of `inner+0x230=127.3927` together with `inner+0x254=3.5555556` produced correct cinematic framing and persisted without repeated writes.
- Immediate post-setter FOV writes were visually ineffective.
- PAGE_GUARD/VEH, random XMM sampling and the adjacent SafetyHook bridge are closed as unsafe or insufficient.

## Approved scope

- Perform a bounded static enumeration on the existing 2.0.4 executable/program.
- Find only instructions that encode a memory store using displacement `+0x230`; do not search for FOV values or all scalar-FP operations.
- Record instruction RVA, containing function, decoded base register, store width/source operand and nearby control-flow/call context.
- Reject obvious non-camera structures and retain at most three plausible candidates.
- Prepare a separate ordinary-hook runtime tracer only after a static candidate is established.

## Explicit non-goals

- No full Ghidra re-analysis of 2.0.3 or 2.0.4.
- No global FOV-like float search, watchpoint, PAGE_GUARD, VEH or debugger-style sampling.
- No runtime writes, automatic correction, manual probe integration or stable gameplay changes in this phase.
- No legacy RVA/offset promotion without current-build evidence.
- No caller reconstruction through speculative stack inspection.

## Expected files or areas

- `02-Research/Ghidra/ghidra-scripts` bounded enumeration helper, if needed.
- This plan file.
- A future experimental source/build script only if a static candidate passes the gate.

## Batches

### Batch 1 — Static +0x230 store enumeration

- Inspect only the current 2.0.4 program/executable and enumerate stores with displacement `0x230`.
- Validation: current executable identity, decoded instruction semantics, containing function and bounded surrounding context.
- Result: current 2.0.4 hash matched. The existing analysed program was opened read-only; no full analysis was run. The enumeration found 16 scalar/object stores among the broader displacement matches.

### Batch 2 — Candidate classification

- Keep no more than three candidates with plausible camera-like ownership and an ordinary hook surface.
- Validation: reject generic/unrelated structures and candidates requiring unsafe control flow.
- Result: retained three bounded candidates for optional read-only runtime confirmation:
  - RVA `0x18826D`, `MOVSS [RBX+0x230],XMM4`; adjacent stores write `+0x22C` and `+0x234`, suggesting a compact scalar camera-like record.
  - RVA `0x63C452`, `MOVSS [RCX+0x230],XMM1`; adjacent `+0x22C` store and `+0x268` byte state provide a bounded state-update context.
  - RVA `0x10FFD01`, `MOVSS [RBX+0x230],XMM0`; immediately preceded by `MULSS XMM0,[RBX+0x230]`, giving the strongest local transform evidence.
- Rejected for this batch: stack-local `RSP/RBP` scratch stores, pointer/integer stores, wide `MOVAPS/MOVUPS/MOVSD` copies without a scalar camera context, and unrelated zero/constant initialization stores.

### Batch 3 — Optional read-only runtime confirmation

- Create and run a read-only ordinary hook only for an approved candidate.
- Require same captured cinematic `inner` and the first transition `90.65574 → 90.0`.
- No candidate write-test is allowed from discovery evidence alone.
- Artifact prepared and built: `src/cinematic_fov230_write_origin_trace_204.cpp`, build script `build-artifacts/test-scripts/build-cinematic-fov230-write-origin-trace-204.cmd`, output `build-artifacts/test-asi/STALKER2CinematicFov230WriteOriginTrace204.asi`.
- The artifact uses ordinary SafetyHook mid-hooks at the three exact current-build scalar stores and validates their bytes before installation. It captures the authoritative `inner` at ENTER, correlates only same-inner writer hits, reads the pre-store FOV/aspect/flags and incoming XMM scalar, and clears correlation at EXIT. It performs no game-state writes and uses no VEH/PAGE_GUARD, timer or polling.
- Validation status: build passed; runtime validation is pending.
- Runtime result: the tracer installed, captured `inner=0x17EFAAAC040` at ENTER with `FOV=90.65574`, `aspect=3.5555556`, `flags=0x05`, and reached EXIT on the same inner, but recorded zero hits for all three candidate writers. None of the promoted sites wrote the observed authoritative inner during this run.
- Batch 3 result: the three candidates are rejected as the observed `+0x230` write origin. The bounded `+0x230` write-origin branch is closed without broadening the search; this does not prove that no other current-build writer exists, only that the enumerated/promoted candidates were not runtime-causal for this cinematic run.

## Risks and safe-failure behavior

- Static displacement matches may belong to unrelated structures; numeric offset alone is not ownership proof.
- Any ambiguous executable identity, unresolved base/object provenance or unsafe hook surface rejects the candidate.
- Stop before runtime artifact creation if no candidate can be bounded safely.

## Stop conditions and phase gates

- Stop after the bounded `+0x230` enumeration if no plausible current-build store is found.
- Stop after at most three rejected candidates; do not broaden to FOV/scalar/projection searches.
- A candidate is not runtime-promoted without exact same-inner correlation and the observed old→new transition.
- This phase does not authorize a write-test or production integration.

## Final review requirements

- Perform read-only Git status/diff review after any helper or plan change.
- Compare actual paths with this plan and classify completed, remaining, deferred, blocked and not-runtime-validated items.
- Keep `TASKLOG.md` update deferred until the bounded result is reviewed.
