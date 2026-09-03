# Configurable hotkey bindings — task plan

## Objective

Make the `CinematicCycle` and `DialogueCycle` INI values control the actual
runtime hotkeys, while preserving the existing `F9`/`F10` defaults.

## Established evidence and current state

- `F9` and `F10` currently work when hard-coded.
- The INI contains `CinematicCycle` and `DialogueCycle`, but the runtime loop
  reads only `VK_F9` and `VK_F10`.
- A test with `DialogueCycle=F8` confirmed the configured binding is ignored.

## Approved scope

- Parse supported function-key names from the `[Hotkeys]` section.
- Use the parsed virtual-key codes in the existing edge-triggered hotkey loop.
- Keep defaults `CinematicCycle=F9` and `DialogueCycle=F10`.
- Preserve current persistence and next-lifecycle semantics.

## Explicit non-goals

- No changes to dialogue/cinematic policy algorithms or resolver hooks.
- No new hotkey families, overlay, input library or key rebinding UI.
- No changes to unrelated configuration values or game files.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- Generated/config documentation only if parsing behavior needs clarification.

## Batches and validation

1. Add safe function-key parsing and use configured virtual-key codes.
2. Build the unified ASI and inspect the result.
3. Perform read-only diff/status review against this plan.
4. Runtime validation remains separate: test default F9/F10 and a changed
   binding such as `DialogueCycle=F8`.

## Risks and rollback/safe-failure

- Unsupported or invalid key names fall back to the documented defaults.
- Invalid configuration must not disable policy handling or alter resolver
  safety.
- Rollback is limited to the source diff; no Git history mutation is allowed.

## Stop conditions and phase gates

- Stop if arbitrary key parsing requires broader input-hook changes.
- Stop before claiming runtime success until a changed binding is tested in-game.

## Expected final Git review

Review changed paths and diff check; separate build success from runtime
validation and preserve unrelated user changes.
