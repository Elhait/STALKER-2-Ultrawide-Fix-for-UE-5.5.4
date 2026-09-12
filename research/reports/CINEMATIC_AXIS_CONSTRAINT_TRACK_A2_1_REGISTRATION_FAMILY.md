# Cinematic Axis-Constraint — Track A2.1 Registration Family

Status: `PASS — shared CameraComponent registration family established`.

## Identity and method

- Program: `Stalker2-Win64-Shipping.exe (2.0.4)`
- SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- Method: read-only Ghidra query on the exact 2.0.4 program; no analysis,
  decompilation fan-out, renames, hooks or writes.

The query followed the four reflected sibling-name data references and dumped
only their bounded surrounding qwords. The table has a reproducible alternating
shape:

```text
pointer to reflected name string
pointer into executable .text
```

## Independently corroborated bindings

```yaml
SetAspectRatio:
  name_pointer_slot: 0x14866BCB0
  native_target: 0x145599FAA

SetAspectRatioAxisConstraint:
  name_pointer_slot: 0x14866BCC0
  native_target: 0x14559A03E

SetConstraintAspectRatio:
  name_pointer_slot: 0x14866BCF0
  native_target: 0x14559A1F2

SetFieldOfView:
  name_pointer_slot: 0x14866BD10
  native_target: 0x14559A324
```

All four name slots and their executable targets occur in the same compact
registration-family region around `0x14866BCB0–0x14866BD18`. The target
addresses are corroborated `.text` pointers with current-build function
coverage where available.

## Evidence boundary

This satisfies the A2.1 PASS condition:

- multiple reflected `CameraComponent` sibling names: `PASS`;
- one shared registration/data region: `PASS`;
- reproducible name→native-pointer structure: `PASS`;
- at least 2–3 independent bindings: `PASS` (four established).

The targets are native registration targets/thunks at this stage. This result
does not yet prove that any target directly writes
`AspectRatioAxisConstraint` or `bOverrideAspectRatioAxisConstraint`. It also
does not establish effective cinematic ownership or authorize runtime writes.

## Phase decision

```yaml
A2.1 registration family: PASS
CameraComponent family anchor: CONFIRMED
SetAspectRatioAxisConstraint target: CONFIRMED
Setter field write: UNRESOLVED
Property offsets: UNRESOLVED
Effective reader/owner: UNRESOLVED
Runtime reflection tracer: BLOCKED pending setter/read analysis
```

The next bounded step is A2.2 setter-path decoding for the four confirmed
targets, beginning with `SetAspectRatioAxisConstraint`, and then a narrow
reader audit if a field write is found. Do not infer field offsets from the
registration table and do not write runtime state.
