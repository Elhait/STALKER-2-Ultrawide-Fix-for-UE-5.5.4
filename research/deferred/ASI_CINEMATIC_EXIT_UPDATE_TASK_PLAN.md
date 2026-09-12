# Cinematic EXIT Resolver Update — Task Plan

## Objective

Adapt the unified ASI cinematic EXIT resolver to the current game executable
after the existing resolver reported `enterMatches=1` and `exitMatches=0`.

## Established evidence and current state

- Current game executable SHA-256:
  `E7B481A97C02D80581FAB0BECE940214A88EBE30211088A00129845A039F9293`.
- Existing runtime log shows gameplay and dialogue setup succeeded, while the
  cinematic FOV resolver rejected the old EXIT pattern.
- The current source uses wildcarded relative displacements, but also assumes
  a specific instruction topology and fixed validation vcall pairs.

## Approved scope

- Inspect the current executable for the bounded cinematic ENTER/EXIT pattern.
- Update only the cinematic resolver/signature validation in
  `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- Rebuild `STALKER2CameraTweaks.asi`.
- Perform static build and identity/diff review.

## Explicit non-goals

- No changes to gameplay or dialogue behavior.
- No broad renderer or settings reverse engineering.
- No runtime hook writes beyond the existing cinematic implementation.
- No game launch, injection or installation.
- No deletion or cleanup.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `build.cmd` only if required by the approved source/output contract
- `STALKER2CameraTweaks.asi`
- `backlog/TASKLOG.md`

## Batches and validation

### Batch 1 — current-image signature inspection

- Verify the current executable identity.
- Locate and validate only the cinematic EXIT boundary and its shared
  consumer/continuation semantics.

Validation: exact byte-pattern inventory and source-level resolver review.

### Batch 2 — bounded implementation

- Update the resolver only when the current image provides a concrete,
  validated replacement topology.

Validation: local VS 2022 build and static diff review.

## Risks and safe failure

- If the current image does not provide a sufficiently specific EXIT boundary,
  leave the resolver unchanged and report the blocker.
- Preserve safe refusal on ambiguity or validation mismatch.
- Build success does not prove runtime compatibility.

## Stop conditions

- Stop before source edits if executable identity or target ownership cannot be
  established.
- Stop if adapting the pattern would require broad or guessed matching.
- Stop before runtime injection or game-folder installation.

## Final review

- Compare changed paths with this plan.
- Record build result, current executable identity, runtime limits and remaining
  compatibility work in `backlog/TASKLOG.md`.
