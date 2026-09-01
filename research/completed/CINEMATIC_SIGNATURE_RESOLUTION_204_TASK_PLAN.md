# CINEMATIC_SIGNATURE_RESOLUTION_204

Status: Complete for the validated 2.0.4 executable. Signature-based
resolution and runtime installation passed for the gameplay writer, cinematic
aspect store and cinematic ENTER/EXIT FOV boundaries. No future-patch
guarantee is implied.

## Objective

Replace the unified artifact's fixed 2.0.4 cinematic RVAs with guarded,
unique signature resolution derived from the reconstructed 2.0.3/2.0.4
transition topology.

## Established evidence and current state

- Legacy 2.0.3 `FUN_142EE14BC` and current 2.0.4 `FUN_142EE68DA` preserve the
  ENTER/EXIT transition shape and downstream resolver/vcall topology.
- The validated current boundaries are the cinematic aspect store and the
  ENTER/EXIT live-FOV consumer callsites, previously observed at fixed RVAs
  `0x6B7CB05`, `0x2EE6936` and `0x2EE69A7`.
- Fixed RVAs are version-specific evidence, not a patch-independent resolver.
- Existing gameplay signature resolution is the accepted safety model:
  executable-wide scan, exactly one match, instruction decode/semantic
  validation, fail-closed on ambiguity.

## Approved scope

- Analyze the legacy/current boundary fingerprints already established in the
  research reports.
- Add signature resolution for the current cinematic aspect store and both
  current live-FOV callsites.
- Require unique matches and instruction/call validation before installing
  cinematic hooks.
- Preserve the unified artifact name and INI feature switches.
- Keep the stable gameplay source and its intentional staged changes intact.

## Explicit non-goals

- No new runtime hooks, writes, or gameplay/cinematic behavior changes beyond
  resolving the same already-validated boundaries.
- No broad renderer search, new FOV/aspect algorithm, or post-EXIT seam work.
- Do not claim patch independence until a different executable build is
  resolved and runtime-validated.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- this task plan;
- a concise research note documenting the signatures and their legacy/current
  semantic correspondence;
- generated test ASI only.

## Batches and validation

### Batch 1 — legacy/current fingerprint review

Completed: confirmed from the 2.0.3/2.0.4 Ghidra differential that the
transition hub preserves the ENTER/EXIT scalar-source asymmetry, consumer
calls, resolver chain and directional vcall pairs. The patterns and limits
are recorded in `02-Research/Ghidra/reports/cinematic-signature-resolution-204.md`.

### Batch 2 — resolver implementation

Completed: the unified source now resolves the aspect setter and ENTER/EXIT
FOV boundaries with unique structural signatures. It rejects ambiguity,
validates the aspect MOV and decodes both FOV MOVSS/CALL boundaries. The two
FOV calls must target the same executable consumer. The INI parser also now
correctly reads the documented `[Gameplay]` and `[Cinematics]` sections while
retaining legacy `[Features]` compatibility.

### Batch 3 — build validation

Completed: `STALKER2UltrawideFix.asi` builds successfully. The available
current executable contains exactly one aspect-setter signature match. Build
success and static match counts are not runtime proof.

The first runtime attempt correctly failed closed because the initial FOV
patterns modeled the resolver calls incorrectly and produced zero matches.
The patterns were corrected from the actual 2.0.4 instruction differential;
the unified log name is now `STALKER2UltrawideFix.log`.

### Batch 4 — user runtime validation — COMPLETED

Completed: user runtime validation confirmed the resolved boundaries and
combined behavior on 2.0.4. Later policy tests also confirmed Auto and forced
16:9/21:9/32:9 modes. Disabled
feature combinations separately if desired.

### Batch 5 — legacy binary validation

Blocked by input availability: the exact raw 2.0.3 executable with accepted
hash `81961b7281c7cf528ce49c549ce086fcc684bd676f32faf042bc743d939e3c69` is
not present. Available Ghidra images have different identities and cannot
validate the 2.0.3 raw-byte matches. The attempted scan is recorded in the
signature-resolution research note.

## Risks and rollback or safe-failure behavior

- Signature ambiguity, decode failure, wrong instruction form, or mismatched
  ENTER/EXIT consumer target must refuse cinematic installation safely.
- The fixed-RVA evidence remains available in reports for diagnosis only and
  must not be used as a runtime fallback.
- Rollback is removal of the new ASI/config; the stable gameplay ASI remains
  untouched.

## Stop conditions and phase gates

- Stop before build if a unique, semantically validated pattern cannot be
  established from the current executable evidence.
- Stop runtime promotion if resolver logs do not identify all enabled cinematic
  boundaries or behavior differs from the established artifact.
- Do not describe the result as cross-patch compatible without another
  executable identity and runtime validation.

## Final review requirements

- Perform read-only Git status/diff review against this plan.
- Confirm stable gameplay changes are intentional and untouched.
- Report completed, remaining, deferred, and not-runtime-validated items.
