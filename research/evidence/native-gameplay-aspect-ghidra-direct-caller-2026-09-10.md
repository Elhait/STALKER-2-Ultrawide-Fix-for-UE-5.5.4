# Native Gameplay Aspect Reevaluation — Ghidra Direct-Caller Evidence

Date: 2026-09-10  
Scope: current Steam 2.0.4 direct-caller audit only  
Ghidra script: `AuditCameraWriterCallers204.java`

## Identity gate

```text
Executable SHA-256: 2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409
.text size: 130818560 bytes
ImageBase: 0x140000000
Identity: PASS
```

The canonical Ghidra project was opened read-only. Non-canonical 2.0.2,
2.0.3 and v1.9 entries were explicitly skipped by the script.

## Target and direct references

```text
Target: FUN_140AF4022
Code direct caller:
    FUN_1453BA300 + 0x22
    call site RVA 0x53BA322

Additional references:
    0x14B1DC778 — DATA
    0x14866CCD8 — DATA
```

Only `0x1453BA322` is a code call to the validated writer. The two data
references are not promoted as callers.

## Caller context

The bounded decompiled/disassembly context establishes:

```text
0x1453BA30B  MOV RSI,R8
0x1453BA311  MOV RDI,RCX
0x1453BA314  CALL 0x1432DE848
0x1453BA319  MOV RCX,RDI
0x1453BA31F  MOV R8,RSI
0x1453BA322  CALL 0x140AF4022
0x1453BA327  MOV RAX,[RDI]
0x1453BA32D  MOV R8,RSI
0x1453BA333  CALL [RAX+0x638]
```

This confirms the immediate object-identity bridge:

```text
RCX/RDI = source object
R8/RSI  = output object
source/output
    → FUN_140AF4022
    → same source/output pair to virtual slot +0x638
```

## Evidence status

```text
Writer direct code caller: CONFIRMED
Writer callsite RVA: CONFIRMED (0x53BA322)
Source/output register provenance: CONFIRMED
Same-pair downstream virtual dispatch: CONFIRMED
Settings-side caller: NOT IDENTIFIED
Transition-specific branch in FUN_1453BA300: NOT PROVEN
Complete reevaluation chain: UNRESOLVED
```

This pass establishes the immediate writer owner/boundary, not the origin of
the settings-driven A→B→C transition. Runtime evidence separately confirms
that the writer sees the transition states on stable source/output objects.

## Stop and next gate

The direct-caller layer is complete. Continue only with a one-level bounded
caller audit of `FUN_1453BA300` if upstream provenance is still required.
Do not scan the general settings system or renderer without a new anchor.
