# Cinematic F7 Safe Timing Probe 2.0.4 — Task Plan

## Objective

Prepare a minimal read-only timing artifact after the combined probe crashed immediately after installation, before any logged callback or manual command.

## Established evidence and current state

- The combined probe source contains one ENTER hook, one EXIT hook and a low-level keyboard hook; it contains no `+0x230` writer hooks.
- The crash run logged successful installation and keyboard-hook setup, then crashed before any ENTER/EXIT/manual callback.
- `F7` is read-only, but the failing run did not reach F7.

## Approved scope

- Add a compile-time timing-only variant of the existing combined probe.
- Keep only ENTER snapshot capture, keyboard input and F7 read-only snapshot.
- Remove EXIT hook and all write-capable manual commands from this variant.
- Build the separate test ASI; do not launch the game in this batch.

## Explicit non-goals

- Do not change the default combined probe behavior.
- Do not modify stable gameplay ASI or production source.
- Do not add camera writes, FOV/aspect correction, EXIT handling, timers, VEH or PAGE_GUARD.

## Files or areas expected to be touched

- Existing combined probe source: compile-time guarded timing-only path only.
- New timing-only build script.
- New test ASI output.

## Batch 1 — Minimal artifact

- Add the timing-only compile path and build script.
- Validation: compiler/linker success and output artifact.

## Runtime procedure

- Enable only the new timing-only ASI.
- Let the opening video finish without keys.
- During the second in-engine cinematic, press `F7` 4–6 times quickly.
- Do not use any other function key.

## Risks and safe-failure behavior

- The variant performs no camera writes and installs no EXIT hook.
- ENTER identity and state reads remain guarded; invalid state produces a log and no action.
- Build failure or setup validation failure stops the artifact without runtime testing.

## Stop conditions and phase gates

- Do not rerun the crashing combined artifact.
- If the minimal variant crashes before its install log, close this instrumentation path and stop further changes.
- Runtime interpretation is deferred until a user-supplied log is available.

## Final review requirements

- Perform read-only Git status/diff review after the build.
- Record build success separately from runtime validation.

## Runtime result

- The probe installed successfully. Two F7 presses before the tested cinematic were rejected with `no-active-target`, so the opening video did not establish an active target through this hook.
- The tested cinematic ENTER captured `inner=0x1F303542740`, `FOV=90.65574`, `aspect=3.5555556`, `flags=0x05`.
- Read-only F7 snapshots then showed `aspect=1.7777778` while FOV converged over approximately 0.93 seconds: `90.1982` at `+0.586 s`, `90.05522` at `+0.766 s`, `90.00292` at `+0.927 s`, and `90.0` at `+1.095 s`.
- This establishes a delayed/native FOV initialization tail after ENTER, separate from the immediate aspect transition. It explains why the immediate post-setter `+0x230=127.3927` write was later ineffective, without identifying the native FOV writer itself.
- Timing experiment result: valid and informative; no camera writes were made. Further runtime instrumentation is not authorized by this timing plan.
