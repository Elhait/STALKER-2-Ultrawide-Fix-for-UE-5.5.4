# Post-EXIT FOV handoff — static audit for Steam 2.0.5

Date: 2026-09-11  
Scope: bounded read-only Ghidra audit of the current cinematic EXIT boundary.  
Executable: `Stalker2-Win64-Shipping.exe (2.0.5)`

## Identity gate

| Field | Expected | Observed | Status |
|---|---|---|---|
| Image SHA-256 | `E7B481A97C02D80581FAB0BECE940214A88EBE30211088A00129845A039F9293` | `E7B481A97C02D80581FAB0BECE940214A88EBE30211088A00129845A039F9293` | PASS |
| Image base | `0x140000000` | `0x140000000` | PASS |
| `.text` size | `0x7CCD000` | `0x7CCD000` | PASS |
| `.text` SHA-256 | — | `66D5289B1DBC34BE4265B7C083D664F7CAA7076EA5583F20DC9D5CA9E01445F4` | recorded |

The executable identity gate passed before interpreting the result. The existing
2.0.2, 2.0.3 and 2.0.4 project programs were also processed by the headless
runner, but were rejected by the 2.0.5 identity gate and contributed no evidence.

## Current EXIT boundary

The current indexed EXIT topology from the production resolver was found exactly
once:

```text
match RVA:       0x366FA2D
function:        FUN_14366F9AA
entry:           0x14366F9AA
end:             0x14366FAF8
```

The boundary begins by loading an indexed FOV value:

```text
MOVZX  EAX,DIL
MOVSS  XMM0,[RBX + RAX*4 + 0x38]
CALL   0x146B6777C
```

The same function then performs a guarded state transition on `param_1 + 0xAC`.
When the state becomes false it selects an indexed value from a global table,
calls the same FOV consumer, resolves a camera object, and invokes virtual slots
`+0x880` and `+0x868`. When the state becomes true it instead uses a global
scalar and invokes slots `+0x878` and `+0x860`. Both branches finish through a
state-dependent call to `FUN_1426E0E5E` followed by a small state finalization
call.

The decompiler therefore supports this bounded classification:

```text
FUN_14366F9AA
  +0xAC state transition
  → choose indexed/global FOV source
  → FUN_146B6777C shared FOV consumer
  → branch-specific virtual camera update
  → state/transition finalization
```

## Direct callers

Ghidra reports four direct references to the boundary:

```text
FUN_1434F8B6C  call 0x1434F8C94
FUN_146B68018  call 0x146B68059
FUN_14366F950  call 0x14366F9A5
<undefined>    call 0x143F6E2B5
```

The static pass did not yet classify these callers as cinematic-only or
post-EXIT-only. No claim is made that `FUN_14366F9AA` itself owns the complete
one-second interpolation; it is the confirmed current EXIT state/FOV handoff
boundary and a valid anchor for the next bounded runtime trace.

## Status

```text
Identity gate:                         PASS
Current indexed EXIT signature:        1 match / PASS
EXIT boundary function:                CONFIRMED
State transition +0xAC:                CONFIRMED
FOV source selection:                  CONFIRMED
Shared FOV consumer call:              CONFIRMED
Branch-specific camera virtual stage:  CONFIRMED
Interpolation target/progress/duration: UNRESOLVED
Immediate settings-side origin:        OUT OF SCOPE
```

`POST_EXIT_FOV_HANDOFF_UPSTREAM_TRACE` static gate: **PASS — new upstream
state/FOV handoff boundary found**.

Next bounded step: one runtime trace around the first few FOV changes after the
validated EXIT, correlated with `FUN_14366F9AA`/its four direct callsites if a
safe observation point is available. Do not add timers, clamps, writes, or
production behavior changes.
