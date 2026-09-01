# Cinematic live-aspect consumer RE — 2.0.4

## Objective

Identify a current-build presentation/consumer boundary where authored
cinematic aspect `1.7777778` leaves native camera state, so a future aspect
transform can be evaluated without guessing a durable `+0x254` write timing.

## Established evidence

- 2.0.4 SHA-256:
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Cinematic FOV live-consumer boundary is confirmed separately at
  `0x2EE6936`; its artifact is frozen and out of scope here.
- Native cinematic lifecycle writes/uses aspect field `inner+0x254` and
  lifecycle flags at `inner+0x259`.
- `FUN_146B6C482` is confirmed lifecycle/state machinery, not yet a proven
  presentation consumer.

## Approved scope

- Read-only Ghidra analysis of existing 2.0.4 program.
- Search authored aspect constants and references, then inspect only bounded
  caller/callee neighborhoods around lifecycle/aspect candidates.
- Compare scalar/register/object provenance and downstream viewport/projection
  roles.
- Produce a ranked shortlist or a documented unresolved result.

## Explicit non-goals

- No ASI, runtime hook, game launch or state write.
- No PAGE_GUARD/VEH, sampling, polling or direct `+0x254` write test.
- Do not modify the frozen FOV artifact, stable gameplay ASI or global baseline.
- Do not infer a safe write boundary from a constant/reference alone.

## Batches and validation

### Batch 1 — constant/reference inventory — COMPLETED

Locate float/global/string references for authored aspect and record containing
functions and direct callers. Validate executable identity and read-only mode.

Evidence: `02-Research/Ghidra/reports/authored-aspect-references-204.log`.
The inventory identified lifecycle/state writers and a ratio-selection chain
around `FUN_14343BC61`, but did not by itself prove a presentation consumer.

### Batch 2 — semantic local review — COMPLETED

Inspect the strongest aspect candidates for presentation/consumer semantics,
object lineage and scalar propagation. Reject lifecycle-only/state-only paths.

Evidence: `02-Research/Ghidra/reports/current-aspect-consumer-review-204.md`.
The bounded review confirmed state writers and ratio selection/forwarding, but
did not establish a cinematic presentation consumer.

### Batch 3 — final review — COMPLETED

Document the shortlist, unresolved links and whether any ordinary runtime
boundary is justified. Perform read-only Git review.

Final evidence: `02-Research/Ghidra/reports/current-aspect-consumer-review-204.md`.
No ordinary presentation/consumer boundary was established; runtime promotion
is not justified. The repository Git review was attempted but is blocked by
the repository's Windows ownership/safe-directory mismatch in this sandbox.

## Risks and safe failure

- Constants may be shared by unrelated resolution/UI/renderer systems; require
  provenance and role evidence.
- Compiler folding or vector packing may hide scalar references; report this
  as uncertainty rather than broadening into runtime tracing.
- If no concrete consumer boundary is found, stop without an artifact.

## Stop conditions

- Stop after the bounded aspect-reference and local semantic passes.
- Runtime promotion requires one concrete ordinary boundary with provenance;
  no hook is authorized by this plan.
