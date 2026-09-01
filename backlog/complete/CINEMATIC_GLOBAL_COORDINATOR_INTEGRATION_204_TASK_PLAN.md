# Cinematic Global Coordinator Integration 2.0.4 — Task Plan

Status: Closed — the coordinator and handoff experiments established the
recovery/replay behavior, but the post-EXIT visual seam remained. No stable
source or production coordinator policy was adopted.

## Objective

Create an isolated global integration artifact that coordinates the validated
cinematic aspect/FOV mechanisms with the existing gameplay aspect state machine,
without allowing gameplay re-arm logic to interfere with native cinematic EXIT
FOV recovery.

## Established evidence and current state

- Cinematic combined feasibility passed at `5120x1440`:
  aspect immediate patch `RVA 0x6B7CB05` plus live FOV transform at
  `RVA 0x2EE6936` produced visually correct 32:9 framing.
- The live FOV transform changes authored `XMM0=90.0` to `126.869896` and
  does not directly write camera state.
- Native EXIT recovery is game-owned and should remain untouched.
- Coexistence with `STALKER2GameplayAspectFix.asi` failed: its camera rebuild
  path later observed `primaryFOV=126.87` and replayed the gameplay constrained
  pass, causing the visible overshoot/recovery sequence.
- The stable gameplay state machine is in `src/gameplay_aspect_fix.cpp`; its
  existing two-pass behavior and player-FOV preservation are accepted contracts
  outside cinematic lifecycle.

## Approved scope

- Build an isolated experimental global ASI; do not modify the stable gameplay
  implementation in this phase.
- Reuse only the already validated cinematic aspect immediate and live XMM0
  boundaries.
- Introduce explicit coordinator states: `Gameplay`, `CinematicActive` and
  `CinematicExiting`.
- Suppress gameplay constrained/restore replay while cinematic is active or
  while native EXIT FOV recovery is in progress.
- Record the EXIT native target as bookkeeping only; do not manually restore
  FOV or aspect.
- Resume gameplay processing only from an already existing validated writer
  event when current FOV is within an explicit epsilon of the recorded native
  EXIT target.
- Reset the cinematic FOV bookkeeping at the validated EXIT boundary; allow
  the next validated ENTER to arm the transform again.

## Explicit non-goals

- No timers, sleeps, frame counters, polling loops, PAGE_GUARD or VEH.
- No direct FOV restore, `+0x230` writes or manual callback invocation.
- No changes to `STALKER2GameplayAspectFix.asi`, stable source or release files.
- No dynamic-resolution policy in the first integration artifact.
- No second-cinematic validation until the first-cycle handoff passes.

## Expected files or areas

- New isolated source under `src/` for the global coordinator.
- New dedicated build script and test `.asi` under `build-artifacts/test-*`.
- This plan and a research/runtime report only.
- Stable `src/gameplay_aspect_fix.cpp` is expected to remain untouched.

## Batches

### Batch 1 — Ownership and state-machine audit

Inspect the stable gameplay writer/state transitions and define the exact
suppression/resume points without editing stable code.

Validation: source review against the existing gameplay contract and current
coexistence logs.

Result: all replay decisions are inside `ReplayManualTransition` in
`src/gameplay_aspect_fix.cpp`. The writer entry and source-state read are always
allowed. The `Complete + wide aspect + flags 0x04` branch logs camera rebuild,
sets `WaitingForAutomaticUpdate` and enables replay; the next invocation applies
the constrained pass, and the following invocation performs Auto restore. The
suppression matrix is:

```yaml
gameplay_writer_entry: allowed in all coordinator states
camera_rebuild_detection:
  Gameplay: preserve existing re-arm
  CinematicActive: observe, suppress re-arm
  CinematicExiting: observe, suppress re-arm
constrained_pass:
  Gameplay: allowed
  CinematicActive: suppressed
  CinematicExiting: suppressed
auto_restore:
  Gameplay: allowed
  CinematicActive: suppressed
  CinematicExiting: suppressed
exit_recovery_observation:
  source: existing gameplay writer event
  writes: none by coordinator
resume:
  state: CinematicExiting
  condition: flags=0x04 and abs(primaryFov-recordedExitTarget)<=epsilon
```

The `primaryFOV=126.87` coexistence log confirms that the current separate
gameplay ASI can mistake the cinematic-transformed value for a gameplay camera
rebuild. The global artifact must therefore own the coordinator and the
gameplay replay decision in one source; merely loading the two existing ASIs
together cannot provide shared state.

### Batch 2 — Isolated global artifact

Implement the coordinator in a separate source, preserving the two validated
cinematic mechanisms and adding only event-driven lifecycle coordination.

Validation: build, byte/signature checks, source inspection and artifact
isolation. No runtime claim from build success.

Result: completed. Added the isolated coordinator source and dedicated build
script. The source keeps the stable gameplay implementation untouched, patches
only the validated cinematic aspect immediate, transforms ENTER `XMM0` once at
the validated live-FOV callsite, suppresses gameplay replay in
`CinematicActive`/`CinematicExiting`, resets `fovApplied` at EXIT, and returns
without replay on the writer invocation that first recognizes native recovery.
The next writer invocation is the only point where the original gameplay
state machine can resume. Setup and detach restore the aspect immediate
fail-closed.

Build result: `STALKER2CinematicGlobalCoordinatorIntegration204.asi` built
successfully with the project C++23/SafetyHook/Zydis configuration. Static
inspection confirmed one gameplay writer hook, two exact cinematic boundary
hooks, no PAGE_GUARD/VEH path, no timer/polling logic, and no direct `+0x230`
camera-state write in the coordinator path.

### Batch 3 — First coexistence runtime gate

Run one cycle with the new global artifact only, at `5120x1440`, and compare
cinematic ENTER, native EXIT recovery and gameplay handoff.

Acceptance: no gameplay replay during native recovery; no FOV overshoot; clean
gameplay state after EXIT.

### Batch 3A — Read-only post-EXIT A/B/C classification

Create a separate diagnostic artifact that preserves the frozen coordinator
behavior and records only the state around the existing handoff stages:
`A_RECOVERY_COMPLETE`, `B_CONSTRAIN_PRE/POST` and `C_RESTORE_PRE/POST`.
Each marker records source/output values, pointers and mode fields. Per-writer
suppression spam and all new writes are excluded.

Validation: build and static inspection only before runtime. The runtime gate
uses one `cinematic -> gameplay` cycle and classifies the first output change
as `A->B`, `B->C` or downstream of the current writer output.

Implementation status: artifact built successfully as
`STALKER2CinematicPostExitABCOutputDiagnostic204.asi`. It retains the frozen
coordinator behavior and adds only bounded `ABC` snapshots around the existing
handoff stages; it does not add hooks, writes, timers or polling. Runtime
validation remains pending.

### Batch 3B — One downstream presentation-adjacent candidate

Candidate selected: current `FUN_140AF4FA4` at `RVA 0xAF4FA4`, the validated
downstream callee of the camera-writer path. Its current 2.0.4 prologue is
guarded by exact bytes before hook installation. The artifact adds one
read-only candidate hook and correlates it only when its `RCX` output pointer
matches the output captured at an A/B/C marker. It records the candidate's
scalar inputs and output fields; no additional game-state writes or discovery
mechanisms are added.

Build result: completed as
`STALKER2CinematicPostExitDownstreamRecalcCorrelation204.asi`.
Runtime validation is pending. Acceptance requires writer/source linkage,
candidate activity at A/B/C or immediately before the seam, and a directionally
consistent state change. Mere candidate activity is not promotion evidence.

Runtime result: `FUN_140AF4FA4` correlated to the same output pointer and
executed at post-EXIT `A_RECOVERY_COMPLETE`, `B_CONSTRAIN_POST` and
`C_RESTORE_POST`. Its observed output fields remained effectively unchanged
(`outputScale=512`, `output9D0=1`, `output9D4=1`, `output9D8=0`; output FOV only
followed the already recovered source FOV). No directionally meaningful
presentation change was observed at A, B or C. Verdict: rejected as the
presentation-relevant downstream boundary for this seam. The downstream
recalculation branch is closed; coordinator behavior remains frozen.

### Batch 3C — Aspect/mode consumer promotion gate

Static candidate review completed against the accepted gate. The remaining
`FUN_14343BC61 -> FUN_14343BD9A` chain proves ratio selection/forwarding only;
it does not establish a B/C-linked presentation state change or a validated
downstream consumer boundary. `FUN_14343BD9A` therefore remains unresolved and
is not promoted to runtime. No new artifact is justified in this batch.

Result: no candidate promoted. The aspect/mode consumer branch is closed for
the current evidence set; coordinator, stable gameplay logic and cinematic
mechanisms remain frozen.

### Batch 4 — Re-arm decision

Only after Batch 3 PASS, test a second cinematic lifecycle and verify one FOV
transform per ENTER with no accumulated state.

## Risks and rollback/safe failure

- If the exit target is not observable or epsilon completion is ambiguous, keep
  gameplay suppression fail-closed and stop; do not add a timeout.
- If the isolated artifact fails setup, it must leave the executable and camera
  state unchanged.
- If runtime behavior regresses, disable only the experimental artifact and
  restore the previous stable-plus-isolated test setup.

## Stop conditions and phase gates

- Do not edit stable gameplay source before the isolated artifact passes the
  first-cycle runtime gate.
- Do not infer clean handoff from logs alone; visual and state evidence are both
  required.
- If deterministic recovery completion cannot be established from existing
  writer events, stop the coordinator design rather than adding timing logic.

## Expected final Git review

Review actual changed paths against this plan, confirm stable gameplay and
release paths are untouched, record runtime limits, and separate completed,
remaining, deferred and not-runtime-validated outcomes.
