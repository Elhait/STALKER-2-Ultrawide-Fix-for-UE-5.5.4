# Cinematic FOV Transition Candidate Correlation 2.0.4 — Task Plan

## Objective

Run a read-only Gate A correlation for the two current-build static candidates
`FUN_140187898` and `FUN_14038e7d0`, determining whether either commits FOV state
for the same authoritative cinematic `inner` during the native ENTER/EXIT
convergence.

## Established evidence and current state

- Current executable identity is SHA-256
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Runtime evidence shows ENTER convergence `90.6557 -> 90.0` and EXIT convergence
  from modified FOV back to the ENTER baseline.
- Static current-build evidence identifies block-commit candidates at
  `0x1401882ba` (`FUN_140187898`) and `0x140392e01` (`FUN_14038e7d0`).
- Neither candidate has runtime causal correlation yet.

## Approved scope

- Add one experimental read-only ASI artifact.
- Validate and observe the two exact current-build `+0x230` commit instructions.
- Use the already validated ENTER/EXIT lifecycle only to capture and clear the
  same authoritative `inner` identity.
- Log candidate identity, destination base, same-inner result, incoming/current
  FOV, aspect, flags, thread and lifecycle phase.

## Explicit non-goals

- No camera-state writes, auto-F3, hotkeys or gameplay/global ASI loading.
- No PAGE_GUARD, VEH, sampling, memory scanning or additional discovery paths.
- No stable gameplay source, global experimental ASI or release asset changes.
- No promotion of a candidate from static evidence alone.

## Expected files or areas to touch

- `src/cinematic_fov_transition_candidate_correlation_204.cpp`
- `build-artifacts/test-scripts/build-cinematic-fov-transition-candidate-correlation-204.cmd`
- `build-artifacts/test-asi/STALKER2CinematicFovTransitionCandidateCorrelation204.asi`

## Batches and validation

### Batch 1 — bounded artifact implementation

- Create lifecycle-gated read-only hooks for the two exact candidate commit
  instructions.
- Validate the executable identity and expected bytes before installing.
- Fail closed and roll back all hooks on any validation or creation failure.

Validation: source review and successful build with the expected ASI output.

Status: completed. The artifact validates the current-build bytes before
installing the lifecycle gates and the two candidate instruction hooks. All
callbacks are read-only; setup failure resets every hook created by the task.

Correction during validation: direct executable-byte inspection showed the
candidate store RVAs were initially offset from the actual instructions. They
were corrected to `0x18826D` for `FUN_140187898` and `0x392D9D` for
`FUN_14038E7D0`; setter and EXIT validation remained unchanged. The artifact
was rebuilt successfully after this correction.

### Batch 2 — Gate A runtime correlation

- Run only the new ASI through the known test cinematic.
- Compare hits against the captured `inner` and the native ENTER curve.
- Do not use modified FOV in this gate.

Validation: a candidate passes only with same-inner runtime hits whose observed
state tracks the native convergence; otherwise it is rejected for this gate.

Status: completed from the supplied user-run log. The ASI installed and
captured the cinematic `inner` during the tested lifecycle.

Runtime result:

- `FUN_140187898` produced many hits, but none matched the captured authoritative
  `inner`; destinations were unrelated objects with non-camera-like values.
- `FUN_14038E7D0` produced no hits in the run.
- No candidate produced a same-inner observation tracking the native
  `90.6557 -> 90.0` ENTER convergence.

The artifact revealed pre-window log noise from the first candidate, but the
verdict remains zero same-inner candidate hits during the active window.

### Batch 3 — final review

- Inspect status/diff against this plan.
- Record build/load/runtime limits and candidate verdicts.

Validation: no candidate is promoted without reproducible runtime evidence.

Status: completed. Gate A rejected both static candidates for the tested
current-build cinematic ENTER path. No Gate B write test is justified.

## Risks and safe failure

- Candidate RVAs are version-specific; byte validation must refuse unknown builds.
- Hooking an instruction with the wrong register assumption can corrupt control
  flow; callbacks remain read-only and exact instruction bytes are checked.
- If any hook cannot be installed safely, all created hooks are reset and the
  artifact logs a setup refusal.

## Stop conditions and phase gates

- If both candidates produce no same-inner ENTER hits, close this branch.
- Do not add a third candidate in this artifact.
- Do not proceed to Gate B or any write test from this pass alone.

## Final review requirements

Report exact source/build paths, executable identity, installed hooks, runtime
log limits, candidate verdicts, untouched stable/global ASIs and remaining
uncertainty.

Final result: both static candidates were runtime-rejected for Gate A. The
supplied log captured ENTER at `inner=0x2158CBE3400`; `FUN_140187898` had no
same-inner hit and `FUN_14038E7D0` had no hit. The native convergence path
remains unresolved, but this candidate branch is closed without widening the
search. No implementation or production change is justified.
