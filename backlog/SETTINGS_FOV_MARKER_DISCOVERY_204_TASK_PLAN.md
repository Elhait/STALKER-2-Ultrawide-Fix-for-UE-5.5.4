# Settings FOV Marker Discovery 2.0.4 — Task Plan

## Objective

Find current-build runtime storage candidates for the controlled Settings FOV sequence `90 → 77 → 79 → 90` without changing game state.

## Established evidence and current state

- The validated gameplay camera writer observed initialization FOV `120` but did not observe live Settings changes.
- The current Settings FOV consumer/path is unresolved.
- The next safe anchor is the controlled marker sequence itself; numeric matches are only candidates until repeated at one address and later correlated to an execution path.

## Approved scope

- Create one read-only marker observer for the current 2.0.4 process.
- Track bounded writable-memory float candidates and report repeated transitions matching `90`, `77`, `79`, and `90`.
- Record address, old/new value, thread, timestamp and candidate region metadata.
- Use candidate addresses only as anchors for a later instruction-level tracer.

## Explicit non-goals

- No writes, page-guard changes, breakpoints, code patches or game calls.
- No cinematic hooks or stable gameplay ASI.
- No promotion of a candidate from a single numeric match.
- No causal write-test in this task.

## Expected files or areas

- `src/settings_fov_marker_discovery_204.cpp`
- `build-artifacts/test-scripts/build-settings-fov-marker-discovery-204.cmd`
- `build-artifacts/test-asi/STALKER2SettingsFovMarkerDiscovery204.asi`
- Runtime log in the game binary directory.

## Batches and validation

### Batch 1 — Read-only marker observer

- Sample bounded writable committed memory at a low-frequency interval.
- Admit candidates only after stable baseline observations and report only marker transitions.
- Cap tracked candidates and total scan bytes to avoid process-wide flood.
- Build validation proves compilation and safe setup only.

Batch 1 implementation result: created `src/settings_fov_marker_discovery_204.cpp` and `build-artifacts/test-scripts/build-settings-fov-marker-discovery-204.cmd`. The observer performs bounded read-only polling of writable committed memory, tracks stable `90.0`/`90.65574` baseline candidates, reports marker transitions and complete cycles, and scans in rotating 64 MiB portions so high-address game allocations are not excluded by a low-address cutoff. The artifact built successfully; no game or stable ASI changes were made.

### Batch 2 — Controlled Settings capture

- Enable only the marker observer; disable all other ASI.
- Apply `90 → 77 → 79 → 90` in one session with a short pause between changes.
- Promote only addresses showing the complete sequence on the same object/region identity.

Batch 2 decision: the observer produced no useful marker log. More importantly, the experiment is not a valid proxy for the historical cinematic fix: the legacy `LetterboxFix` changed a live FOV value in the cinematic call/data-flow, not necessarily the user Settings storage. The Settings-marker branch is therefore closed as an invalid premise for cinematic FOV discovery; the empty log is not treated as proof that no Settings path exists. No candidate or write-test is authorized.

### Batch 3 — Instruction-level follow-up

- Use at most one promoted marker address for a separate runtime write-origin/data-flow tracer.
- Do not infer instruction RVA from polling alone.

## Risks and safe-failure behavior

- Memory polling can produce false candidates; repeated sequence correlation is mandatory.
- Scan limits, candidate caps and interval prevent unbounded logging/CPU use.
- On setup failure the ASI logs the failure and performs no writes.

## Stop conditions and phase gates

- Stop after the controlled marker sequence is captured or unavailable.
- Stop if no address repeats the full sequence; do not broaden scan limits in this task.
- No instruction-level claim or write-test until a candidate address is promoted.

## Final review

- Classify candidates as confirmed marker storage, plausible or rejected.
- Review changed paths against this plan and perform read-only Git status/diff review.
- Keep stable gameplay and cinematic artifacts untouched.
