# Cinematic FOV/Aspect Causal Transition 2.0.4 — Task Plan

## Objective

Measure the current 2.0.4 cinematic lifecycle and determine whether native FOV convergence overwrites the known manual combined correction during the ENTER transition.

## Established evidence and current state

- The same authoritative `inner` camera state is captured at the validated cinematic ENTER boundary.
- Aspect changes from `3.5555556` to `1.7777778` immediately after ENTER.
- FOV converges from `90.65574` to `90.0` over roughly one second.
- Manual combined `FOV=127.3927` plus `aspect=3.5555556` is visually effective once the cinematic is active.
- Three enumerated scalar `+0x230` writers were not correlated with the observed inner; their branch is closed.

## Approved scope

- Create a separate 2.0.4 diagnostic ASI.
- Reuse only the validated ENTER and EXIT lifecycle anchors.
- Automatically sample same-inner `+0x230`, `+0x254` and `+0x259` changes and log previous-to-current transitions with timestamps and phases.
- Add only one manual command: `F3`, which performs the known combined diagnostic write `FOV=127.3927` and aspect `3.5555556`, with before/after logging.
- Use polling only as read-only observation; it must not be used to perform correction or enforce state.

## Explicit non-goals

- No production correction, stable gameplay changes or release integration.
- No writer hooks, PAGE_GUARD, VEH, debugger sampling, timers for correction or repeated enforcement writes.
- No automatic FOV/aspect restore beyond native lifecycle observation.
- No other manual write commands or legacy offsets.

## Files or areas expected to be touched

- New experimental source under `src`.
- New test build script under `build-artifacts/test-scripts`.
- New test ASI output.
- This plan file.

## Batch 1 — Diagnostic artifact

- Implement the read-only lifecycle sampler and single F3 diagnostic command.
- Validate source safety properties and build output.

## Runtime procedure

- Enable only the new probe.
- Do not press F3 during the opening video.
- When the second in-engine cinematic begins, press F3 as early as possible once.
- Let the cinematic and exit complete naturally.

## Risks and safe-failure behavior

- The sampler reads only the captured inner and does not write during automatic observation.
- F3 is explicitly diagnostic and writes only the known combined pair once.
- Invalid pointers or state reads cause logging/no-op; hook setup validates current bytes and rolls back partial hooks.

## Stop conditions and phase gates

- Stop if ENTER/EXIT validation or hook installation fails.
- Do not add further hooks or broaden sampling if the probe produces no causal timeline.
- This artifact is not evidence of a production correction until the user supplies runtime logs and visual results.

## Final review requirements

- Perform read-only Git status/diff review after the build.
- Report build success separately from runtime validation and keep TASKLOG update deferred until runtime evidence is reviewed.
