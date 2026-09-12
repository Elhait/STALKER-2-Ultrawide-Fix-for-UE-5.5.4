# Unified ASI Build Wiring — Task Plan

## Objective

Wire the current unified gameplay/cinematic/dialogue implementation into the
canonical build command and produce the documented `STALKER2CameraTweaks.asi`.

## Established evidence and current state

- The repository contains the current unified implementation in
  `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- The repository README documents `STALKER2CameraTweaks.asi` as the unified
  output.
- `build.cmd` still compiles only the superseded gameplay-only source and
  outputs `STALKER2GameplayAspectFix.asi`.

## Approved scope

- Change only the canonical build command's source and output wiring.
- Do not alter gameplay/cinematic implementation logic.
- Do not remove historical research sources or artifacts.

## Explicit non-goals

- No runtime game validation.
- No executable-version claims.
- No production installation or release upload.
- No deletion or cleanup.

## Expected files or areas

- `build.cmd`
- `STALKER2CameraTweaks.asi` build output if generated
- `backlog/TASKLOG.md`

## Batches and validation

### Batch 1 — build wiring

- Point `build.cmd` at the unified source and documented output name.

Validation: inspect the command and run the bounded local build if the Visual
Studio environment is available.

### Batch 2 — review

- Perform read-only Git status/diff review against this plan.

## Risks and safe failure

- The unified source is version-specific; build success does not prove runtime
  compatibility.
- If compilation fails, preserve the source and report the exact build error;
  do not fall back silently to the gameplay-only artifact.

## Stop conditions

- Stop if the unified source cannot compile without unrelated source changes.
- Stop before any game launch or installation.
