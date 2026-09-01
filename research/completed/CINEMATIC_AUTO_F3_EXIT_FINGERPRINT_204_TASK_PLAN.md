# Cinematic Auto-F3 EXIT Fingerprint 2.0.4 — Task Plan

## Objective

Build a separate diagnostic ASI that performs the known combined F3 write once after the native cinematic FOV has reached a stable endpoint, then records the complete EXIT FOV transition for three seconds.

## Established evidence and current state

- 2.0.4 cinematic ENTER and EXIT anchors are validated in the existing causal probe.
- Native ENTER convergence moves FOV toward 90.0 and can overwrite an early manual F3 write.
- The same F3 write is effective after `STEADY_CINEMATIC`.
- The previous post-EXIT observation window was too short to establish the final FOV endpoint.

## Approved scope

- Experimental diagnostic artifact only.
- Reuse the validated ENTER/EXIT hooks and authoritative inner identity.
- Trigger one combined F3 automatically after stable cinematic FOV detection.
- Extend post-EXIT observation to three seconds and label EXIT transitions correctly.
- Keep all writes limited to the known diagnostic F3 values: `+0x230=127.3927` and `+0x254=3.5555556`.

## Explicit non-goals

- No changes to `STALKER2GameplayAspectFix.asi` or stable gameplay source.
- No PAGE_GUARD, VEH, debugger/watchpoint or new hook boundary.
- No polling-based production mechanism; sampling is diagnostic only.
- No new FOV discovery, legacy offset recovery or static executable analysis.
- No release packaging or source integration.

## Expected files or areas

- Existing experimental causal probe source, behind a diagnostic compile-time mode.
- New build script and new `.asi` output.
- This task plan only; no production files.

## Batches and validation

### Batch 1 — bounded implementation

- Add auto-F3 trigger after stable FOV detection.
- Add one-shot guard and explicit `AUTO_F3_TRIGGER` log.
- Extend EXIT observation to 3000 ms and use `EXIT_TRANSITION` phase after EXIT.

Validation: source inspection and compile-time consistency review.

### Batch 2 — local build

- Build the separate diagnostic ASI.
- Verify the expected output exists and no stable artifact is overwritten.

Validation: compiler success and output inspection only; this does not prove in-game behavior.

### Batch 3 — review

- Read-only Git status/diff review against this plan.
- Record runtime validation limits and the exact next log acceptance criteria.

## Risks and safe failure

- If the validated hooks fail setup, the ASI must refuse safely without writes.
- If the target is invalid or FOV is not stable, no auto-F3 write is allowed.
- If EXIT identity is unavailable, the probe records no fabricated state and stops observation safely.

## Stop conditions

- Do not add hooks or widen the capture window beyond this artifact.
- Stop if the source cannot preserve the existing validated identity checks.
- Runtime interpretation remains pending until the user supplies the resulting log and visual result.

## Final review

Confirm changed paths match this plan, stable gameplay files are untouched, the diagnostic artifact is separate, and distinguish build validation from runtime evidence.
