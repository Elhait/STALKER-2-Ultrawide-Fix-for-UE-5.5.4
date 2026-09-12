# UE4SS Cinematic Architecture Feasibility

Date: 2026-09-10

## Scope

Read-only architecture audit of the installed UE4SS environment for a future
cinematic subsystem. The stable Camera/FOV ASI, release assets and runtime
game state were not changed.

## Capability matrix

| Capability | Status | Boundary |
|---|---|---|
| Live Camera/CameraManager/PCM discovery | PASS | Confirmed through existing UE4SS observers |
| Reflected state observation | PASS | Camera and PCM lifecycle fields are readable |
| Lifecycle observation | PASS | Timers, keybinds and change observers are available |
| Reflected property mutation | AVAILABLE | Storage mutation does not prove downstream rebuild |
| Value-only reflected calls | AVAILABLE / CONDITIONAL | `SafeObject.call` exists; only understood calls are admissible |
| Complex ref/out invocation | BLOCKED | No confirmed construction, packing or readback contract |
| Native camera reevaluation | NOT PROVEN | No safe callable boundary established |
| Downstream projection rebuild | NOT PROVEN | Storage mutation alone was insufficient in the tested probe |
| Deterministic native-like cinematic refresh | BLOCKED / UNPROVEN | Missing reevaluation primitive |

## Findings

- UE4SS is a viable platform for observing live ownership and cinematic state.
- Direct reflected storage mutation is confirmed, but storage mutation alone
  causing the required downstream reevaluation was not observed.
- `GetCameraView` and `BlueprintUpdateCamera` are metadata-positive candidates,
  but their complex ref/out invocation contract is not established.
- `CallFunctionsByHandle` is exposed only through an opaque wrapper; no
  source-backed ABI, parameter packing, lifetime or readback rules were found.
- Moving the module to UE4SS would therefore not remove the fundamental missing
  native reevaluation primitive.

## Final disposition

```text
UE4SS observer/state architecture: FEASIBLE
UE4SS cinematic native-refresh architecture: BLOCKED / UNPROVEN
Production migration: DEFERRED / NOT JUSTIFIED
```

The cinematic research branch should reopen only with a new evidence class:
documented ref/out call support, a safe value-only reevaluation function, a
runtime lifecycle event that performs the rebuild, engine/source evidence for a
committing setter/update function, or a new current-build native anchor with
proven ownership and callable semantics.

## Non-goals and limits

- No replacement UE4SS module was implemented.
- No undocumented handle invocation was attempted.
- No runtime writes, suppression tests, renderer scans or production changes
  were performed.
