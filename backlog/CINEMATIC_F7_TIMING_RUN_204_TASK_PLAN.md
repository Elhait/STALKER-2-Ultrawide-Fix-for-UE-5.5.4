# Cinematic F7 Timing Run 2.0.4 — Task Plan

## Objective

Prepare a clean manual-probe ASI for a read-only timing run that samples the tested cinematic camera state with `F7` immediately after the second in-engine cinematic begins.

## Established evidence and current state

- The combined manual probe already has a validated 2.0.4 ENTER/EXIT lifecycle and working keyboard input through `F1–F8`.
- `F7` performs only a read-only snapshot; `F1–F6` and `F8` perform writes and must not be used in this run.
- The game begins with a video sequence, followed by the tested in-engine cinematic; the timing run must distinguish these phases by timestamps and ENTER/EXIT records.

## Approved scope

- Rebuild the existing combined manual probe without source or behavior changes.
- Produce the ASI for one isolated runtime timing run.

## Explicit non-goals

- No gameplay ASI, automatic correction probe, FOV230 tracer or other diagnostic ASI.
- No source changes, camera writes, new hooks or runtime launch by this batch.

## Files or areas expected to be touched

- Existing combined manual probe build script and source are inspected only.
- Generated test ASI output is rebuilt; it is not a production release artifact.

## Batch 1 — Isolated rebuild

- Rebuild the existing combined manual probe.
- Validation: compiler/linker success and expected ASI output.

## Runtime procedure

- Enable only the rebuilt combined manual probe.
- Do not press keys during the opening video.
- When the second, in-engine cinematic begins, press `F7` 4–6 times quickly.
- Do not use `F1–F6` or `F8`; allow the cinematic to finish naturally.

## Risks and safe-failure behavior

- `F7` is read-only. If no active target exists, the probe logs a rejected snapshot and performs no write.
- Any unexpected hotkey/keyboard registration or setup failure stops the test without camera writes.

## Stop conditions and phase gates

- Stop if the probe does not install or `F7` does not produce `manual-snapshot` records.
- Do not infer timing from the opening video until the log identifies its lifecycle separately.

## Final review requirements

- Review build output and read-only Git status/diff after the rebuild.
- Runtime visual/log analysis is deferred until the user supplies the new log.
