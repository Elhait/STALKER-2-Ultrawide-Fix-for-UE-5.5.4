# Cinematic FOV Semantic Transition RE 2.0.4 — Task Plan

## Objective

Identify a bounded current-build semantic path that explains the native
cinematic FOV curves through a target, interpolation/blend state, delta-time
step, clamp or completion condition.

## Established evidence and current state

- Current executable SHA-256 is
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- ENTER converges `90.65574 -> 90.0` over approximately one second.
- After the native endpoint, one combined FOV/aspect correction gives correct
  cinematic framing and persists without repeated writes.
- EXIT converges the modified FOV back to the ENTER baseline and returns to
  gameplay flags `0x04`.
- Layout-based `+0x230` writer candidates were runtime-rejected and must not be
  reopened.

## Approved scope

- Read-only inspection of existing current-build reports and runtime evidence.
- Search only for semantic fingerprints: target/desired value, current-target
  difference, interpolation/blend weight, delta-time, clamp/snap or completion.
- Prefer paths plausibly shared by ENTER and EXIT.
- Keep the global experimental ASI and stable gameplay ASI untouched.

## Explicit non-goals

- No new ASI, runtime hook, write test or global/static broad scan in this pass.
- No direct `+0x230` writer enumeration or layout-based candidate promotion.
- No legacy RVA/offset reuse, PAGE_GUARD/VEH or production integration.
- No claim of a native owner without current-build runtime correlation.

## Expected files or areas

- Existing 2.0.4 Ghidra reports containing interpolation, clamp, target or
  completion evidence.
- Existing ENTER/EXIT behavioral logs and task records.
- This task plan only unless a bounded analysis report is explicitly required.

## Batches and validation

### Batch 1 — evidence inventory

- Locate current-build reports with interpolation/target/time/clamp semantics.
- Exclude historical and layout-only candidate material.

Validation: executable identity and report provenance cross-check.

Result:

- The inspected reports identify themselves as read-only analyses of the
  current `Stalker2-Win64-Shipping.exe (2.0.4)` project entry; the executable
  identity remains the SHA-256 recorded above.
- Historical 2.0.3/2.0.2 sections were excluded from consideration.
- Existing reports contain semantic-looking math, but no report currently
  binds a target/blend value to the validated cinematic `inner` or to the
  observed FOV curve.

### Batch 2 — semantic candidate review

- Inspect only a small number of current-build neighborhoods already connected
  to camera/cinematic state.
- Record whether each candidate has evidence for current, target, blend or
  completion semantics.

Validation: value similarity alone is insufficient; candidates must explain a
transition component.

Reviewed bounded neighborhoods:

1. `FUN_1424d8a58` in `exit-writer-pairs-204-20260829.log` contains an
   interpolation-shaped sequence (`SUBSS`, `MULSS`, `ADDSS`) and clamp logic,
   but it operates on an object reached through `RAX` and writes an aspect/
   state field. The report does not establish `+0x230` FOV provenance, the
   validated cinematic `inner`, or ENTER/EXIT FOV targets.
2. `FUN_140773c32` contains scalar conversion, square-root/division and
   comparisons around aspect/state fields, but no bounded evidence connects
   its values to the native `90.65574 -> 90.0` or reverse EXIT curve.
3. `FUN_140bc5090` compares state values and uses a global double converted to
   scalar, but the available context does not identify a FOV target, blend
   input, authoritative `inner`, or cinematic lifecycle correlation.
4. The current camera-field/access and previous writer reports contain many
   additional scalar and block operations. They are layout/access inventories
   rather than causal transition evidence and were not promoted.

Verdict for all four: unresolved as native FOV-transition candidates; none
meets the acceptance gate.

### Batch 3 — bounded decision

- Mark candidates as accepted, rejected or unresolved.
- Stop if no candidate has a defensible current-build semantic link.

Validation: no runtime artifact or implementation follows from this pass
without a reproducible candidate boundary.

Result:

- No candidate explains enough of `current -> target -> blend/time -> next`
  for the authoritative cinematic object.
- No candidate is accepted for runtime correlation.
- The semantic static pass is closed as bounded-unresolved; the cinematic FOV
  path remains unresolved rather than disproven.
- No new ASI, hook, write test, or production-source change was made.

## Risks and safe failure

- Decompiled math can look like interpolation while serving unrelated data;
  preserve uncertainty until runtime correlation exists.
- Historical reports can contain valid-looking legacy transitions; use only
  the current executable identity.
- If the semantic inventory is empty or ambiguous, close the pass without
  widening the search.

## Stop conditions and phase gates

- Do not inspect more than a few bounded current-build neighborhoods.
- Do not return to rejected writer candidates or broad FOV-like scans.
- No runtime artifact is justified unless a semantic candidate is first found.

## Final review

Report exact evidence sources, current-build identity, candidate verdicts,
untouched ASI/source files, unresolved assumptions and whether a separate
runtime-correlation plan is justified.

Final review status:

- Evidence sources reviewed: `exit-writer-pairs-204-20260829.log`,
  `camera-field-accesses-204-20260829.log`,
  `revalidation-204-direct-20260829.log`,
  `callback-dispatcher-bounded-204-20260829.log`, and
  `semantic-fingerprint-204-20260829.log`.
- Stable gameplay source, global experimental ASI and all runtime artifacts
  were intentionally untouched.
- A separate runtime-correlation plan is not justified by this pass because
  no bounded semantic candidate survived review.
