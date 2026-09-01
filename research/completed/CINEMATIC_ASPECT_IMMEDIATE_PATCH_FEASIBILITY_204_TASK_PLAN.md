# Cinematic Aspect Immediate-Patch Feasibility 2.0.4 — Task Plan

Status: Complete for tested install-time aspect path — the isolated feasibility
artifact built and passed one 5120x1440 runtime test. Dynamic resolution and
FOV integration remain separate. Stable gameplay code remains untouched.

## Objective

Test whether replacing only the validated ENTER store's four-byte aspect
immediate can preserve native lifecycle/control flow while selecting the
current client-window aspect.

## Established evidence and current state

- Executable identity: 2.0.4, SHA-256
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Validated ENTER store: `RVA 0x6B7CB05`.
- Original bytes: `C7 80 54 02 00 00 39 8E E3 3F`.
- Only the immediate bytes `39 8E E3 3F` may change; this is
  `1.7777778f` in little-endian form.
- Runtime provenance is confirmed on the authoritative cinematic `inner` with
  pre-store aspect `3.5555556` and flags `0x05`.
- Live FOV transform is separately solved and is out of scope for this artifact.

## Approved scope

- Validate exact store bytes before changing anything.
- Read current top-level game client dimensions and compute `width / height`;
  if the window does not yet exist during early DLL initialization, use the
  primary desktop dimensions as an explicit install-time fallback and log the
  source.
- Accept only a finite, positive aspect within a bounded range.
- Temporarily make the instruction page writable, replace only four immediate
  bytes, flush the instruction cache, and restore the original page protection.
- Restore the original four bytes during process detach when possible.
- Log selected aspect and patch/restore status.

## Explicit non-goals

- No camera-state writes, FOV changes, `+0x259` changes or callback calls.
- No hooks, PAGE_GUARD, VEH, timers, polling, hotkeys or repeated enforcement.
- No stable gameplay ASI changes, release artifact or dynamic re-patching after
  a resolution change. The desktop fallback is read once at install and is not
  a runtime polling mechanism.
- No claim that process-detach restoration is a complete crash-recovery path.

## Batches and validation

### Batch 1 — Feasibility artifact

Create the isolated source and build script. Confirm the source changes exactly
four instruction bytes and contains guarded install/restore paths.

Validation: successful C++ build and static source inspection.

Result: created the isolated source and build script; compilation succeeded and
produced `build-artifacts/test-asi/STALKER2CinematicAspectImmediatePatchFeasibility204.asi`.
The source validates the six-byte store prefix and original four-byte immediate,
changes only those four immediate bytes, flushes the instruction cache and
restores page protection. No hook or camera-state write is present.

Runtime preflight result: the first artifact refused safely because the game
window did not yet exist during early initialization; no code patch was made.
The artifact was revised to use a logged, one-time primary-desktop fallback
when client dimensions are unavailable. Rebuild and runtime validation remain
pending.

Runtime result: the revised artifact installed with `aspect=3.5555556`,
`sourceKind=desktop`, `size=5120x1440`, `patchedBytes=4` and
`cameraWrites=0`. The user observed the cinematic in 32:9. Install-time aspect
feasibility passed; dynamic resolution and FOV integration were not tested.

### Batch 2 — Runtime feasibility

Run only this ASI at the tested ultrawide client size. Compare cinematic aspect,
FOV behavior, native ENTER/EXIT lifecycle and gameplay handoff. Do not enable
the stable gameplay ASI or FOV diagnostic artifacts in the first run.

## Risks and safe failure

- Any executable-byte mismatch, missing client size or invalid ratio refuses the
  patch without modifying code.
- Patching executable code while another thread executes the instruction is a
  runtime risk; this artifact is experimental only.
- If the scene remains 16:9 or the lifecycle regresses, restore/disable this
  artifact and do not alter the stable ASI.

## Stop conditions

- Stop after one clean isolated runtime run.
- Do not add hooks or runtime re-patching if the one-time patch is ineffective.
- Dynamic resolution support requires a separate design; it is not inferred
  from this fixed-at-install experiment.

## Final review

Compare changed paths with this plan, record runtime limits and perform a
read-only Git review before any combined integration work.
