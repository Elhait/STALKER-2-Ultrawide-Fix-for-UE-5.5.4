# Configuration template synchronization — task plan

## Objective

Make the unified ASI automatically synchronize the managed descriptions and
hotkey entries in an existing `STALKER2UltrawideFix.ini` while preserving the
user's selected values and unrelated settings.

## Established evidence and current state

- The source creates the expanded template only when the INI does not exist.
- Existing INIs are currently parsed, but their comments and missing managed
  sections are not synchronized.
- The live INI was manually updated and confirmed to contain the intended
  `[Dialogue]` and `[Hotkeys]` documentation.

## Approved scope

- Update configuration synchronization in the unified ASI source.
- Preserve existing values for `Gameplay.Enabled`, `Cinematics.AspectRatio`,
  `Dialogue.Zoom` and `Hotkeys.Enabled` when valid.
- Add or refresh only the managed template comments and hotkey keys.

## Explicit non-goals

- No resolver, hook, dialogue classifier, cinematic behavior or hotkey behavior
  changes.
- No game launch or injected runtime validation in the implementation batch.
- No changes to archives, Ghidra projects or release packaging.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- Existing INI template/synchronization helpers near `LoadFeatureConfig`.

## Batches and validation

1. Implement a bounded managed-template synchronization helper and call it
   after loading the existing file.
2. Build the unified ASI and inspect compiler output.
3. Perform read-only diff/status review against this plan.

## Risks and rollback/safe-failure

- Risk: overwriting user comments or values. Mitigation: replace only known
  generated comments/keys and preserve all values and unknown lines.
- If synchronization cannot safely read or write the file, keep current runtime
  behavior and log the failure; do not replace the whole file.
- Rollback is the normal source diff reversal after review; no Git history
  mutation is authorized.

## Stop conditions and phase gates

- Stop if preserving existing values cannot be guaranteed.
- Stop if the change requires modifying runtime behavior or resolver contracts.
- Stop before runtime validation unless separately requested.

## Expected final Git review

Review status, diff and changed paths; confirm only the approved source and plan
paths changed, and report build/runtime-validation limits separately.
