# Cinematic Axis-Constraint — Track A1 Metadata Fingerprint

Status: `COMPLETE / LEAD ONLY — native registration unresolved`.

## Scope

Perform the cheap current-build metadata check and a bounded executable
string-family probe for the reflected `CameraComponent` setter family. No
native address was guessed, no function was hooked and no memory was written.

## Current-build identity

- Executable: Steam 2.0.4
- SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- `.text`: RVA `0x1000`, virtual size `130818504`, raw size `130818560`
- Identity: `PASS` from the established current-build gate.

## Cooked ScriptObjects metadata query

The `global.utoc` ScriptObjects data contains these reflected names:

- `EAspectRatioAxisConstraint`
- `SetAspectRatioAxisConstraint`
- `SetAspectRatio`
- `SetConstraintAspectRatio`
- `SetFieldOfView`

The query did not return separate ScriptObjects for the expected enum
enumerators `AspectRatio_MaintainYFOV`, `AspectRatio_MaintainXFOV` or
`AspectRatio_MajorAxisFOV`. Therefore the metadata confirms the enum type and
function vocabulary, but does not expose enumerator identity or native
registration details by itself.

## Executable family probe

The exact reflected strings occur once each in the current executable at the
following file offsets:

```text
SetFieldOfView:              154299813
SetAspectRatioAxisConstraint:154378338
SetConstraintAspectRatio:    155065329
SetAspectRatio:              155065392
```

These are file offsets of string data, not function RVAs or callable native
addresses. Their presence provides a compact family fingerprint for the next
correspondence step. A string match or proximity relationship alone is not
native registration proof.

## Result and next gate

```yaml
Track A1: COMPLETE / LEAD ONLY
Reflected family vocabulary: CONFIRMED
Enum enumerator metadata: NOT EXPOSED IN QUERY
Native registration record: NOT FOUND
Native thunk/implementation: UNRESOLVED
Field offsets: NOT ESTABLISHED
Runtime read/write: NOT AUTHORIZED
```

The next bounded step is to inspect current-build native registration
correspondence around this family, using the strings only as anchors and
requiring a decoded registration/thunk structure before promoting any setter
or field offset. Do not infer a function address from these file offsets and
do not read presumed UE object layout.
