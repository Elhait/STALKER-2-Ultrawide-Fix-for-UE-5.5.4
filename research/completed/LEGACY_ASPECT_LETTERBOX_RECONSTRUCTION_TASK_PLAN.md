# Legacy cinematic aspect/letterbox reconstruction — 2.0.3 to 2.0.4

## Objective

Reconstruct the semantic role of the legacy working LetterboxFixA/B
boundaries and derive a portable fingerprint for mapping that role into the
2.0.4 executable.

## Established evidence and current state

- Legacy executable identity: SHA-256
  `81961b7281c7cf528ce49c549ce086fcc684bd676f32faf042bc743d939e3c69`.
- Legacy `FUN_142EE14BC` contains separate ENTER/EXIT branches.
- Both branches reach native aspect handling and the known aspect setters
  write `1.7777778` into a camera-related object at `+0x254`.
- Legacy aspect handling also changes/uses lifecycle flag state at `+0x259`.
- Current 2.0.4 direct state writers are known, but their presentation role is
  unresolved; this task must not assume they are the legacy descendants.

## Approved scope

- Read-only inspection of existing legacy reports, source and preserved
  Ghidra programs.
- Reconstruct LetterboxFixA/B instruction windows, callers, object
  provenance, direction and nearby helper/callback topology.
- Produce a semantic fingerprint suitable for a later 2.0.4 mapping pass.

## Explicit non-goals

- No current-build runtime artifact or ASI.
- No new hooks, writes, game launch, PAGE_GUARD/VEH or polling.
- Do not modify stable gameplay ASI, frozen FOV artifact or global baseline.
- Do not reuse legacy RVA/signatures as 2.0.4 addresses without fresh mapping.

## Batches and validation

### Batch 1 — legacy boundary reconstruction — COMPLETED

Review the legacy A/B signatures and surrounding `FUN_142EE14BC` control flow;
record exact writes, source object, lifecycle flags, helpers and direction.

Evidence: `02-Research/Ghidra/reports/legacy-aspect-letterbox-reconstruction-203.md`.

### Batch 2 — portable fingerprint — COMPLETED

Separate compiler-specific details from semantic invariants and document the
current-build mapping inputs required for a later static pass.

The portable fingerprint is recorded in the same reconstruction report.

### Batch 3 — final review — COMPLETED

Compare the report with this plan and perform a read-only Git review.

The reconstruction report matches the approved scope. Git review was
attempted but is blocked by the repository's Windows ownership/safe-directory
mismatch in this sandbox; no repository state was changed.

## Risks and safe failure

- The legacy setter may be lifecycle/state plumbing rather than presentation;
  do not infer a consumer role from the aspect constant alone.
- Historical signatures and offsets are version-specific evidence only.
- If legacy caller/object provenance is incomplete, record the gap rather than
  inventing a current equivalent.

## Stop conditions

- Stop after reconstructing the legacy boundary and portable fingerprint.
- Current-build mapping is a separate phase and requires its own plan/gate.
- No runtime promotion is allowed from this task alone.
