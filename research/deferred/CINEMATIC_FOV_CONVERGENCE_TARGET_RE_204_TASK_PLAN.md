# Cinematic FOV Convergence Target RE 2.0.4 — Task Plan

## Objective

Identify, using bounded current-build evidence, the native target/input or deterministic update/completion path responsible for the cinematic FOV convergence from approximately `90.6557` to `90.0`.

## Established evidence and current state

- The 2.0.4 cinematic authoritative inner is validated at runtime.
- ENTER uses `flags=0x05`; native FOV converges from approximately `90.6557` to `90.0` over about one second.
- After convergence, one combined correction to FOV `127.3927` and aspect `3.5556` gives correct cinematic framing and persists without repeated writes.
- EXIT changes to `flags=0x04` and native FOV converges from the modified value back to the ENTER baseline.
- The experimental global ASI is a working behavioral baseline and must remain untouched.

## Approved scope

- Read-only bounded analysis of current 2.0.4 executable evidence and existing runtime traces.
- Search for the native convergence target/input or a deterministic update/completion boundary.
- Correlate any candidate against the same authoritative inner and both ENTER/EXIT fingerprints.
- Use prior legacy behavior only as semantic context, never as an address or offset source.

## Explicit non-goals

- No modification or rebuild of the global experimental ASI.
- No modification of `STALKER2GameplayAspectFix.asi` or release source.
- No direct `+0x230` writer enumeration, legacy offset revival, PAGE_GUARD/VEH, broad renderer scan or new runtime hook.
- No production implementation in this phase.

## Expected files or areas

- Existing 2.0.4 Ghidra/disassembly reports and research notes.
- Existing runtime logs for the global ASI and gameplay aspect fix.
- This task plan only unless a bounded analysis report is explicitly needed.

## Batches and validation

### Batch 1 — evidence inventory

- Locate current-build reports, scripts and logs relevant to validated ENTER, EXIT and FOV convergence.
- Confirm executable identity and exclude historical 2.0.3/legacy-only material from current-build conclusions.

Validation: read-only file inspection and version/evidence cross-check.

Status: completed. Current-build evidence was restricted to the 2.0.4 executable
with SHA-256 `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
Historical 2.0.3/legacy reports were not used as current-build proof.

### Batch 2 — bounded candidate analysis

- Inspect only the immediate camera/cinematic update neighborhoods already established by runtime evidence.
- Look for target/input propagation, interpolation math or completion conditions associated with the observed FOV curve.

Validation: candidate must be tied to current 2.0.4 code evidence; value similarity alone is insufficient.

Status: completed for the existing bounded report set. The current 2.0.4
`exit-writer-pairs` report identifies two state-commit candidates that write the
`+0x230` field in the same local block as `+0x254` and adjacent mode/flag fields:

- `FUN_140187898`, block at `0x1401882ba`: writes `+0x224..+0x25c`, including
  `+0x230` from `XMM4`, `+0x254` from `XMM13`, and then performs follow-up scalar
  processing. Its semantic fingerprint includes `+0x254`, `+0x2A0`, `+0xF8`
  and indirect calls.
- `FUN_14038e7d0`, block at `0x140392e01`: writes `+0x230` from `XMM0` and
  the surrounding `+0x234..+0x26c` state, including `+0x254` and flag fields.
  Its fingerprint also includes `+0x254`, `+0x2A0`, `+0xF8`, `+0x180` and
  indirect calls.

The known `FUN_140af4022` path is an output/consumer-side reader of
`source+0x230` and is not promoted as the native convergence writer. No
current-build runtime correlation to the ENTER/EXIT FOV curves has yet been
established.

### Batch 3 — correlation decision

- Compare candidates against both ENTER and EXIT fingerprints.
- Record accepted, rejected and unresolved candidates.

Validation: no candidate is promoted without a reproducible current-build causal link.

Status: static correlation decision completed; runtime correlation pending.
Neither candidate is accepted as the native convergence path: the existing
evidence does not correlate either function to the same cinematic `inner` or
to both the ENTER and EXIT curves. The result is therefore a bounded candidate
inventory, not proof that either function owns the transition.

## Risks and safe failure

- Historical offsets or stale Ghidra projects may produce false candidates; they are evidence only after current executable identity is confirmed.
- If the bounded neighborhood yields no causal target/update path, close this pass as unresolved/negative for the bounded method without widening the search.
- No runtime artifact is built during this pass.

## Stop conditions and phase gates

- Stop after the approved camera/cinematic neighborhoods are exhausted.
- Do not broaden into all FOV-like values, all scalar-FP instructions or all renderer code.
- A candidate that cannot be correlated with the same inner and convergence curve remains rejected.

## Final review

Report exact evidence sources, current-build identity, candidate status, untouched ASI/source files and whether the next step is justified.

Final bounded-pass result: current-build static evidence produced two plausible
state-commit candidates, with `FUN_140187898` prioritized for the first
runtime check and `FUN_14038e7d0` retained as the second candidate. Static
correlation is complete; runtime correlation remains pending. The next step,
if pursued, requires a separate read-only artifact for those two addresses
only; no implementation or production change is justified by this pass.
