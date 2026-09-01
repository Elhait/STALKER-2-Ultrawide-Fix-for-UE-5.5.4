# Cinematic Combined Live Boundary Feasibility 2.0.4 — Task Plan

Status: Complete — isolated combined feasibility passed at 5120x1440. Stable
gameplay code remains untouched; coexistence and dynamic-resolution tests are
deferred.

## Objective

Test the two independently validated mechanisms together on cinematic ENTER:

- aspect: replace only the immediate of `RVA 0x6B7CB05` with the install-time
  client/desktop aspect;
- FOV: transform authored `XMM0=90.0` at exact consumer callsite `RVA
  0x2EE6936` to the Hor+ value and let the native call continue.

## Established evidence

- 2.0.4 executable SHA-256:
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Aspect store `0x6B7CB05` and its authoritative `RAX` lineage are runtime
  confirmed; replacing its four-byte immediate produced visual 32:9.
- Live FOV consumer `0x2EE6936` is runtime confirmed; transforming `XMM0`
  from authored `90.0` to `126.869896` produced a visible wider FOV.
- Native EXIT behavior is already confirmed, but EXIT intervention is excluded
  from this first combined run.

## Approved scope

- One guarded four-byte immediate patch at `0x6B7CB05`.
- One ordinary read-only `SafetyHookMid` at `0x2EE6936` that changes only the
  transient `XMM0` lane for the first valid ENTER consumer call.
- Install-time aspect selection from client window, with one-time desktop
  fallback if the window is not yet created.
- Restore the original immediate on process detach when possible.
- Log patch source/ratio and the FOV before/after transform.

## Explicit non-goals

- No `+0x230` writes, aspect state writes, flags writes or callback calls.
- No ENTER/EXIT lifecycle hooks beyond the exact FOV consumer boundary.
- No timers, polling, PAGE_GUARD, VEH, hotkeys or repeated enforcement.
- No stable gameplay/global ASI changes or release artifact.
- No dynamic resolution re-patching during the session.

## Batches and validation

### Batch 1 — Combined artifact

Create source/build script, compile and inspect that only the immediate patch
and one FOV consumer hook are present.

Result: source and build script created; compilation succeeded and produced
`build-artifacts/test-asi/STALKER2CinematicCombinedLiveBoundaryFeasibility204.asi`.
Static inspection confirms one four-byte aspect-immediate patch, one ordinary
FOV consumer hook, fail-closed byte validation and restoration on setup failure
or process detach. No camera-state writes are present.

### Batch 2 — Isolated runtime feasibility

Enable only the combined artifact at `5120x1440`. Verify from the log and visual
result: aspect patch installed before ENTER, one `90.0 -> 126.869896` transform,
32:9 framing from the start, no delayed correction jump and no crash.

Result: the artifact installed with `aspect=3.5555556` and transformed the live
ENTER FOV from `90` to `126.869896` at `RVA 0x2EE6936`. The user observed the
cinematic as 32:9 with correct FOV. No camera-state writes or delayed correction
were observed.

### Batch 3 — Decision

PASS requires immediate correct 32:9 cinematic framing without delayed FOV
correction. EXIT and gameplay coexistence are deferred to a separate gate after
this isolated result.

Result: PASS. The isolated combined mechanism is validated. Stable gameplay
coexistence, EXIT handoff regression and dynamic resolution remain outside this
completed gate.

## Risks and safe failure

- Any exact-byte mismatch refuses both mechanisms without patching.
- Failure to read dimensions refuses the code patch.
- If hook creation fails after patching, restore the immediate before reporting
  setup failure.
- The artifact is experimental and not a stable release candidate.

## Stop conditions

- Stop after one isolated runtime run.
- Do not add EXIT hooks, gameplay ASI or dynamic re-patching in this phase.
- If the combined artifact is visually wrong or unstable, preserve the two
  separate passing baselines and investigate only under a new plan.

## Final review

Compare changed paths with this plan and perform read-only Git review before any
combined integration or production-source change.
