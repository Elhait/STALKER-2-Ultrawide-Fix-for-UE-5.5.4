# Cinematic Axis-Constraint — Track A2.1 Registration Attempt

Status: `BLOCKED / TOOLING TIMEOUT — NOT A NEGATIVE RESULT`.

## Scope

A single read-only Ghidra pass was prepared to correlate the four reflected
`CameraComponent` sibling names as a bounded registration family:

```text
SetAspectRatioAxisConstraint
SetAspectRatio
SetFieldOfView
SetConstraintAspectRatio
```

The pass was limited to string references and containing functions. It did
not authorize renames, program changes, hooks, memory writes or production
source changes.

## Execution

- Current-build identity gate: previously `PASS` for Steam 2.0.4.
- Ghidra mode: read-only, no-analysis.
- Agent-owned process: PID `22896`.
- Result: the `cmd` wrapper remained running for more than the bounded timeout;
  no result log was produced and no registration finding was obtained.
- Cleanup: only tracked PID `22896` was terminated normally; no workspace lock
  files remained afterward.

## Decision

This is an instrumentation/tooling timeout, not evidence that a shared
registration structure is absent. A2.1 remains unresolved and must not be
promoted to `NEGATIVE` until the same bounded query completes or a separate
valid evidence route closes it.

```yaml
A2.1: BLOCKED / TOOLING TIMEOUT
Shared registration structure: NOT OBSERVED
Registration absence: NOT PROVEN
Native thunk: UNRESOLVED
Field offset: UNRESOLVED
Runtime read/write: NOT AUTHORIZED
```

Next action requires a corrected, bounded Ghidra invocation against the
canonical project (or an independently verified current-build project
selection). Do not broaden the family, scan the renderer or infer native
addresses from string file offsets.
