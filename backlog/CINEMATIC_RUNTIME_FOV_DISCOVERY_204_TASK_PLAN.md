# Cinematic Runtime FOV Discovery 2.0.4 — Task Plan

## Objective

Perform a read-only runtime discovery pass on executable 2.0.4 to identify live FOV/XMM/scalar/call-path candidates that execute during the validated cinematic ENTER → steady cinematic → native EXIT lifecycle. This phase generates candidates only; it does not modify camera state.

## Established evidence and current state

- Executable: S.T.A.L.K.E.R. 2 version 2.0.4.
- Known executable SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- The validated ENTER setter is an aspect/mode-only path: `item+0x18 → inner+0xF8 → +0x259 |= 1 → +0x254 = 16:9`.
- The validated native EXIT callback is `FUN_146B6C482`; it restores native aspect/mode and provides the deterministic lifecycle boundary.
- The prior `+0x230` experiment proved one-shot write/restore mechanics but did not produce correct cinematic FOV framing.
- The historical visually effective FOV mechanism was found through runtime logging of a live register/data-flow, not through the current ENTER setter.

## Approved scope

- Phase A only: read-only runtime discovery.
- Bound tracing to the validated cinematic lifecycle window.
- Capture executable instruction/RVA, thread, register/scalar values and relevant call/store context for FOV-like data-flow candidates.
- Use the resulting log to select at most one candidate for a separate Phase B causal write test.
- The closed `vtable+0x60` aspect-setter boundary is not reopened. Discovery may cover a bounded execution interval around ENTER, including activity immediately before and after that setter, to locate a parallel cinematic/camera path.

## Explicit non-goals

- No camera/FOV/aspect/mode writes.
- No write-test or visual intervention in this phase.
- No legacy RVA, `state+0x54`, `+0x230`, or approximate-value matching as acceptance criteria.
- No broad scan of all floats, all vtables, renderer traffic or generic callbacks.
- No changes to `STALKER2GameplayAspectFix.asi`, stable gameplay source, production source or release artifacts.
- No manual callback invocation, timers, polling, frame delays or game-owned function calls from the tracer.

## Expected files or areas

- Research tracer source under `src/` or the approved diagnostic source area.
- A bounded build script under `build-artifacts/test-scripts/` if a new artifact is required.
- Runtime log produced by the user during the isolated test.
- This task plan; no production source or release directory.

## Batches and validation

### Batch 1 — Read-only runtime discovery tracer

- Anchor the trace to the validated 2.0.4 cinematic ENTER and native EXIT lifecycle.
- Capture only bounded execution evidence: thread, RIP/RVA, XMM/scalar values, relevant argument registers, and candidate call/store transitions.
- Fail closed on unresolved addresses, unreadable memory or ambiguous state.
- Build validation proves only that the diagnostic artifact compiles and installs safely; it is not runtime proof.

Batch 1 implementation result: created `src/cinematic_runtime_fov_discovery_204.cpp` and `build-artifacts/test-scripts/build-cinematic-runtime-fov-discovery-204.cmd`. The tracer validates the current 2.0.4 ENTER signature and `FUN_146B6C482` prologue, hooks only those two lifecycle points, records caller return RVA, `RCX/RDX/R8/R9`, the first four XMM scalar lanes and guarded `item+0x18 -> inner` state snapshots, and performs no game-state writes or game-function calls. The build succeeded and produced `build-artifacts/test-asi/STALKER2CinematicRuntimeFovDiscovery204.asi`. This is compile/build evidence only; runtime installation and candidate discovery remain pending.

Phase A runtime result for executable 2.0.4: the tracer installed and captured a cinematic ENTER on thread `19776` with `callerRva=0x26F7A25`, `RCX` as the item, `item+0x18` resolving to the shared context, and the inner state readable as `aspect=3.5555556`, `flags=0x04`, `fov230=90.65574` before the native ENTER setter executes. The ENTER-boundary XMM lanes were `0`, `1`, `9.3975676e+26`, `1`; no plausible live FOV value or FOV transform was present at this boundary. This independently revalidates the same upper caller anchor previously associated with the cinematic polymorphic branch, but does not establish the FOV data-flow location. The native EXIT callback fired twice on the same thread with stable `callerRva=0x67D4A97` and the expected scalar arguments (`RDX=0x643`, `R8=0x1BBDC`, `R9=0x10`). The EXIT object snapshot is diagnostically invalid because the tracer passed the already-resolved inner pointer into a logger that expects an item/context pointer and then reads `+0xF8` again; the caller/argument evidence remains valid, but the reported EXIT `inner`, aspect, flags and `fov230` fields are not evidence. Result: runtime discovery found no FOV candidate yet, while confirming the ENTER owner anchor and the two-hit EXIT lifecycle. A bounded tracer correction is required before relying on EXIT state snapshots; no write-test is authorized.

Phase A deeper-boundary tracer revision: the read-only tracer now additionally hooks the validated indirect `CALL RAX` at `VA 0x1426F7A23` (`RVA 0x26F7A23`, bytes `FF D0`) and records the actual target in `RAX`, item/vtable identity, matching vtable slot within a bounded `0x100`-byte window, XMM0–XMM7 and the resolved inner state. The EXIT logger now consumes the already-resolved inner pointer directly, avoiding the previous double-`+0xF8` interpretation. The build succeeded and regenerated `build-artifacts/test-asi/STALKER2CinematicRuntimeFovDiscovery204.asi`. This remains Phase A discovery only: no game-state writes, calls, timers, polling or write-test behavior were added.

Phase A filter repair: the first deeper-boundary runtime build was rejected for analysis because its unrestricted `CALL RAX` hook exhausted the 256-record limit on generic traffic before cinematic ENTER. The tracer is now lifecycle-filtered: it logs the indirect call only when `RAX` equals the validated ENTER target, then arms at most eight subsequent dispatch-call records on the same thread and disarms at native EXIT. The call-site return value is not treated as a caller RVA because the hook runs before the CALL pushes its return address. The rebuild succeeded; no runtime evidence from the flooded run is promoted and no game-state writes were introduced.

Validation correction: the runtime log at `21:54–21:55` was produced by the prior artifact, before the filter-repair rebuild. It therefore repeats the generic post-ENTER traffic and cannot validate the repaired filter. The corrected artifact was rebuilt successfully at `21:57:52`; the next isolated run is the first valid runtime test of the context-filtered tracer.

Valid Phase A capture after the corrected rebuild: the tracer installed at `22:00:02` and captured one validated ENTER dispatch at `22:01:33` with vtable slot `+0x60` targeting the current 2.0.4 ENTER setter. The same boundary showed `aspect=3.5555556`, `flags=0x04`, `fov230=90.65574`, while XMM0–XMM7 contained no plausible live FOV or transform. The repaired context/thread filter suppressed the previous generic post-ENTER records; only the two expected native EXIT callback invocations remained at `22:01:45`, both on the same thread with `callerRva=0x67D4A97` and readable shared inner state. Result: the tracer correction is validated, but this dispatch/ENTER boundary still yields no FOV candidate; no candidate is promoted and no write-test is authorized.

Batch 2A artifact: created `src/cinematic_runtime_edge_discovery_204.cpp` and `build-artifacts/test-scripts/build-cinematic-runtime-edge-discovery-204.cmd`. The new read-only tracer opens a window at validated ENTER, correlates indirect dispatch edges by thread and shared `item+0x18` context, deduplicates target/vtable pairs into a fixed 64-edge table, caps total hits at 512, records first/last timing relative to ENTER and selected XMM lanes, and emits an edge summary at validated EXIT. The first build-script attempt used invalid dependency paths and was rejected; the script was corrected against the repository's existing `external` layout and the artifact then built successfully. Runtime capture is pending; no production source or stable gameplay ASI was changed.

Batch 2A runtime result: the new artifact installed successfully at `22:20:34` and opened a 13,047 ms window for context `0x1DDAA6F4A50`, but recorded `totalHits=0` and `uniqueEdges=0` before validated EXIT. This is a bounded negative result for the known indirect-dispatch observation site: no context-correlated dispatch edges occurred during the steady cinematic interval. It does not disprove a parallel FOV path elsewhere; it rejects this site as a useful discovery source for that interval. No candidate or write-test is authorized.

Batch 2B artifact: created `src/cinematic_thread_sample_discovery_204.cpp` and `build-artifacts/test-scripts/build-cinematic-thread-sample-discovery-204.cmd`. The read-only tracer arms on validated ENTER, samples only the identified cinematic thread at a bounded 10 ms interval until validated EXIT, records repeated executable RIP/XMM-lane candidates in a fixed 128-entry table, caps samples at 2048 and emits a summary. It does not write game memory, invoke game functions or alter page protection. The artifact built successfully; thread suspension is limited to the short `GetThreadContext` observation and remains a runtime-safety risk to evaluate from the isolated test.

Safety correction: the ENTER context read now uses guarded committed-memory validation and fails closed on unreadable `item+0x18`; the corrected artifact rebuilt successfully.

Batch 2B runtime result: the sampler opened the cinematic window and collected 1,219 samples, but produced 98 mostly single-hit RIP/XMM candidates with no repeated causal FOV boundary. The capture is rejected for evidence: candidate lines reported the monitor thread (`11976`) instead of the sampled cinematic thread (`9052`), and several entries had `ripRva=0`/non-executable RIPs. The values in the 30–160 range are therefore uncorrelated register noise, not promoted FOV evidence. No write-test is authorized. The sampler requires a bounded correction before another run: report the sampled thread ID, reject non-executable RIPs, and rank only repeated executable RIP/lane observations.

### Batch 2C — Bounded static inspection of runtime clusters

- Inspect only raw/current-build code around `RVA 0x2433F3E–0x2433F99` and `RVA 0x2240852–0x2240909`.
- Establish function boundaries, XMM5/XMM7 use, nearby calls, loads/stores, and whether either region forms a coherent data-flow or only contains unrelated sampled instructions.
- Use raw EXE bytes as the authority where Ghidra boundaries conflict.
- Do not search globally for FOV constants, reopen the closed dispatcher/setter branches, or promote a candidate without static and runtime correlation.

Batch 2C result: the current 2.0.4 Ghidra program was inspected read-only with a bounded script, and the two regions were independently checked against raw executable bytes from the known 2.0.4 executable. The primary cluster `RVA 0x2433F3E–0x2433F99` is inside `FUN_142433EE6` and decodes as control flow plus pointer/table traversal: conditional branches, `[R13+0x30]`, indexed record access, stack spills, `RBX=[RBP]`, then `RAX=[RBX]` and `RAX=[RAX+0x18]`. The sampled XMM7 values in this region are not consumed by any instruction in the cluster; there is no local FOV arithmetic, scalar load/store, or call boundary. The secondary cluster `RVA 0x2240852–0x2240909` is inside `FUN_1422406F3` and decodes as registry/lookup logic: a conditional branch, global read at `RVA 0xA0C3DC8`, ID comparisons at `+0x38`, indexed pointer comparisons, and counter/table access around `+0x8A0–0x8E8`; it likewise contains no XMM use, FOV transform, or camera-state store/call. Raw `.text` bytes match the decoded instruction starts. Result: both runtime clusters are rejected as FOV/data-flow candidates; their observed XMM values were stale sampler register state. The bounded cluster-inspection branch is closed without a write-test or a broader scan. The helper script is `02-Research/Ghidra/ghidra-scripts/DumpBoundedRuntimeClusters204.java`; the headless run processed only the 2.0.4 program by executable hash and used no full re-analysis.

### Batch 2 — Isolated runtime capture

- Enable only the new discovery tracer.
- Disable `STALKER2GameplayAspectFix.asi` and every other diagnostic/cinematic ASI.
- Use the established ultrawide display and the same natural cinematic transition.
- Analyze the resulting log for candidates tied to ENTER and stable cinematic execution.

### Batch 2A — Parallel cinematic runtime-boundary discovery

- Use validated ENTER to arm a bounded capture window and validated EXIT to close it.
- Collect deduplicated execution edges/call sites associated with the same cinematic camera/context, including a short interval before and after ENTER.
- Record only bounded candidate metadata: caller/return RVA, target RVA, thread, context/object identity, selected GPR/XMM lanes and guarded camera-state snapshots where available.
- Exclude the already closed `vtable+0x60` setter from candidate promotion; do not perform writes or call game-owned functions.
- Cap records and deduplicate repeated edges so generic `.text` traffic cannot consume the capture budget.
- Validation is limited to artifact build/install and production of a bounded runtime candidate list; no candidate is causal until a separate reversible write test.

### Batch 3 — Candidate decision

- Promote no candidate from numeric similarity alone.
- Select at most one bounded candidate for a separately planned Phase B reversible write test.
- If no candidate has a reproducible lifecycle-bound runtime signal, close Phase A as negative without broadening the scan.

## Risks and safe-failure behavior

- Runtime register snapshots may include unrelated traffic; lifecycle and thread/object correlation are required.
- Memory reads must be guarded and unresolved values logged as unavailable.
- The tracer must be read-only and must not call game functions or alter registers/state beyond the hook mechanism.
- Safe failure is no hook installation, no write, and a diagnostic log explaining the rejected validation.

## Stop conditions and phase gates

- Stop after the bounded ENTER-to-EXIT capture window is represented in the log.
- Stop if the trace produces only generic traffic without a cinematic lifecycle correlation.
- Stop this discovery batch if the bounded window cannot be correlated to the validated cinematic context; do not broaden to unrestricted process-wide execution tracing.
- Do not start Phase B until one concrete candidate is selected from runtime evidence.
- Do not reopen the closed ENTER-local static branch, `+0x230` branch, aspect/writer branch or EXIT restoration research.

## Final review

- Review the produced log and classify candidates as confirmed, plausible or rejected.
- Compare changed paths with this plan.
- Perform read-only Git status/diff review after any source or build-script change.
- Do not update the stable release or claim a cinematic fix until a separate Phase B test and regression evidence exist.
