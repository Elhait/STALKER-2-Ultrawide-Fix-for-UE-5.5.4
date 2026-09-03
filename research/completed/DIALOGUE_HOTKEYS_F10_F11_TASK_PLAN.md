# F10/F9 hotkey integration task plan

## Objective

Add production runtime hotkeys to the unified ASI: `F10` cycles dialogue zoom
policy (`Native → Reduced → Disabled`) and `F9` cycles cinematic policy for the
next cinematic (`Auto → Native → 16:9 → 21:9 → 32:9`).

## Established evidence and current state

- Dialogue production core is runtime-validated on canonical Steam 2.0.4 for
  `Native`, `Reduced` and `Disabled`.
- Cinematic aspect/FOV integration is already validated and must remain unchanged.
- The previous test-only dialogue toggle used `F7`; production bindings are now
  explicitly `F10` and `F9`.

## Approved scope

- Add an opt-out `[Hotkeys] Enabled` setting.
- Add F10/F9 edge-triggered runtime policy cycling.
- Diagnose and repair INI persistence, preserving unrelated content.
- Record that F9 intentionally applies to the next cinematic, not an already
  active cinematic.
- Keep the active policy synchronized with the existing unified configuration.
- Persist the selected policy to the existing INI without creating another file.

## Explicit non-goals

- No changes to dialogue math, lifecycle classifier, resolver signatures or
  cinematic coordinator behavior.
- No F5/F6/F7 bindings in the production hotkey layer.
- Do not claim F9 changes an already active cinematic.
- No new runtime research tracer or camera writes.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `release-assets/STALKER2UltrawideFix.ini`
- this task plan

## Batches and validation

1. Diagnose/fix INI persistence and add F9/F10 runtime polling.
2. Confirm F9 is a next-cinematic selector; no live reapply is attempted.
3. Build the unified ASI and inspect changed paths/diff.
4. Runtime-check F10 inside an active dialogue, F9 before a new cinematic, and
   confirm cinematic behavior remains unchanged.

## Risks and safe-failure

- Hotkeys are edge-triggered and can be disabled through `[Hotkeys] Enabled=false`.
- Invalid policy or failed INI persistence leaves the current runtime policy active
  and does not alter native camera behavior.
- Existing resolver and native pass-through guards remain authoritative.

## Stop conditions

- Stop if the build changes dialogue/cinematic core behavior or if configuration
  persistence cannot preserve unrelated INI content.
- Do not expand into new FOV research during this task.

## Final review

- Run `git diff --check`, inspect status and diff summary, verify only approved
  paths changed, and record runtime validation limits before finalizing the task log.
