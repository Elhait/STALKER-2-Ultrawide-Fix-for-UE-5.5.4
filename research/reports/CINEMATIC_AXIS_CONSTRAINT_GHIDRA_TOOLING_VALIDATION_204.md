# Cinematic Axis-Constraint — Ghidra Tooling Validation 2.0.4

Status: `INVOCATION PASS / PROGRAM DATABASE INCOMPLETE FOR A2.1`.

## Scope

Validate the headless invocation before repeating A2.1. The control script
uses progress markers and performs only one exact reflected string lookup,
one known current-build anchor lookup and bounded xref/containing-function
queries. It does not modify the project or executable.

## Results

```yaml
Canonical program selection: PASS
Selected program: Stalker2-Win64-Shipping.exe (2.0.4)
SHA-256: 2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409
Image base: 0x140000000
Known anchor address: 0x146B7CB05
Exact string lookup: PASS
String xref enumeration: PASS (count=1)
Containing-function query: COMPLETE (count=0)
Script completion: PASS
```

The script emitted all expected markers through `[COMPLETE]` and exited
normally. The wildcard was necessary to select the exact `2.0.4` program;
the project also contains 2.0.2, 2.0.3 and v1.9 programs.

## Database limitation

For the selected 2.0.4 program, the known anchor and the string xref were
located, but neither had a containing Ghidra function. This indicates that
the saved program database has incomplete or absent function boundaries for
the locations needed by A2.1. It does not indicate that the native
registration structure is absent.

Therefore the invocation is validated, but the current project state is not
yet a sufficient evidence base for native-registration correlation.

## Decision

```yaml
Tooling mini-pass: PASS
Current-build identity: PASS
Exact 2.0.4 selection: PASS
Function/xref database readiness: BLOCKED
A2.1 registration correlation: NOT STARTED
Registration absence: NOT PROVEN
Runtime reads/writes: NOT AUTHORIZED
Production source: UNCHANGED
```

Next bounded action is to prepare or verify a fully analyzed current 2.0.4
Ghidra program, then rerun the same minimal control query before A2.1. Do not
run A2.1 against this incomplete database and do not broaden the search.
