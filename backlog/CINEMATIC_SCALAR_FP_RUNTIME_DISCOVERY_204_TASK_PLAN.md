# Cinematic Scalar-FP Runtime Discovery 2.0.4 — Task Plan

## Objective

Find a causal current-build cinematic FOV data-flow boundary by observing actual scalar floating-point instructions or scalar XMM call arguments during the validated 2.0.4 cinematic lifecycle.

## Established evidence and current state

- 2.0.4 executable identity: SHA-256 `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Cinematic ENTER and EXIT anchors are runtime validated.
- The ENTER `vtable+0x60` setter is an aspect/mode-only transition; its local data-flow contains no FOV argument or transform.
- `+0x230` supports a deterministic one-shot write/restore lifecycle but did not produce the required cinematic framing through the tested Hor+ correction.
- Settings-FOV discovery, gameplay writer discovery and random `GetThreadContext()` XMM sampling are closed as unsuitable sources for causal FOV attribution.
- Runtime sampling produced stale XMM values at unrelated instructions; numeric similarity alone is not evidence.

## Approved scope

- Create one read-only experimental tracer for current 2.0.4.
- Arm capture at the validated cinematic ENTER and disarm at validated `FUN_146B6C482` EXIT.
- Use only a bounded, explicitly selected camera/cinematic execution source; do not scan the whole executable.
- Record only actual scalar-FP instruction/call-boundary evidence: instruction RVA, decoded operation class, relevant source/destination operand addresses or registers, scalar values where safely available, caller/return site, thread, timing and correlated camera/context identity.
- Deduplicate and cap records so generic execution cannot flood the log.

## Explicit non-goals

- No game-state writes, FOV/aspect modification, hotkeys, timers, polling or game-function calls.
- No broad `.text` scalar-FP scan.
- No promotion based only on an XMM snapshot or a value near 90.
- Do not reopen the closed `+0x230`, Settings, gameplay-writer, ENTER-local or random-sampler branches.
- Do not modify `STALKER2GameplayAspectFix.asi` or production source.

## Expected files or areas

- New research source under `src/`.
- New build helper under `build-artifacts/test-scripts/`.
- New experimental `.asi` under `build-artifacts/test-asi/`.
- This task plan and, after scoped validation, the research task log if required.

## Batches

### Batch 1 — Bounded instruction-semantic tracer

- Define the bounded runtime source and validate its raw bytes/semantics before installing.
- Hook only the selected source and lifecycle anchors.
- Record actual scalar-FP operations or scalar call arguments, not sampled register state.
- Build and inspect the artifact; no in-game causal claim from build success.

Batch 1 result: created `src/cinematic_scalar_fp_runtime_discovery_204.cpp` and `build-artifacts/test-scripts/build-cinematic-scalar-fp-runtime-discovery-204.cmd`. The artifact validates the existing 2.0.4 ENTER signature, uses the confirmed EXIT callback RVA, arms a guarded page only for the resolved authoritative inner camera state, decodes actual faulting instructions with Zydis and logs only scalar-FP instruction classes. It does not write game memory, call game functions, scan the executable broadly or use random register snapshots. The build succeeded and produced `build-artifacts/test-asi/STALKER2CinematicScalarFpRuntimeDiscovery204.asi`. This is build/read-only setup evidence only; runtime candidate discovery is pending.

Batch 1 correction: the first isolated run installed successfully but never armed the ENTER window; both EXIT records therefore had `inner=0` and `scalarRecords=0`. This is rejected as setup evidence, not as a FOV-negative result. The ENTER resolver now derives `inner` from the confirmed calling convention `RCX=item -> [item+0x18]`, falls back to `RAX`, and logs rejected ENTER identity when the state is unreadable. The corrected artifact rebuilt successfully. No runtime evidence is promoted and no write-test is authorized.

Batch 1 activation correction: the second isolated run again reached only EXIT with `inner=0`, so the setter was not a reliable activation event in that scenario. The tracer now also arms from the already runtime-validated indirect dispatch call at `RVA 0x26F7A23` after validating raw bytes `FF D0`; it resolves the same `RCX=item -> [item+0x18]=inner` identity before the setter and retains the setter hook as a secondary activation path. The artifact rebuilt successfully. The two runs remain setup-invalid for scalar-FP conclusions; no candidate or write-test is promoted.

Safety result: the next run logged only installation and then the game terminated with `EXCEPTION_ACCESS_VIOLATION writing address 0xffffffffb5409105`; no ENTER window or scalar-FP record was produced. This is a tracer safety failure after hook installation, not runtime evidence about the cinematic FOV path. The artifact is disabled for further runs pending root-cause isolation; no write-test, source integration or stable gameplay change is authorized.

Offline postmortem: the dispatch callback was attached to generic `RVA 0x26F7A23` `CALL RAX`, but it did not filter `context.rax` against the validated ENTER setter target before calling `ArmInner`. It could therefore apply `PAGE_GUARD` to arbitrary `RCX`-derived pages during generic traffic, explaining the crash before any `window-enter` record. Independent safety risks remain in doing `VirtualProtect`, locking and spdlog/Zydis work from a VEH/single-step path, plus unsynchronized page state across threads. The guard-page scalar-FP design is rejected for further runtime use; no rerun or replacement artifact is authorized in this batch.

### Batch 2 — Isolated runtime discovery

- Enable only the new tracer.
- Disable gameplay and all other diagnostic ASIs.
- Run one natural cinematic transition at the established ultrawide resolution.
- Promote only repeatable, context-correlated instruction-level candidates.

### Batch 3 — Candidate decision

- If no instruction semantically uses a repeatable cinematic scalar, close this method as negative.
- If one bounded candidate emerges, create a separate plan before any reversible write test.

## Validation

- Batch 1: source/build validation, raw-byte validation of the selected hook region, fail-closed resolver behavior and read-only inspection.
- Batch 2: runtime log must show ENTER/EXIT window, actual instruction semantics, valid executable addresses, correct sampled thread metadata and bounded records.
- No runtime visual or causal success may be claimed without a user-provided isolated run and visual result.

## Risks and safe failure

- Instruction hooks can be version-specific and may observe unrelated scalar operations if the source is too broad.
- Resolver ambiguity, byte mismatch, unreadable context or unsupported operand decoding must disable the hook without writes.
- The tracer must not alter game state or call game-owned functions.

## Stop conditions and phase gates

- Stop after one bounded ENTER-to-EXIT capture.
- Stop if the selected source produces only snapshots, generic traffic or values without instruction-level use.
- Do not broaden the source or scan the whole executable after a negative result.
- Do not start a write test until one concrete instruction/call candidate is selected.

## Final review

- Classify results as confirmed, plausible or rejected.
- Compare changed paths with this plan.
- Perform read-only Git review for the canonical repository after each changed batch.
- Keep the stable gameplay ASI and release artifacts untouched.
