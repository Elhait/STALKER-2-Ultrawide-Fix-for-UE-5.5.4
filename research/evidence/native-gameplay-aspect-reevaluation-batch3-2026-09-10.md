# Native Gameplay Aspect Reevaluation — Batch 3 Evidence

Date: 2026-09-10  
Scope: native writer correlation; read-only diagnostic ASI

## Result

The validated current-build camera writer was entered during both relevant
settings transitions. The same source/output pair was preserved:

```text
source = 0x21212A0D9C0
output = 0x20E8BA969C0
```

This establishes a concrete downstream native boundary correlated with the
known A→B→C transition. It does not identify the settings-side caller or prove
the complete upstream reevaluation chain.

## Correlated writer states

```text
A — 32:9 before:
    source FOV=90, aspect=3.5555556, flags=0x04
    output FOV=90, aspect=3.5555556, field68=0x4

B — 16:9 constrained:
    source FOV=90, aspect=1.7777778, flags=0x05
    output FOV=90, aspect=1.7777778, field68=0x5

C — post-reevaluation:
    source FOV=90, aspect=1.7777778, flags=0x04
    output FOV=90, aspect=1.7777778, field68=0x4
```

Each relevant sample contains both `pre-481A` and `post-481A` writer records.
The unrelated initial `FOV=120` sample is excluded.

## Evidence status

```text
Writer entry during A→B: CONFIRMED
Writer entry during B→C: CONFIRMED
Same source identity: CONFIRMED
Same output identity: CONFIRMED
State propagation at writer boundary: CONFIRMED
Settings-side caller: UNRESOLVED
Complete native reevaluation chain: UNRESOLVED
```

## Limits and safety

- The ASI installed only the validated writer trace.
- No camera/property writes were performed.
- Production CameraTweaks and release assets were untouched.
- The first return-address probe was invalid: `context.rsp` produced
  non-executable values (`0x0`, `0x4`, `0x20`). No callsite or upstream owner
  is promoted from that data.
- A corrected diagnostic build now records both SafetyHook stack pointers and
  their top values for a follow-up run; those fields are not yet validated.

## Next gate

Continue only with a bounded method that can recover the immediate caller or
settings-side owner for this same writer boundary. Do not broaden into a
general camera or renderer trace.
