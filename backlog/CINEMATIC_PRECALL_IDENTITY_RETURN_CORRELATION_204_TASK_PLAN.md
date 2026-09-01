# Cinematic Pre-call Identity → Return Correlation 2.0.4 — Task Plan

## Objective

Safely test the `RVA 0x26F7A25` timing boundary while preserving authoritative inner identity across the volatile Win64 call registers.

## Established evidence

- Manual F3/F4 produced correct cinematic framing with `+0x230=127.3927` and `+0x254=3.5555556`.
- Immediate post-setter writes were too early for effective FOV.
- `RVA 0x26F7A25` is the confirmed return site after the indirect ENTER setter, but post-call `RCX` cannot identify the item.
- Generic dispatch instrumentation is unsafe unless filtered to the validated ENTER setter target.

Offline SafetyHook audit result: `MidHook` exposes no callable original/trampoline method. Its internal `InlineHook` trampoline is a continuation that executes relocated bytes and jumps back to the original flow; the mid-stub restores the captured context and transfers to that continuation after the destination callback returns. It is not a normal callable function returning control to the callback. Therefore a single-hook `CALL RAX -> original call -> post-call callback logic` bridge cannot be implemented through the stock SafetyHook API without a custom assembly/control-flow bridge, manual stack/return handling or equivalent ABI emulation. Per the stop condition, this boundary remains untested and the bridge implementation path is closed as implementation-unsafe.

## Approved scope

- Hook pre-call `RVA 0x26F7A23` and filter `RAX == validated ENTER setter`.
- Capture `pendingInner=[RCX+0x18]` and the current thread before the native call.
- At return `RVA 0x26F7A25`, use only the captured same-thread `pendingInner`.
- Require readable state with aspect `1.7778`, flags `0x05`, and finite FOV in a safe range before any write.
- Write FOV `127.3927` and aspect `3.5555556` once; restore only FOV at validated EXIT.

## Explicit non-goals

- No post-call `RCX` reconstruction.
- No PAGE_GUARD, VEH, single-step, timers, polling, hotkeys or repeated writes.
- No writes to flags, no legacy offsets/signatures, and no changes to stable gameplay ASI.

## Batches

### Batch 1 — Correlated artifact

- Validate current ENTER signature, raw pre-call `FF D0`, raw return bytes `84 C0 75 EB`, and EXIT bytes.
- Build the read/write experimental artifact.

Batch 1 result: created `src/cinematic_precall_identity_return_correlation_204.cpp` and `build-artifacts/test-scripts/build-cinematic-precall-identity-return-correlation-204.cmd`. The artifact validates the current ENTER signature, raw pre-call `FF D0`, raw return bytes `84 C0 75 EB` and confirmed EXIT bytes. It captures `inner` before the call only when `RAX` equals the resolved ENTER setter, carries that identity through a thread-local pending record, and at return requires the same thread plus aspect `1.7778`, flags `0x05` and a finite FOV before writing. It ignores post-call `RCX`, performs one FOV/aspect pair and restores only FOV at EXIT. The build succeeded and produced `build-artifacts/test-asi/STALKER2CinematicPrecallIdentityReturnCorrelation204.asi`. Runtime evidence is pending.

Safety result: the first run logged installation and then crashed before producing any pre-call record. Offline audit found that the pre-call hook at `RVA 0x26F7A23` and return hook at `RVA 0x26F7A25` are only two bytes apart; independent inline SafetyHook patches necessarily overlap their instruction/trampoline regions. This artifact is rejected as unsafe and must not be rerun. The timing boundary remains untested; any future implementation must use one non-overlapping hook strategy or a validated custom control-flow bridge.

### Batch 2 — Isolated runtime test

- Enable only this artifact and run one natural cinematic transition.
- Inspect correlation gates, post-return state, visual framing and FOV-only EXIT restore.

### Batch 3 — Decision

- Promote only if the captured identity is valid, writes occur once on the authoritative target, framing matches manual F3/F4 and handoff is clean.
- Otherwise reject this implementation method without reopening the timing boundary or broadening the scan.

## Risks and safe failure

- The pre-call site is shared machinery; all writes are blocked unless setter target, same-thread identity and state gates pass.
- Pending state is thread-local and cleared after each return observation.
- Resolver or byte mismatch refuses installation with no writes.

## Final review

- Classify identity correlation, timing and visual result separately.
- Compare changed paths with this plan and perform read-only Git review.
