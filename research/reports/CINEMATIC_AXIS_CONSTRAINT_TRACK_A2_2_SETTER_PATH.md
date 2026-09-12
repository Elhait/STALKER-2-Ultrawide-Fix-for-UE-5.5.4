# Cinematic Axis-Constraint — Track A2.2 Setter Path

Status: `PASS — field writes and current-build ABI established`.

## Identity and method

- Program: `Stalker2-Win64-Shipping.exe (2.0.4)`
- SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- Method: bounded instruction-level decode of the four A2.1 targets.
- No decompiler fan-out, hooks, writes or production changes.

## ABI and layout observations

All four targets establish the same object-pointer convention:

```text
RCX → object/state pointer, copied to RSI
RDX → reflected value/argument path, copied to RDI
```

The sibling controls provide coherent current-build layout corroboration:

```yaml
SetAspectRatio:
  target: 0x145599FAA
  terminal_write: MOVSS [RSI + 0x254], XMM0
  classification: aspect float field

SetAspectRatioAxisConstraint:
  target: 0x14559A03E
  terminal_write: MOV [RSI + 0x258], AL (byte)
  classification: axis-constraint enum/value field candidate

SetConstraintAspectRatio:
  target: 0x14559A1F2
  terminal_write: read [RSI + 0x259], clear bit 0, OR computed AL,
    write [RSI + 0x259]
  classification: constraint-related flag bit; exact reflected property
    identity not yet proven

SetFieldOfView:
  target: 0x14559A324
  terminal_write: read [RSI + 0x262], clear bit 2, OR computed value << 2,
    write [RSI + 0x262]
  classification: FOV/selector state bits
```

The `SetAspectRatio` `+0x254` write and `SetFieldOfView` `+0x262` update
corroborate the already observed source-state offsets in the validated camera
writer. The new axis-policy field candidate is `+0x258`.

## Important boundary

`+0x258` is now a current-build field offset written by the reflected
`SetAspectRatioAxisConstraint` target. It is not yet promoted to a durable
`UCameraComponent` property offset or effective projection-policy owner.

`+0x259` is a constraint-related bitfield modified by
`SetConstraintAspectRatio`, but it must not be automatically named
`bOverrideAspectRatioAxisConstraint`. That identity requires independent
property metadata or reader semantics. The setter name alone is insufficient.

## Phase decision

```yaml
A2.2 setter-path decode: PASS
RCX object ABI: CONFIRMED
Axis field candidate: +0x258
Aspect field: +0x254
Constraint-related bitfield: +0x259 bit 0
FOV/selector field: +0x262 bit 2 path
bOverrideAspectRatioAxisConstraint identity: UNRESOLVED
Effective reader: UNRESOLVED
Runtime read/write: NOT AUTHORIZED
```

The next bounded phase is a narrow reader audit for `+0x258`, followed by
independent classification of reads of `+0x259`. Priorities are view/camera
construction and effective axis-policy selection; constructor, serialization
and generic setter machinery remain lower priority. Do not infer runtime
policy from the setter alone.
