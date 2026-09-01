# Legacy FOV Callgraph/Data-flow Reconstruction — 2.0.4

## Objective

Reconstruct the exact legacy working live-cinematic-FOV boundary and its
small callgraph/data-flow neighbourhood, then identify ranked current-build
descendants or semantic equivalents in the existing 2.0.4 Ghidra program.

## Established evidence and current state

- The historical working fix transformed a live game-owned scalar in `XMM0`
  at a cinematic call boundary; it was not based on a durable current-build
  `+0x230` field write.
- Current 2.0.4 behavior is established separately: native cinematic FOV
  converges toward `90.0`; a later one-shot `127.3927` plus `3.5555556`
  aspect produces correct 32:9 framing; native EXIT recovery and gameplay
  handoff work.
- Current scalar-shape candidates were rejected as unrelated settings,
  sentinel, generic scalar, or non-cinematic paths.
- No current-build live-FOV consumption boundary is currently promoted.
- The 2.0.4 executable identity is SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.

## Approved scope

- Reconstruct the legacy producer, caller, exact `XMM0` boundary, callee,
  surrounding argument/register setup, downstream propagation and one-to-two
  hop caller/callee neighbourhood.
- Compare that neighbourhood against the existing 2.0.4 program using CFG,
  callgraph, argument/data-flow, object/context and operation-role evidence.
- Allow for inlining, splitting, merging, renamed functions, changed register
  allocation and changed calling conventions.
- Produce a ranked, evidence-backed shortlist or a documented unresolved
  result.

## Explicit non-goals

- No runtime hooks, ASI builds, game launches or write tests.
- No PAGE_GUARD, VEH, random sampling or broad runtime tracing.
- No stable gameplay/global ASI edits.
- Do not reuse legacy RVAs/offsets as 2.0.4 addresses.
- Do not repeat broad ranking based only on `XMM0`, interpolation-shaped math,
  FOV-like constants or layout offsets.
- Do not promote `FUN_142AB...` solely because it remains in the old shortlist.

## Expected files and areas

- Legacy working-fix source and existing legacy evidence.
- Existing read-only Ghidra programs for 2.0.3 and 2.0.4.
- Reusable scripts under `02-Research/Ghidra/ghidra-scripts`.
- Reports under `02-Research/Ghidra/reports`.
- This task plan and, after scoped validation, the canonical task log.

## Batches and validation

### Batch 1 — legacy boundary reconstruction — COMPLETED

- Recover the exact old hook boundary, scalar producer, caller/callee edges,
  register/argument contract, downstream use and object/context lineage.
- Validation: reproducible legacy evidence with addresses, function names,
  instruction roles and explicit uncertainty where the old artifact is absent.

Result: reconstructed `FUN_142EE14BC` in 2.0.3. ENTER loads the game-owned
FOV global into `XMM0` at `0x142EE1510` and calls `FUN_146B68976`; EXIT loads an
object-local FOV scalar from `+0x38` at `0x142EE1584` and calls the same callee.
Both branches then share the related object resolver and transition helper,
with direction-specific vtable operations. The portable fingerprint and
evidence limits are recorded in
`02-Research/Ghidra/reports/legacy-fov-callgraph-boundary-203.md`.

### Batch 2 — current-build descendant search — IN PROGRESS / NOT PROMOTED

- Search the existing 2.0.4 program for descendants/equivalents of the
  reconstructed neighbourhood, including split/inline/merged possibilities.
- Validation: candidate evidence must cite graph/data-flow similarities, not
  numeric or offset resemblance alone.

Progress: the first inventory script produced a 100-result noisy list because
its predicate was only repeated direct calls preceded by `MOVSS`. It was
replaced with a read-only paired matcher requiring two separated scalar-
prepared calls to the same callee and the same short downstream direct call,
with absolute/object-relative source classification. The matcher is bounded
to functions of at most 1200 instructions and retains only the top 160 pairs,
but the full existing Ghidra listing still exceeded the practical runtime
budget before producing a ranked result. No candidate was promoted and no
runtime artifact was built. The partial inventory is recorded in
`02-Research/Ghidra/reports/legacy-graph-matching-204.log`; the refined run
was attempted in `legacy-graph-matching-204-paired.log` and safely stopped
before evidence output. The full-listing matcher is now closed as
method-not-practical, not as negative FOV evidence. The next bounded approach
is anchor-driven: map the legacy helper/resolver nodes individually into 2.0.4,
then walk only their local one-to-two-hop neighborhoods. Batch 2A covers
`FUN_1433EEA0E`, `FUN_146B68976`, `FUN_142630240`, `FUN_142616B94` and
`FUN_1426F250E`; Batch 2B checks whether mapped neighborhoods converge on a
current ENTER/EXIT transition cluster. No runtime artifact is justified until
that local graph correspondence exists. Batch 2A/2B then inspected the current
lifecycle cluster: `FUN_143016080 -> FUN_143016C98 -> FUN_146B6C482` is a
setup/registration/aspect topology, not the legacy live-FOV consumer. The
correspondence is therefore not established from this anchor; the result is
recorded in `02-Research/Ghidra/reports/current-lifecycle-local-graph-204.md`.
Batch 2A now pivots to individual legacy helper/resolver reconstruction,
starting with `FUN_142630240 -> FUN_142616B94`. String-anchored current
world-context resolution identified `FUN_1426EF832`, `FUN_14248BEB0` and
`FUN_14262E3D2`; bounded decompilation established `FUN_14262E3D2` as a
current resolver-role match and `FUN_142636472` as a strong semantic
descendant of legacy `FUN_142630240`. Local inspection found `FUN_142EE68DA`,
whose two-branch topology reproduces the shared helper, resolver-A,
resolver-B and direction-specific vcall sequence. Instruction-level review
also showed `MOVSS XMM0` immediately before both calls to `FUN_146B6DAAC`,
whose global-state/downstream-call structure is a strong semantic match for
legacy `FUN_146B68976`. The current runtime FOV curve is not yet correlated.
Evidence is recorded in
`02-Research/Ghidra/reports/current-legacy-anchor-mapping-204.md` and the
supporting logs. Batch 2A/2B static correspondence is complete for this
neighborhood. A separate read-only runtime correlation plan is now justified,
but no runtime artifact is created by this plan.

### Batch 3 — semantic discrimination and ranking — PENDING

- Cluster near-duplicates, reject subsystem-specific false positives and rank
  only candidates with a plausible live-FOV consumption role.
- Validation: each retained candidate has a written rationale and a clear
  runtime gate requirement; no runtime artifact is created by this plan.

### Batch 4 — final review — PENDING

- Compare changed paths with this plan, inspect reports/scripts, review Git
  status/diff and record completed, remaining, deferred, blocked and
  not-runtime-validated outcomes.

## Risks and safe-failure behavior

- The legacy artifact may not preserve enough surrounding context; record the
  missing link rather than inventing a match.
- Compiler transformations may erase a one-to-one function match; report
  producer/consumer alternatives separately.
- Existing Ghidra programs must be opened read-only with `-noanalysis`; do
  not trigger full re-analysis.
- If no current candidate meets the semantic contract, close this phase as
  unresolved and do not create a runtime artifact.

## Stop conditions and phase gates

- Stop after the legacy neighbourhood is reconstructed and one complete
  cross-version matching pass is documented.
- Runtime promotion requires a separate plan and a concrete boundary.
- Reject candidates supported only by constants, offsets, generic FP shape,
  or function-name/layout similarity.
- Stable gameplay/global experimental ASI remain untouched.

## Final review requirements

- Record executable/program identity, Ghidra mode and exact evidence paths.
- Verify no runtime artifact or production source was changed.
- Perform read-only Git review against the approved scope before reporting
  completion.
