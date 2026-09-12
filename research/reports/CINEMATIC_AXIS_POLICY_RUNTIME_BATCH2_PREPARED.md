# Cinematic Axis Policy — Runtime Batch 2 Preparation

## Scope

Prepared a separate read-only diagnostic ASI for the confirmed 2.0.4 writer
boundary. No camera, output, projection, or production source writes were
added.

## Diagnostic behavior

The tracer validates the current 2.0.4 enter/exit instruction anchors and the
unique known writer signature before installing hooks. It observes only:

- source pointer and output pointer at the validated writer boundary;
- source `+0x258`;
- source `+0x259` and gate `source +0x259 & 0x2`;
- output `+0x60` and validity byte `output +0x64`;
- ENTER, EXIT and first post-EXIT writer lifecycle markers.

Samples are emitted on value/pointer/phase changes to limit log volume.

## Build evidence

- Build script: `build-artifacts/test-scripts/build-cinematic-axis-policy-lifecycle-trace-204.cmd`
- Output: `build-artifacts/test-asi/STALKER2CinematicAxisPolicyLifecycleTrace204.asi`
- Build result: **PASS**
- Runtime result: **NOT YET VALIDATED**

The user must run the diagnostic in the game and provide
`STALKER2CinematicAxisPolicyLifecycleTrace204.log` for Batch 2 analysis.

## Safety boundary

This artifact is research-only. It is not the stable ASI, does not modify the
production build, and does not perform runtime writes.
