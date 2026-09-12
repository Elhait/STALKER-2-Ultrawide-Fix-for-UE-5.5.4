# POST_EXIT_PCM_VIEWTARGET_CAMERACACHE_TOPOLOGY_AUDIT_205

## Objective

Perform one bounded, read-only Ghidra audit of the 2.0.5 PCM/ViewTarget/
CameraCache topology around the confirmed post-EXIT FOV handoff. Look for a
camera/view update or blend state that can account for the observed
`126.87 -> 90` recovery without assuming that the writer or a scalar `+0x230`
store is the interpolation owner.

## Established evidence

- Current 2.0.5 image identity is gated by SHA-256, `.text` size, image base and
  section layout before analysis.
- `FUN_14318DCD4` receives the native EXIT target FOV `90`.
- During final `CinematicExiting`, `cameraWorldFov` and `writerInputXmm0` show a
  roughly one-second `126.87 -> 90` stream.
- `FUN_1432B7706` executes, but its `+0x230` store targets other objects and is
  rejected as the active recovery owner.
- The known writer is a consumer of the interpolated stream; production ASI
  behavior must remain untouched.

## Approved scope

- Read-only Ghidra analysis of the validated 2.0.5 executable only.
- Inspect only the known PCM/camera object offsets and the immediate functions
  reachable from the established handoff/camera-update evidence.
- Classify references to `ViewTarget`, `PendingViewTarget`,
  `CameraCachePrivate` and `LastFrameCameraCache`.
- Record functions that combine current/target POV, FOV, delta time, blend
  duration/progress or cache propagation.
- Produce one evidence report with identity header and an explicit result.

## Explicit non-goals

- No source edits, runtime probe, hook, write, guessed call or production build.
- No broad `.text`, renderer, `.rdata` or scalar `+0x230` enumeration.
- No claim of native UE blend ownership from names or offsets alone.
- No modification of the stable ASI or release artifacts.

## Expected files or areas

- One reusable Ghidra script under `02-Research/Ghidra/ghidra-scripts`.
- One bounded runner/evidence file under `02-Research`.
- One report under `02-Research/reports`.
- This task plan, archived after the pass.

## Batches

### Batch 1 — identity and existing-evidence check

Verify the exact 2.0.5 image identity and inspect existing reports/scripts for
the confirmed handoff, PCM offsets and writer callsite.

### Batch 2 — bounded topology audit

Use one tracked, read-only/no-analysis Ghidra run. Inspect only the bounded
handoff/camera-update neighborhood and classify direct code/data references,
field accesses and any explicit blend/cache state.

### Batch 3 — report and review

Write the evidence report, compare results with this plan, and perform a
read-only Git status/diff review. Do not expand the search from a negative or
ambiguous result.

## Validation

- Identity: SHA-256, `.text` size, image base and section layout must pass.
- Ghidra process and project lock must be tracked and cleanly released.
- Every positive topology claim must include function/RVA and the observed
  supporting instructions or decompilation context.
- Build/runtime validation is intentionally not performed.

## Risks and safe failure

- Wrong or stale image: stop immediately and mark the pass invalid.
- Ambiguous field/type association: report as unresolved, not as ownership.
- Ghidra timeout/lock: terminate only the tracked agent-owned process after
  rechecking its identity and verify the project lock is released.

## Stop conditions

- Stop after the bounded handoff/camera-update neighborhood.
- If no function exposes new ViewTarget/cache blend state or a transition
  parameter, record `NO NEW TOPOLOGY ANCHOR` and defer further ASI research.
- Runtime work is allowed only in a separately approved task after a concrete
  new anchor is established.

## Final review

Perform read-only Git status, relevant diff/stat review and path comparison
against the approved scope. Report completed, remaining, deferred, blocked and
not-runtime-validated items.
