# Legacy Live-FOV Consumption Boundary Recovery — 2.0.4 Task Plan

## Objective

Recover the current-build semantic equivalent of the legacy visually working
cinematic FOV consumption boundary. The target is a downstream call/data-flow
boundary where a live cinematic FOV scalar is passed through an XMM/register
argument into a cinematic or camera consumer and remains relevant downstream.

## Established evidence and current state

- Current executable: `Stalker2-Win64-Shipping.exe`, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Manual current-build state test confirmed that `+0x230=127.3927` together
  with aspect `3.5555556` gives correct cinematic framing after native FOV
  convergence, while immediate post-setter FOV writes are visually ineffective.
- Native FOV convergence `90.65574 → 90.0` and EXIT recovery are confirmed
  behavioral transitions; `90.0` remains an authored/native 16:9 FOV hypothesis,
  not a presumed error target.
- Prior current-build owner/interpolator candidates were rejected or held;
  `FUN_14569...` remains held.
- Legacy working behavior established a live FOV scalar/XMM call-flow boundary;
  historical addresses are evidence for reconstruction only and are not current
  build hook addresses.

## Approved scope

- Broad offline Ghidra analysis across the legacy working boundary and current
  2.0.4 executable.
- Reconstruct legacy XMM0 provenance, immediate callers/callees, CFG shape,
  nearby calls, object/register relationships and scalar propagation.
- Search all current 2.0.4 functions for structural/semantic equivalents,
  allowing inlining, splitting and compiler changes.
- Rank and document a shortlist of current-build boundaries.

## Explicit non-goals

- No runtime tracer, ordinary hook, write-test, ASI build or game launch.
- No PAGE_GUARD, VEH, random sampling or broad runtime instrumentation.
- No global FOV-like value scan used as proof of ownership.
- No reuse of legacy RVA/offsets as 2.0.4 addresses.
- No changes to stable gameplay or global experimental ASI sources.

## Expected files or areas

- Read-only Ghidra program data for the legacy and current executable versions.
- New reusable scripts under `02-Research/Ghidra/ghidra-scripts` if needed.
- New report under `02-Research/Ghidra/reports` containing the fingerprint,
  search method, ranking and evidence limits.
- This plan; no stable source or release assets.

## Batches and validation

### Batch 1 — legacy boundary reconstruction — COMPLETED

- Identify the exact legacy working hook boundary from existing evidence.
- Recover XMM0 provenance, value role, containing function, callers/callees,
  CFG and object/register relationships.

Validation: reproducible legacy program/version reference and a documented
semantic fingerprint distinct from raw RVA/offset values.

Evidence: `cutscene_letterbox_fix.cpp` and the existing legacy behavior establish
the live `XMM0` cinematic call-flow fingerprint; no durable FOV-field write is
part of the working mechanism.

### Batch 2 — current-build broad matching — COMPLETED

- Search the complete current 2.0.4 executable using the semantic fingerprint.
- Account for register renaming, inlining, split functions, changed layouts and
  changed call boundaries.
- Rank candidates by live scalar propagation, cinematic/camera plausibility,
  downstream call structure and ordinary observability.

Validation: generated candidate inventory, deduplication/family clustering and
explicit rejection of value-only or layout-only matches.

Evidence: existing-program read-only 2.0.4 pass completed with `7368` initial
structural candidates and `500` retained entries. The new report records the
semantic shortlist and its evidence limits.

### Batch 3 — shortlist review — COMPLETED

- Review the highest-ranked candidates and select at most a small shortlist for
  a future read-only runtime gate.
- Do not create that runtime artifact in this task.

Validation: each promoted candidate has a written reason tied to the fingerprint
and a clearly stated uncertainty.

Verdict: no current-build causal equivalent is promoted. `FUN_1404A4CCE` is the
leading static semantic neighbor, followed by `FUN_142D08BB0`, `FUN_14456D1B8`,
`FUN_1476C41A6` and a clustered scalar-propagation family. Full evidence is in
`02-Research/Ghidra/reports/legacy-live-fov-consumption-recovery-204.md`.

### Batch 4 — `FUN_1404A4CCE` scalar-propagation micro-review — COMPLETED

- Inspect only the ranked internal slice at load `0x1404A678C` through its
  downstream calls.
- Determine whether the loaded `XMM0` scalar survives the first call and
  reaches the later arithmetic/consumer, or whether the apparent lerp shape is
  driven by a returned/unrelated scalar.
- Do not create a runtime artifact until this lineage is established.

Validation: instruction-level lineage statement with the exact current-build
addresses and an explicit promote/reject decision.

Verdict: the initial `XMM0` scalar is not preserved into the later arithmetic;
the slice is rejected under the live-FOV consumption contract. No runtime
artifact is justified. Full evidence is in
`02-Research/Ghidra/reports/legacy-live-fov-consumption-micro-review-204.md`.

### Batch 5 — `FUN_142D08BB0` scalar/call micro-review — COMPLETED

- Inspect the exact pre-call scalar source, object setup, call target and
  callee-facing XMM argument at the ranked slice.
- Distinguish a valid callee argument boundary from unrelated post-call return
  arithmetic.
- Do not create a runtime artifact until the slice has a defensible lineage and
  consumer-role hypothesis.

Validation: instruction-level lineage and call-target review with an explicit
promote/reject decision.

### Batch 7 — `FUN_1476C41A6` scalar/consumer micro-review — COMPLETED

- Inspect the scalar source, first callee and object-local propagation in the
  ranked current-build slice.
- Determine whether the same scalar has a plausible live-FOV role and reaches a
  camera/cinematic downstream consumer.
- Do not create a runtime artifact until the lineage gate passes.

Validation: instruction-level lineage and consumer-role review with an explicit
promote/reject decision.

Verdict: the candidate is a sensitivity-settings/UI function. The ranked
`XMM0` scalar comes from `DAT_14b183300`, is stored into settings field
`+0x108`, and is committed through `FUN_143a56190`; the surrounding function
also handles aim and motion sensitivity controls. Live FOV provenance and a
cinematic consumer relationship were not established. The candidate is
rejected and no runtime artifact is justified. Full evidence is in
`02-Research/Ghidra/reports/legacy-live-fov1476c41a6-micro-review-204.md`.

### Batch 6 — `FUN_14456D1B8` scalar/call micro-review — COMPLETED

- Inspect the exact scalar source before the first call.
- Determine whether the same value reaches the first callee and whether any
  post-call derivative is propagated to a downstream consumer.
- Do not create a runtime artifact until live-FOV provenance is defensible.

Validation: instruction-level lineage and call-target review with an explicit
promote/reject decision.

Verdict: the scalar source is current-build `+FLT_MAX/-FLT_MAX` sentinel data,
not live FOV. The candidate is rejected and no runtime artifact is justified.
Full evidence is in
`02-Research/Ghidra/reports/legacy-live-fov14456d1b8-micro-review-204.md`.

## Acceptance gate

Strong result requires at least one current-build candidate with a plausible
live scalar FOV provenance, XMM/register argument propagation into a camera or
cinematic consumer, and downstream continuation of that value. A candidate is
not accepted from a constant, field offset, interpolation shape or numeric
match alone.

## Risks and safe failure

- Legacy boundary may be incompletely recoverable; record the missing evidence
  rather than inventing a fingerprint.
- Current compiler changes may split or inline the old boundary; search results
  must distinguish plausible families from exact matches.
- Ghidra analysis must use the existing programs read-only and must not trigger
  a full re-analysis of 2.0.3 or 2.0.4.
- If no candidate meets the acceptance gate, close the phase as bounded
  unresolved without creating a runtime artifact.

## Stop conditions and phase gates

- Do not build or run an ASI in this phase.
- Do not promote a candidate solely because it contains `90.0`, FOV-like
  constants, a familiar offset or interpolation-shaped arithmetic.
- Stop after the legacy fingerprint and one complete current-build matching pass;
  further runtime work requires a separate plan and explicit gate.
- Stable gameplay/global experimental ASI remain untouched.

## Final review

Compare changed paths with this plan, inspect the report and scripts, record
the exact executable/program identities and analysis mode, review Git status and
diff, and separate completed, remaining, deferred, blocked and not-runtime-
validated items.
