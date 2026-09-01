# MOD_RUNTIME_IDENTITY_LOGGING

Status: Complete. Runtime logs confirmed uppercase SHA-256 identities for the
loaded ASI and 2.0.4 game executable.

## Objective

Record reproducible SHA-256 identities in the unified ASI startup log so
support reports can distinguish the installed mod build from the game build.

## Approved scope

- Hash the loaded ASI file and the loaded game executable at initialization.
- Log both hashes before hook installation.
- Fail safely to an explicit unavailable marker if hashing cannot complete.
- Do not change hooks, feature behavior, configuration or game state.

## Non-goals

- No network checks, update system, telemetry or persistence.
- No release upload or Git state changes.

## Expected files

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- its build script;
- this plan.

## Batches and validation

### Batch 1 — implementation

Use Windows CNG SHA-256 over the ASI and executable files and emit uppercase
hexadecimal hashes in the existing startup log.

Completed: the unified ASI now logs `modSha256` and `gameSha256` at startup.
Hash failures are represented as `unavailable` and do not block installation.
The initial runtime showed both values as unavailable; the file reader was
reworked to use Win32 `ReadFile` with the same fail-safe behavior.

### Batch 2 — build validation

Build the unified ASI and verify the output exists. Hashing remains runtime
validated only when the user launches the artifact.

Completed: build succeeded with the CNG `bcrypt.lib` dependency after the
file-reader correction. Runtime hash availability remains to be confirmed by
the next user launch.

### Batch 3 — runtime confirmation — COMPLETED

Completed: the user launch logged 64-character uppercase SHA-256 values for
both the loaded ASI and game executable while resolver installation remained
successful.

## Risks and safe failure

Hashing is read-only. File-open, CNG or read failures must not block hook
installation; the log should identify the hash as unavailable.

## Stop conditions and final review

Stop on build failure, unavailable hashes after the corrected reader, or any
unrelated behavior/source change. Review the bounded diff and confirm no
stable gameplay source was modified.
