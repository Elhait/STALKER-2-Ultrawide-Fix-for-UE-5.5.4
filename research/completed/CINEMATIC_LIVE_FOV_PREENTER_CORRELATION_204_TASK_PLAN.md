# Cinematic live-FOV pre-ENTER correlation — 2.0.4

## Objective

Extend the isolated live-FOV consumer correlation experiment with one bounded
pre-ENTER observation at current callsite `0x2EE6936`, then correlate it to
the next validated ENTER using thread, QPC proximity and transition-context
pointer. Determine whether the ENTER global-to-XMM0 path is executed before
the lifecycle hook opens its window.

## Scope and evidence

- Executable: Steam 2.0.4, SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Existing Gate A confirmed EXIT `[RDI+0x38] -> XMM0 -> FUN_146B6DAAC`.
- ENTER callsite `0x2EE6936` was not observed only after the lifecycle gate;
  its static `MOVSS XMM0,[0x149EE151C]` source is validated.

## Approved scope

- Modify only the research consumer-correlation artifact and its task plan.
- Permit at most one pre-ENTER log record per lifecycle at `0x2EE6936`.
- Log source/XMM0, transition context, thread and QPC proximity to ENTER.
- Retain existing read-only ENTER/EXIT anchors and EXIT callsite logging.

## Explicit non-goals

- No game-state writes, FOV correction, aspect changes or production hook.
- No new hooks beyond the two consumer callsites and existing lifecycle
  anchors.
- No PAGE_GUARD, VEH, polling, random sampling or broad tracing.
- Do not modify stable gameplay/global ASI.

## Validation and safe failure

- Validate all four existing anchor/callsite byte sequences before installing.
- If any validation or hook creation fails, reset all hooks and leave state
  untouched.
- Build success is not runtime proof; user must provide an isolated log.
- A pre-ENTER record without close ENTER correlation is inconclusive, not a
  FOV verdict.

## Stop conditions

- One artifact and one user-run gate only.
- Promote the ENTER boundary only if pre-ENTER source/XMM0 and subsequent
  ENTER share a plausible context/thread/time relationship.
- Otherwise classify ENTER as unobserved and close this gate.

## Runtime result

Gate A passed on the supplied isolated run. `0x2EE6936` produced one
`PRE_ENTER` record with `source=global`, `sourceValue=90`, and `XMM0=90`.
The following validated ENTER used the same thread and the same transition
context pointer, with `preEnterDeltaQpc=774`. The EXIT callsite then produced
`source=[RDI+0x38]`, `sourceValue=90.65574`, and `XMM0=90.65574` on the same
thread immediately before EXIT. This establishes the current-build
live-FOV-consumer boundary for both source patterns. No write-test has been
performed; any write feasibility must be a separate plan and artifact.
