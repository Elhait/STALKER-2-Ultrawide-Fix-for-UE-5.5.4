# INI Boolean Help Text — Task Plan

## Objective

Clarify the meaning of `true` and `false` for the two boolean production INI
settings without changing runtime behavior.

## Established evidence and current state

- Production 0.6.0 behavior is runtime-validated on Steam 2.0.5.
- The distributed INI and the ASI's auto-generated INI template must remain
  semantically synchronized.

## Approved scope

- Add plain-language `true`/`false` explanations under `[Gameplay] Enabled`
  and `[Hotkeys] Enabled`.
- Update the release INI, embedded template source and release archive.
- Rebuild only to embed the comment-only template change.

## Explicit non-goals

- No runtime logic, defaults, resolver, hooks or FOV behavior changes.
- No research instrumentation or macro cleanup.
- No new runtime research or publication.

## Files/areas

- `release-assets/STALKER2CameraTweaks.ini`
- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- production `STALKER2CameraTweaks.asi` and 0.6.0 archive

## Batches and validation

1. Update both comment surfaces; verify only comments changed semantically.
2. Rebuild with the established production build command; verify the binary
   still contains the expected production markers and no research markers.
3. Refresh the release archive; verify exact contents and extracted ASI/INI.
4. Perform read-only Git review and report that the rebuild is not separately
   runtime-tested unless the user runs it.

## Risks and rollback

- Comment-only source changes must not alter parsed defaults. If build output
  or markers differ unexpectedly, retain the prior A0A5 artifact and stop.
- Preserve all unrelated user changes and historical artifacts.

## Stop conditions

- Stop on compile/link failure, unexpected behavior markers, changed defaults,
  archive contamination or SHA/provenance ambiguity.

## Final review

- Confirm the two boolean explanations match in source-generated and packaged
  INI files, then review changed paths with `git status` and `git diff --check`.
