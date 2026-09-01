# Cinematic Post-Enter Anchor Batch 2.0.4 — Task Plan

## Objective

Evaluate at most three safe, deterministic post-enter runtime anchors where a single reversible write to the already validated cinematic state can become visually effective.

## Established evidence and current state

- 2.0.4 cinematic ENTER aspect transition is runtime-confirmed.
- Native EXIT callback `FUN_146B6C482` and gameplay handoff are runtime-confirmed.
- Manual `aspect=3.5555556` plus `+0x230=127.3927` produces correct cinematic framing.
- Immediate post-setter `+0x230` write is state-visible but visually ineffective.
- The adjacent `0x26F7A23/0x26F7A25` bridge is closed as implementation-unsafe with stock `SafetyHookMid`.

## Approved scope

- Inspect only existing 2.0.4 runtime/static evidence for camera activation/update-adjacent boundaries.
- Select no more than 2–3 candidates.
- Use ordinary safe hooks only, with strict target/context validation.
- Test one reversible `+0x230` write per candidate, only after the candidate executes post-enter.
- Keep aspect handling and native EXIT ownership unchanged.

## Explicit non-goals

- No broad FOV or scalar-FP scans.
- No new PAGE_GUARD, VEH, debugger-style sampling or exception instrumentation.
- No custom assembly, naked stubs, stack manipulation or adjacent dual hooks.
- No legacy RVA/offset transplantation without current-build validation.
- No changes to `STALKER2GameplayAspectFix` or production release artifacts.

## Expected files or areas

- Existing experimental cinematic research sources and build scripts only, if a candidate is selected.
- This plan file.
- No stable gameplay source or release directory.

## Batches

### Batch 1 — Candidate inventory and safety screening

- Review existing 2.0.4 logs, task plans and research artifacts.
- Rank up to three post-enter camera/update-adjacent anchors.
- Validation: each candidate must have current-build evidence, deterministic timing after ENTER, and a safe ordinary hook surface.

Batch 1 result: the existing 2.0.4 evidence does not contain a new safe post-enter anchor to promote. The immediate post-setter boundary was runtime-tested and made aspect effective but not the cinematic FOV write. `RVA 0x26F7A25` remains a timing hypothesis but its available stock-SafetyHook implementation is unsafe because post-call identity is volatile, adjacent dual hooks overlap, and `MidHook` exposes no callable original bridge. The known dispatcher produced no context-correlated steady-cinematic hits, and the validated gameplay writer is not a cinematic steady-state consumer. No candidate is promoted, no new ASI is built, and no write-test is authorized.

Phase result: this bounded candidate batch is closed negative for the currently available evidence. The cinematic manual combined behavior remains confirmed; only the search for a safe automatic post-enter boundary is unresolved outside this batch.

### Batch 2 — Candidate 1 runtime test

- Build/run one isolated reversible probe only if Batch 1 identifies a viable candidate.
- Validation: one post-enter FOV write, no repeated writes, visual framing result, and no crash or lifecycle pollution.

### Batch 3 — Candidate 2 and optional Candidate 3

- Proceed only if the previous candidate is negative and the stop conditions remain unmet.
- Validation: same bounded criteria as Batch 2.

## Risks and safe-failure behavior

- Reject any candidate with ambiguous object identity, volatile-register reconstruction, overlapping patch ranges, or unsafe control-flow requirements.
- Probe must fail closed and perform no write when validation fails.
- Disable all other ASIs during runtime tests.
- Preserve stable gameplay ASI and all production artifacts.

## Stop conditions and phase gates

- Stop after three rejected or ineffective candidates.
- Stop immediately on a crash, ambiguous target, or need for custom control-flow machinery.
- A candidate is accepted only if one post-enter write makes cinematic framing match the manual combined state without polling/timers.
- Negative results close this automation model; they do not invalidate the confirmed manual aspect/FOV behavior.

## Final review requirements

- Perform read-only Git status and diff review.
- Compare actual changed paths with this plan.
- Record completed, remaining, deferred, blocked and not-runtime-validated outcomes before any final task-log update.
