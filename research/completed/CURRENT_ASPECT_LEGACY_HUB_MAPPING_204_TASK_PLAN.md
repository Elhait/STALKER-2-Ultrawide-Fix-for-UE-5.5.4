# Current-build aspect mapping from legacy lifecycle hub — 2.0.4

## Objective

Determine whether the current `FUN_142EE68DA` transition descendant also owns
the legacy cinematic aspect write boundary, or whether that write was split
into adjacent lifecycle machinery.

## Established evidence and current state

- Legacy `FUN_142EE14BC` is a direction-aware transition hub with native
  aspect/lifecycle handling.
- Current `FUN_142EE68DA` preserves the resolver/vcall/final-helper topology
  and is a very strong transition-hub descendant.
- Current direct `+0x254` writers are observed in nearby lifecycle/setup
  functions, not yet proven as the legacy setter descendants.

## Approved scope

- Read-only comparison of `FUN_142EE14BC` and `FUN_142EE68DA`.
- Inspect at most one hop from the current hub for aspect state writes,
  object lineage and helper relationships.
- Produce a mapping verdict; runtime promotion is a separate future task.

## Explicit non-goals

- No ASI, runtime hook, game launch or state write.
- No global `+0x254` scan, PAGE_GUARD/VEH or broad current-build search.
- Do not modify the stable gameplay fix, global baseline or frozen FOV artifact.

## Batches and validation

### Batch 1 — hub differential — COMPLETED

Compare direction branches, scalar sources, resolver pair, vcall slots and
final helper. Evidence is recorded in
`02-Research/Ghidra/reports/current-aspect-legacy-hub-mapping-204.md`.

### Batch 2 — aspect sub-boundary — COMPLETED

Check whether the current hub itself writes `inner+0x254` or delegates to a
nearby helper. Result: the hub body has no direct `+0x254` write; nearby state
writers remain semantically unresolved as legacy descendants.

### Batch 3 — final review — COMPLETED

No runtime boundary is promoted. Git review was attempted but is blocked by
the repository's Windows ownership/safe-directory mismatch in this sandbox;
no repository state was changed.

## Stop conditions

- Stop this local mapping pass after the hub/sub-boundary verdict.
- Do not build a runtime artifact without a concrete aspect boundary and
  ordinary hook safety validation.
