# Cinematic Axis-Constraint Runtime Reflection — Track B

Status: `COMPLETE — provenance established; UE object type unresolved`.

## Scope

Characterize only the already-known live source pointer passed through the
validated Steam 2.0.4 camera writer. This is a static provenance result; it
does not add a tracer, install a hook, write memory or modify production
behavior.

## Identity gate

- Executable: Steam 2.0.4
- Expected SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- `.text`: RVA `0x1000`, virtual size `130818504`, raw size `130818560`
- Known anchors: cinematic aspect setter `0x6B7CB05`; live-FOV ENTER
  `0x2EE6936`; live-FOV EXIT `0x2EE69A7`
- Identity: `PASS` for the previously validated writer evidence.

## Confirmed writer data flow

The complete camera-writer signature is not a generic `MOVSS` match. It
includes the source reads, output writes and flag handling, and is validated
by decoding `MOVSS [RBX+0x30], XMM0` at the resolved site.

- `RSI` is the source pointer received by the writer.
- `RBX` is the output view-state pointer used by the writer.
- Source primary FOV is read from `+0x230`.
- Source secondary FOV is read from `+0x234`.
- Source aspect is read from `+0x254`.
- Source flags are read from `+0x259`.
- Source selector/state byte is read from `+0x262`.
- Output FOV is written at `+0x30`.
- Output aspect is written at `+0x5C`.

The existing diagnostic path also observes this same source pointer around
the manual-transition replay and cinematic handoff lifecycle. This confirms
that the pointer is a meaningful game-owned camera/source state input to the
view-copy path, rather than an arbitrary pointer obtained from a scan.

## What this does and does not establish

Confirmed:

- known live source pointer: `CONFIRMED`
- source-pointer role: camera/FOV/aspect state input to the validated writer
- source-to-output copy relationship: `CONFIRMED`
- current-build executable identity for this evidence: `PASS`

Unresolved:

- UE reflected class identity: `UNRESOLVED`
- `UCameraComponent` or subclass relation: `UNRESOLVED`
- `FMinimalViewInfo` relation: `UNRESOLVED`
- vtable/type identity: `UNRESOLVED`
- producer chain and effective axis-constraint owner: `UNRESOLVED`

The source pointer must not be promoted to a `UObject`, `UCameraComponent`
or reflected property owner based only on its role and offsets. No
`UObject + 0x10` read, presumed UE layout read, generic pointer scan or
pointer enumeration is authorized by this result.

## Phase decision

Track B is complete as a bounded provenance characterization. It does not
provide the safe UE reflection bridge required for the diagnostic tracer.
Therefore:

```yaml
Track B: COMPLETE / TYPE UNRESOLVED
Known writer source state: CONFIRMED
CameraComponent relation: UNRESOLVED
Effective axis owner: UNRESOLVED
Reflection tracer: BLOCKED
MaintainYFOV write/test: NOT AUTHORIZED
Production Full Hor+: UNCHANGED
```

The next permitted progress is Track A native correspondence for the
reflected CameraComponent setter family, or a genuinely new independent
evidence class. A broader pointer scan or another guessed-layout pass is
explicitly out of scope.
