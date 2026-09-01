# Cinematic live-FOV consumer write feasibility — 2.0.4

## Objective

Test whether transforming the live cinematic FOV scalar in `XMM0` at the
validated current-build consumer callsite removes the visible ENTER jump.

## Established evidence

- 2.0.4 SHA-256:
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- `FUN_142EE68DA` is the current transition descendant.
- ENTER consumer callsite `0x2EE6936` receives the live global FOV in XMM0.
- EXIT consumer callsite `0x2EE69A7` receives `[RDI+0x38]` in XMM0.
- Read-only Gate A correlated both source patterns to the cinematic lifecycle.

## Approved scope

- One separate experimental ASI.
- At most one Hor+ transform of XMM0 on the ENTER consumer callsite per
  lifecycle; native call executes once through SafetyHook's normal path.
- Keep EXIT read-only and retain validated lifecycle logging.
- Use the established aspect path as an external condition; do not write
  aspect from this boundary because authoritative inner is unavailable there.

## Non-goals

- No durable camera-state writes, aspect writes, EXIT writes or polling.
- No PAGE_GUARD, VEH, manual assembly, post-call bridge or adjacent hooks.
- No stable/global ASI or production-source changes.

## Validation and stop conditions

- Refuse safely on any anchor/callsite byte mismatch or hook failure.
- Build success is not runtime proof.
- PASS requires corrected FOV at ENTER without the delayed jump, with native
  call once and no crash or EXIT regression.
- If aspect is not 32:9 during the visual run, classify the run as incomplete,
  not as a FOV rejection.

## Runtime result

FOV write feasibility passed. The ENTER consumer callsite transformed
`XMM0=90.0` to `126.869896` before the native call, and the scene visibly used
the wider FOV. EXIT remained read-only and observed
`[RDI+0x38]=XMM0=90.65574`. The run's authoritative aspect was still
`1.7777778`, so it validates only the live-FOV transform, not the complete
32:9 cinematic presentation. A separate established aspect-path integration
is required before claiming full visual PASS.
