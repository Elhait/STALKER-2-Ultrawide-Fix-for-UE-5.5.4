# Cinematic FOV Legacy Cross-Version Static RE 2.0.4 — Task Plan

## Objective

Reconstruct the historically working cinematic FOV boundary from the legacy
build, derive a semantic/structural fingerprint, and perform a broad offline
cross-version search across the current 2.0.4 executable. Produce a ranked
shortlist of current-build candidates for later runtime validation.

## Established evidence and current state

- Current executable identity: SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- The legacy FOV correction produced the desired cinematic framing; its known
  defect was lifecycle overshoot on exit, not lack of visual causality.
- Current 2.0.4 manual/global evidence confirms the desired combined aspect/FOV
  state, while the native FOV owner remains unresolved.
- Prior bounded current-build searches for layout writers, local setter flow,
  random register snapshots, generic dispatch and static interpolation-shaped
  math did not produce an accepted owner.
- Runtime work is explicitly deferred until this static pass produces a ranked
  candidate boundary.

## Approved scope

- Read-only reconstruction of the legacy working boundary: hook instruction,
  containing function, XMM0 provenance, pre/post propagation, callers/callees,
  CFG shape, object/register relationships and relevant constants/vtables.
- Broad offline analysis of all available legacy and current Ghidra program
  databases and reports.
- Cross-version structural matching that tolerates changed offsets, inlining,
  function splitting and compiler rearrangement.
- Automated candidate ranking using multiple signals, followed by manual review
  of the highest-ranked neighborhoods.
- Creation of research-only scripts/reports under `02-Research/Ghidra`; do not
  alter stable gameplay or experimental runtime modules.

## Explicit non-goals

- No runtime tracer, broad hook, write test, game launch or injected ASI.
- No PAGE_GUARD, VEH, random sampling or game-state writes.
- Legacy RVA/offsets are evidence for reconstruction only and must never be
  emitted as 2.0.4 implementation addresses without fresh validation.
- No production integration, release packaging or stable gameplay changes.

## Expected files or areas

- Legacy working-fix source, reports and Ghidra program data.
- Current 2.0.4 Ghidra program and reports.
- New reusable scripts in `02-Research/Ghidra/ghidra-scripts`.
- New analysis reports under `02-Research/Ghidra/reports`.
- This plan and, if needed, a concise evidence note; no ASI output expected.

## Batches and validation

### Batch 1 — legacy boundary reconstruction — COMPLETED

- Identify the exact old hook instruction and containing function.
- Trace XMM0 backward to its source and forward through calls/stores/returns.
- Record nearby FP operations, constants, object/register relationships, callers,
  callees and CFG features.

Validation: every fingerprint item must be tied to a concrete legacy address,
instruction or data relationship; distinguish confirmed facts from hypotheses.

Result: the historical source confirms `MOVSS XMM0,[RIP+global]` followed by
the hooked cinematic call boundary. The correction mutates only the incoming
scalar XMM0 lane and lets the original call continue; it does not identify a
durable camera-field write. Legacy addresses remain historical evidence only.

### Batch 2 — current-build exhaustive candidate generation — COMPLETED

- Search the complete current 2.0.4 executable for the fingerprint components.
- Include direct and indirect argument flow, scalar FP transforms, relevant
  callgraph patterns, vtable/copy relationships and tolerant CFG signatures.
- Generate a broad candidate inventory with machine-readable scores and reasons.

Validation: candidate generation must not use legacy addresses as current
  addresses; scores must expose their contributing signals.

Result: the correct 2.0.4 program was processed read-only and matched the
verified executable hash. The inventory contains `7368` initial candidates;
the ranking retained the top `500` with explainable structural signals.

### Batch 3 — ranking and manual review — COMPLETED / BROAD STATIC-UNRESOLVED

- Rank candidates by combined structural, semantic and object/context signals.
- Review a sufficiently broad top tier rather than stopping after two or three
  failures.
- Mark each candidate accepted-for-runtime, rejected, or unresolved.

Validation: no candidate is promoted solely by a shared constant, offset or
  interpolation-like instruction; runtime correlation remains a separate gate.

Result: reviewed the high-scoring structural candidates and semantic neighbors,
including `FUN_1422FC35A`, `FUN_1431FA182`, `FUN_1404A4CCE`,
`FUN_1405EDA3A`, and the known `FUN_146B6C482` EXIT anchor. None has proven
current-build cinematic-camera ownership or the legacy live-FOV causal chain.
No runtime candidate was promoted.

## Risks and rollback or safe failure

- Cross-version similarity can produce unrelated mathematical or engine-code
  matches; preserve explainable scoring and provenance.
- Ghidra scripts must run read-only against copied/read-only project inputs and
  write only reports/scripts in the research area.
- If legacy reconstruction is incomplete, stop ranking and report the missing
  fingerprint rather than inventing one.
- If the current-build inventory is large or noisy, retain it as a report and
  narrow only through explicit scoring signals, not arbitrary manual selection.
- The reviewed shortlist is evidence for later investigation only; it is not a
  runtime hook list.

## Stop conditions and phase gates

- Do not begin runtime work until Batch 3 produces a documented shortlist.
- Do not convert legacy addresses into current hooks.
- Do not stop merely because the first few current candidates fail static review.
- Stop after the full approved search/ranking pass if no candidate has enough
  multi-signal evidence for runtime validation; classify the result as broad
  static-unresolved.

Current gate: met. The broad static pass is closed as unresolved; any future
runtime work requires a new, separately scoped evidence phase.

## Final review

Review generated scripts/reports, executable identities, candidate scores,
changed paths, Git status/diff, untouched stable/global ASI areas, validation
limits and the exact runtime-validation gate for any promoted shortlist.
