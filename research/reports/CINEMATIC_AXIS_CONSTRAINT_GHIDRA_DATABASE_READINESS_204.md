# Cinematic Axis-Constraint — Ghidra Database Readiness 2.0.4

Status: `PASS — canonical database ready for bounded A2.1`.

## Inventory

The read-only inventory enumerated all existing program objects in the
canonical `STALKER2-Ghidra` project. No alternate 2.0.4 copy with a stronger
function database was present.

The current 2.0.4 program is:

```yaml
Program: /Stalker2-Win64-Shipping.exe (2.0.4)
SHA-256: 2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409
ImageBase: 0x140000000
FunctionCount: 412707
InstructionCount: 30458375
```

For comparison, the project also contains 2.0.2, 2.0.3 and v1.9 programs;
none is a same-SHA replacement for current 2.0.4.

## Anchor readiness

```yaml
Cinematic ENTER 0x142EE6936: PASS — enclosing function 0x142EE68DA
Cinematic EXIT  0x142EE69A7: PASS — enclosing function 0x142EE68DA
Aspect store    0x146B7CB05: instruction site; no enclosing function
```

The aspect store is a validated instruction/site anchor rather than a
function entry. Its missing enclosing function does not invalidate the
database as a whole. The independent ENTER/EXIT code anchors do have
function boundaries and satisfy the control requirement for proceeding.

## Decision

```yaml
Canonical 2.0.4 selection: PASS
Identity: PASS
Function database: READY
Code-anchor readiness: PASS
Alternate analyzed 2.0.4 copy: NOT FOUND
A2.1: UNBLOCKED
```

The next action is one bounded A2.1 multi-sibling registration-family query.
The aspect-store site remains an instruction-level reference; it must not be
used as a guessed function entry. No full re-analysis or alternate research
branch is justified by this audit.
