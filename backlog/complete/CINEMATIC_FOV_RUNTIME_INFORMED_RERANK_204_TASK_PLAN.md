# Cinematic FOV Runtime-Informed Reranking 2.0.4 — Task Plan

## Objective

Re-rank the retained top-500 2.0.4 static candidates using the completed
runtime verdicts and stronger ownership-oriented criteria. Produce a new
shortlist for possible future read-only correlation without automatically
building or running another ASI.

## Established evidence and current state

- Executable: `Stalker2-Win64-Shipping.exe`, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Initial static inventory: `7368` candidates; top `500` retained.
- Runtime verdicts:
  - `FUN_1422FC35A`: active but candidate state did not track FOV; rejected as
    FOV owner.
  - `FUN_1405EDA3A`: zero hits in validated lifecycle; rejected.
  - `FUN_1431FA182`: zero hits in validated lifecycle; rejected.
- Native cinematic FOV behavior remains confirmed, but its owner is unresolved.

## Approved scope

- Parse and re-rank the existing top-500 inventory.
- Add runtime-informed penalties for rejected candidates and generic activity.
- Add explainable boosts for lifecycle plausibility, explicit scalar
  arguments/stores, object-local state, setup/reset/completion semantics and
  rare-boundary plausibility.
- Review the new shortlist statically and record reasons.

## Explicit non-goals

- No new Ghidra full analysis or re-analysis of 2.0.3/2.0.4.
- No runtime tracer, ASI build, game launch, hooks, writes, PAGE_GUARD, VEH or
  random sampling.
- No stable gameplay/global experimental ASI changes.
- No automatic runtime promotion of the highest-ranked result.

## Expected files or areas

- New research-only reranking helper and report under `02-Research/Ghidra`.
- This task plan; stable source and release assets untouched.

## Batches and validation

### Batch 1 — runtime-informed ranking — COMPLETED

- Consume the existing top-500 static inventory and runtime verdict ledger.
- Apply deterministic, explainable score adjustments.
- Preserve original static score and reasons beside the new score.

Validation: output identifies source reports, executable identity, adjustment
rules, rejected candidates and ranked results.

Result: the existing top-500 inventory was parsed and deduplicated to `351`
function/entry rows. Runtime hard penalties were applied to the three tested
and rejected candidates; original static scores and reasons were preserved.

### Batch 2 — shortlist review — COMPLETED

- Review the new top tier for object/state observability and subsystem-specific
  false positives.
- Mark candidates held, rejected, or eligible for a separately planned
  read-only runtime gate.

Current result: the top tier is still provisional. Static score adjustments
improve prioritization but do not establish ownership; no candidate is yet
eligible for runtime promotion.

Result: decompiled the tied top tier and a representative of the related
`FUN_14569...` family. Most rows were deprioritized as subsystem-specific,
large/generic or lacking lifecycle semantics. Three entries remain held:
`FUN_14027A5E4`, `FUN_146880C06`, and the `FUN_14569...` family represented by
`FUN_14569CAD2`. None is runtime-promoted.

### Batch 3 — final gate — COMPLETED

- Do not create a runtime artifact in this task.
- Any candidate promotion requires a new task plan.

Result: semantic review is complete. No runtime candidate is authorized by
this task; the held shortlist requires a separate plan for any future Gate A.

## Ranking rules

Upweight explicit scalar arguments/stores, object-local state, completion/reset
branches and plausible rare lifecycle boundaries. Downweight generic tick
loops, large multipurpose functions, subsystem-specific matches, constant-state
activity and layout-only similarity. Apply hard runtime penalties to the three
already rejected candidates.

## Risks and safe failure

- Log parsing ambiguity: preserve original score/reasons and mark uncertain
  adjustments rather than inventing evidence.
- Runtime verdicts are path-specific; penalties apply to the tested lifecycle,
  not as universal claims about each function's whole role.
- If the inventory cannot be re-ranked reproducibly, stop without promoting a
  candidate.

## Stop conditions and phase gates

- No new runtime artifact in this task.
- Do not automatically choose `FUN_1404A4CCE`.
- Stop after producing and reviewing the new ranked report.
- A future runtime test must have its own plan and isolated artifact.

## Final review

Compare changed paths with this plan, verify executable identity and ranking
inputs, inspect Git status/diff, preserve stable/global ASI files, and record
completed, remaining, deferred, blocked and not-runtime-validated items.
