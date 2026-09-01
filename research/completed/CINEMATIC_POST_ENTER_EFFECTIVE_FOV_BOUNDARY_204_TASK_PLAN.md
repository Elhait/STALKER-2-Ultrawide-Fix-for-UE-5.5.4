# Cinematic Post-ENTER Effective FOV Boundary 2.0.4 — Task Plan

## Objective

Test the first deterministic runtime boundary after the validated cinematic ENTER setter where the combined `+0x230` FOV and `+0x254` aspect state may become visually effective.

## Established evidence and scope

- Manual `F3/F4` confirmed correct cinematic framing with `FOV=127.3927` and `aspect=3.5555556`.
- Immediate post-setter application wrote both values but did not change cinematic FOV; aspect did change.
- The runtime-confirmed caller return site is `RVA 0x26F7A25`, immediately after `CALL RAX @ 0x26F7A23` and the ENTER setter.
- This batch tests only that one candidate; no broad update-function search is allowed.

## Approved scope

- Validate raw bytes around `RVA 0x26F7A25` (`84 C0 75 EB` in the current executable).
- Hook only this post-call return boundary and the confirmed EXIT callback.
- Capture baseline FOV dynamically, then perform one FOV write and one aspect write on the same resolved inner state.
- Restore only FOV at EXIT; native callback owns aspect/mode cleanup.

## Explicit non-goals

- No generic dispatch hook, VEH, PAGE_GUARD, timers, polling, hotkeys or repeated writes.
- No legacy offsets/signatures beyond the current validated state contract.
- No changes to stable gameplay ASI or production source.

## Batches and validation

### Batch 1 — Candidate artifact

- Build and verify the exact return-site bytes and hook installation.
- Build success is not runtime or visual proof.

Batch 1 result: created `src/cinematic_post_enter_effective_fov_boundary_204.cpp` and `build-artifacts/test-scripts/build-cinematic-post-enter-effective-fov-boundary-204.cmd`. The artifact validates raw bytes `84 C0 75 EB` at `RVA 0x26F7A25`, hooks only this post-call boundary and the confirmed EXIT callback, captures baseline FOV, writes the combined `+0x230/+0x254` state once, and restores only FOV at EXIT. The build succeeded and produced `build-artifacts/test-asi/STALKER2CinematicPostEnterEffectiveFovBoundary204.asi`. No runtime evidence is established yet.

Batch 2 runtime result: the return-site hook produced one invalid pointer-like target with unreadable state and a second target whose state was `flags=0x00`, not the validated cinematic `0x05` state. Although the second record accepted writes, the hook incorrectly assumed `RCX` still identified the dispatcher item after the called setter returned; `RCX` is volatile and was clobbered. The artifact therefore cannot safely identify the authoritative inner state at `RVA 0x26F7A25`. The reported 16:9 visual result is not a valid FOV causal result. This candidate boundary is rejected as unsafe; do not rerun or use its writes. Any future boundary must preserve item identity before the call and correlate it without trusting post-call `RCX`.

### Batch 2 — Isolated runtime test

- Enable only this artifact; disable gameplay and all diagnostic ASIs.
- Run one natural cinematic transition and inspect post-return state, framing and EXIT handoff.

### Batch 3 — Decision

- Promote only if one write pair at this boundary matches manual framing and the handoff remains clean.
- Otherwise reject this boundary and stop; do not broaden the search in this batch.

## Risks and safe failure

- If return-site bytes or object identity do not validate, install no hooks and perform no writes.
- Writes are limited to the approved FOV/aspect fields; flags remain untouched.

## Final review

- Classify the candidate as confirmed, inconclusive or rejected and compare changed paths with this plan.
