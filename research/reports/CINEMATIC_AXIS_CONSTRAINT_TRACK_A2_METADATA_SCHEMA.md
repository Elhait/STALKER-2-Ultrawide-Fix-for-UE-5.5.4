# Cinematic Axis-Constraint — Track A2 Metadata and Schema Routes

Status: `A2.2 COMPLETE / BOUNDED NEGATIVE`; `A2.3 COMPLETE / NO MATCHING SCHEMA FOUND`.

## Identity and scope

The query was limited to the known Steam 2.0.4 `global.utoc` ScriptObjects
container and workspace file names. No package was repacked, no asset was
modified, no runtime tracer was created and no production source was changed.

- Executable SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- `.text`: RVA `0x1000`, virtual size `130818504`, raw size `130818560`
- Identity: `PASS` from the established current-build gate.

## A2.2 — Property-level metadata

The bounded ScriptObjects query returned the reflected type/class vocabulary
including `CameraComponent`, `LocalPlayer` and `EAspectRatioAxisConstraint`,
plus `SetAspectRatioAxisConstraint`. It did not return separate property
objects or exact outer/index records for:

- `AspectRatioAxisConstraint`
- `bOverrideAspectRatioAxisConstraint`
- `LocalPlayer::AspectRatioAxisConstraint`

Therefore the query does not establish a property descriptor, native field
offset, override flag offset or effective owner. The enum type remains a
useful correspondence anchor, but enumerator metadata is not required for
this route and was not promoted.

## A2.3 — Matching schema search

The bounded workspace search found no `.usmap` or current-build unversioned
property schema with clear STALKER 2 Steam 2.0.4 provenance. It found only
generic tool source/object files and unrelated historical mapping reports.
Those files do not establish a usable property layout for this executable
and were not promoted.

## Decision

```yaml
A2.2 property metadata: COMPLETE / BOUNDED NEGATIVE
A2.3 external schema route: COMPLETE / NO MATCHING SCHEMA
Property offset: UNRESOLVED
Override flag offset: UNRESOLVED
Current-build schema: NOT FOUND
Runtime read/write: NOT AUTHORIZED
```

The remaining A2 route is A2.1: a class-level native registration fingerprint
using multiple reflected `CameraComponent` siblings. It must stop if no
shared registration structure appears within the bounded family search. A
string match, generic tool mapping or historical report is insufficient.
