# UE4SS STALKER2CameraTweaks Port — Task Plan

## Objective

Create a separate UE4SS implementation named `STALKER2CameraTweaks` that
ports the proven gameplay, cinematic and dialogue behavior of the current
ASI where the UE4SS Lua surface supports it safely, while also serving as a
native-UE-facing prototype for future communication with GSC.

The existing `STALKER2CameraTweaks.asi` and its source remain the production
baseline and must not be modified by this task.

## Established evidence and current state

- The ASI production path has a validated gameplay two-pass aspect transition
  and preserves authored/player FOV.
- The ASI production path has working cinematic aspect/FOV policies and
  dialogue zoom policies, but its native cinematic reevaluation research is
  blocked/deferred.
- UE4SS reliably exposes the live `CameraComponent`, game-specific
  `Stalker2.CameraManager` bridge and `PlayerCameraManager` without stable
  numeric object IDs.
- UE4SS reflected property observation and mutation are available.
- Complex reflected calls requiring undocumented ref/out ABI handling are not
  safe or proven and are explicitly excluded from this port.
- A storage write alone has not been proven to trigger the required downstream
  projection reevaluation.

## Approved scope

- Add a separate UE4SS module at `research/ue4ss/STALKER2CameraTweaks/`.
- Keep the module name exactly `STALKER2CameraTweaks`.
- Port configuration-compatible gameplay, cinematic and dialogue policies.
- Discover the live camera ownership chain dynamically and re-arm after object
  replacement.
- Use bounded polling and lifecycle state transitions rather than hard-coded
  object IDs.
- Apply only safe, directly supported UE4SS property/setter operations and
  log whether the operation was accepted.
- Preserve authored FOV by default; make any calculated cinematic FOV change
  an explicit configurable policy matching the existing mod.
- Produce a standalone README and a test ZIP without deploying to the game.

## Explicit non-goals

- Do not edit or rebuild the stable ASI.
- Do not claim that a UE4SS property write performs native projection rebuild.
- Do not invoke `GetCameraView`, `BlueprintUpdateCamera` or any other function
  requiring undocumented complex ref/out argument packing.
- Do not use guessed pointers, fixed object IDs, raw offsets or native hooks.
- Do not add renderer/D3D12 work, ImGui, overlay work or weapon/viewmodel work.
- Do not deploy files into the installed game directory in this task.
- Do not remove existing research modules or change the global UE4SS module
  configuration.

## Expected files and areas

- `research/ue4ss/STALKER2CameraTweaks/Scripts/main.lua`
- `research/ue4ss/STALKER2CameraTweaks/README.md`
- `build-artifacts/test-ue4ss/STALKER2CameraTweaks.zip`
- This plan file during execution; archive it after the implementation review.

## Batches

### Batch 1 — Source/config contract inventory

Use the current ASI source, release INI and existing UE4SS observers to define
the port's configuration and runtime state machine.

Validation: compare the Lua configuration names and policy values with the
release INI and current source; no runtime claims are made.

### Batch 2 — UE4SS module implementation

Implement dynamic discovery, safe property access, gameplay/cinematic/
dialogue state detection, configurable policy application and change-only
logging. Keep unsupported native reevaluation paths disabled and explicit.

Validation: inspect the final module structure, run a Lua syntax check if a
Lua interpreter is available, and review the diff against this plan.

### Batch 3 — Packaging and static review

Create a test ZIP containing only the module files and document installation,
coexistence rules and the required ASI-off test condition.

Validation: list archive contents, verify no production ASI files are included,
perform read-only Git status/diff/path review, and record runtime validation as
not performed until the user tests in-game.

## Risks and rollback / safe-failure behavior

- UE4SS object/property names may vary across builds. Discovery and every
  access are protected by validity checks and `pcall`; failure leaves the game
  untouched and logs a bounded diagnostic.
- Camera objects can be replaced after loading or transitions. The module
  invalidates cached references and rediscovers them.
- A setter/property mutation may be accepted without rebuilding the active
  projection. The module logs this as an accepted write, not as proof of a
  visual fix.
- The UE4SS module must not run simultaneously with the production ASI during
  validation because both may change the same camera state. Rollback is simply
  disabling/removing the UE4SS module folder and its `mods.txt` entry.

## Stop conditions and phase gates

- Stop implementation expansion if a required operation depends on undocumented
  ref/out invocation or guessed native ABI.
- Do not add a new native-call or renderer branch under this task.
- Do not call the port production-ready without an in-game test log and visual
  evidence from gameplay, cinematic entry/exit and dialogue.
- If safe UE4SS operations cannot reproduce a behavior, retain the diagnostic
  state and mark that behavior `UNVALIDATED/BLOCKED`, rather than adding blind
  writes or timing hacks.

## Final review requirements

- Read-only Git review of status, diff summary and affected paths.
- Confirm the stable ASI source/release paths are untouched.
- Report completed, remaining, deferred, blocked and not-runtime-validated
  items separately.
- After review, move this plan to the appropriate research archive; do not
  leave an implementation plan in the repository root.
