# Cinematic Axis-Constraint — Track A2.3 Reader Audit

Status: `PARTIAL PASS — known camera/view writer consumes +0x258; effective projection owner remains unresolved`.

## Identity and method

- Program: `Stalker2-Win64-Shipping.exe (2.0.4)`
- SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- `.text`: RVA `0x1000`, virtual size `130818504`, raw size `130818560`
- Identity: `PASS`
- Method: read-only instruction scan with an explicit object-relative filter;
  stack-local `[RSP + 0x258]`/`[RSP + 0x259]` matches were excluded.
- No decompiler, renames, hooks, writes or production changes.

## Confirmed known-writer bridge

The validated camera/view writer is reached through the known instruction
anchor `0x140AF4022`, inside current function `0x140AF3898`.

Within that function the source state is used as follows:

```text
0x140AF41A6  MOVSS XMM0, dword ptr [RSI + 0x254]
0x140AF41B3  MOVZX EDX, byte ptr [RSI + 0x259]
0x140AF41C8  MOVZX EDX, byte ptr [RSI + 0x259]
0x140AF42B9  TEST byte ptr [RSI + 0x259], 0x2
0x140AF42C0  JNZ  0x140AF4B88
0x140AF4B88  MOVZX EAX, byte ptr [RSI + 0x258]
0x140AF4B8F  MOV  dword ptr [RBX + 0x60], EAX
0x140AF4B92  MOV  byte ptr  [RBX + 0x64], 0x1
```

This is the strongest current result. It establishes that the byte written by
`SetAspectRatioAxisConstraint` at `source + 0x258` is not merely dead state or
reflection metadata: the known camera/view writer conditionally consumes it
when the source `+0x259` bit mask `0x2` is set and transfers it into the output
view-state region at `RBX + 0x60`, with an accompanying validity/presence byte
at `RBX + 0x64`.

The result does **not** yet identify the symbolic meaning of the `0x259` bit,
prove that `RBX + 0x60` is the final projection axis policy, or show the later
FOV/aspect conversion that produces the projection matrix.

## `+0x259` classification

The known writer reads `+0x259` as a byte and tests mask `0x2` immediately
before selecting the `+0x258` value. This supports the bounded classification
`constraint-related gate for axis-value propagation`.

It is not sufficient to rename the bit as
`bOverrideAspectRatioAxisConstraint` or `bConstrainAspectRatio`; the exact
reflected/property semantics remain unproven.

## Other direct hits

The identity-gated bounded scan found many additional object-relative accesses
to `+0x258` and a smaller number to `+0x259`. Most are writes, copies,
serialization-like transfers, address formation, or accesses to unrelated
structures that happen to share the same displacement. The scan alone does
not establish their object type or camera relevance. They are therefore not
promoted as projection consumers.

Some examples are retained as leads only:

```text
0x1401FA63C / 0x1401FA872  writes zero to [RSI + 0x258] — initialization/default lead
0x140280FC8                reads +0x259 and +0x258 together — bounded follow-up lead
0x140210A62                reads qword [RAX + 0x258] — object identity unresolved
```

The scan reached its bounded hit limit after 128 current-image hits. This is a
scope limit, not evidence that all readers were classified.

## Result and next gate

```yaml
A2.3a direct +0x258 readers: PARTIAL PASS
A2.3b +0x259 bit semantics: GATE OBSERVED, SYMBOLIC MEANING UNRESOLVED
A2.3c known camera/view writer consumes +0x258: PASS
A2.3d effective projection-policy owner: NOT ESTABLISHED
Runtime read-only observation: NOT YET AUTHORIZED
Runtime writes: PROHIBITED
Production Full Hor+: UNCHANGED
```

The next bounded action, if continued, is to inspect only the known writer's
output consumer and the one adjacent candidate that reads `+0x259` and
`+0x258` together. Do not launch a generic renderer scan or treat the output
field as a projection-matrix owner without a separate downstream proof.
