# Native Gameplay Aspect Reevaluation — Coordinator Caller Audit

Date: 2026-09-10  
Scope: one-level read-only audit above `FUN_1453BA300` on Steam 2.0.4

## Identity gate

```text
Executable SHA-256: 2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409
Identity: PASS
```

## Result

The bounded audit targeted coordinator entry `FUN_1453BA300` and found no
direct code references to its entry point. It found two data references only:

```text
0x14B46D748 — DATA
0x1484A6728 — DATA
```

Therefore no direct caller function or callsite can be promoted from this
pass. The result is consistent with `FUN_1453BA300` being reached through an
indirect/vtable dispatch or another function-pointer table, but that
interpretation is not promoted until the two data references are classified.

## Established chain remains valid

The earlier direct-caller audit still confirms:

```text
FUN_1453BA300 + 0x22  →  FUN_140AF4022
FUN_1453BA300          →  [RDI.vtable + 0x638]
```

Within `FUN_1453BA300`, `RDI` is the source object and `RSI`/`R8` is the output
object. The native runtime trace independently showed the same source/output
pair at the writer during A→B→C.

## Bounded classification of the two data references

```text
0x14B46D748:
    block = .pdata
    type  = IMAGE_RUNTIME_FUNCTION_ENTRY
    begin = 0x1453BA300
    end   = 0x1453BA465
```

This is unwind metadata for `FUN_1453BA300`, not a function-pointer table and
not a caller.

```text
0x1484A6728:
    block = .rdata
    target = FUN_1453BA300
```

Its surrounding entries form a dense function-pointer sequence:

```text
0x1484A6720  FUN_14559ABC2
0x1484A6728  FUN_1453BA300
0x1484A6730  FUN_1453BA63A
0x1484A6738  FUN_1453BA466
0x1484A6740  FUN_1453BBC3A
```

Indirect/function-table reachability is therefore strongly supported. This
does not prove vtable identity or identify the owner/type.

## Evidence status

```text
Direct code caller of FUN_1453BA300: NOT OBSERVED
Data references to FUN_1453BA300: CONFIRMED x2
First reference: .pdata unwind metadata
Second reference: dense .rdata function-pointer sequence
Indirect/function-table reachability: STRONGLY SUPPORTED
Vtable identity: NOT PROVEN
Owner/type association: UNRESOLVED
Settings-side caller: UNRESOLVED
Transition-specific owner: UNRESOLVED
```

## Stop and next gate

The two-reference classification pass is complete. Continue only if a narrow
owner/type association can be recovered from the `.rdata` table itself;
otherwise close the upstream provenance branch as deferred. Do not broaden
into a general `.rdata`, settings or renderer scan.
