# Cinematic Manual FOV/Aspect Control Probe 2.0.4 — Task Plan

## Objective

Provide a bounded manual diagnostic probe for comparing cinematic FOV/aspect write order in the current 2.0.4 build, with dynamic baseline capture and an explicit restore control.

## Established evidence and current state

- 2.0.4 executable identity: SHA-256 `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- The current cinematic ENTER transition resolves `item+0x18` to the authoritative inner camera state and changes aspect/mode state.
- A one-shot aspect write survives the cinematic interval and native EXIT handoff.
- `+0x230` accepts a reversible one-shot FOV write, but its visual role remains unresolved; the combined `+0x230` Hor+ test did not produce correct cinematic FOV.
- Native EXIT `FUN_146B6C482` and gameplay handoff are already validated.

## Approved scope

- Create a separate manual experimental ASI for current 2.0.4.
- Use only the already validated cinematic ENTER/EXIT lifecycle and resolved inner camera object.
- Capture `+0x230`, `+0x254` and `+0x259` baseline on ENTER; do not assume a fixed FOV.
- Provide manual controls for one-shot FOV/aspect changes and order comparison:
  - `F1`: FOV only to `127.3927`.
  - `F2`: aspect only to `3.5555556`.
  - `F3`: FOV then aspect in one handler.
  - `F4`: aspect then FOV in one handler.
  - `F5`: aspect only to native `1.7777778`.
  - `F6`: restore captured baseline FOV/aspect; flags remain game-owned.
  - `F7`: read-only state snapshot.
  - `F8`: baseline FOV plus wide aspect.
- Log each action with before/requested/after state, thread, object identity and lifecycle sequence.

## Explicit non-goals

- No timers, polling, frame delays, VEH, PAGE_GUARD or exception-based instrumentation.
- No manual writes to flags `+0x259`.
- No automatic correction on ENTER; all writes are manual controls only.
- No changes to `STALKER2GameplayAspectFix.asi`, production source or release assets.
- No claim that a visual lever is causal from logs alone; visual outcome must be reported separately by the user.

## Expected files or areas

- New research source under `src/`.
- New build helper under `build-artifacts/test-scripts/`.
- New experimental `.asi` under `build-artifacts/test-asi/`.
- This task plan and later scoped evidence entry.

## Batches

### Batch 1 — Manual probe artifact

- Resolve and validate current 2.0.4 ENTER/EXIT anchors.
- Capture baseline dynamically on ENTER.
- Install only manual hotkey controls and bounded state logging.
- Build and inspect the artifact; no runtime claim from build success.

Batch 1 result: created `src/cinematic_manual_fov_aspect_control_probe_204.cpp` and `build-artifacts/test-scripts/build-cinematic-manual-fov-aspect-control-probe-204.cmd`. The probe validates the current 2.0.4 ENTER signature and EXIT callback bytes, captures dynamic FOV/aspect baseline on ENTER, registers F1–F8 through an event-driven Windows hotkey loop, leaves flags untouched, and performs only explicit manual writes plus EXIT/F6 restore. It contains no VEH, PAGE_GUARD, timer or polling loop. The build succeeded and produced `build-artifacts/test-asi/STALKER2CinematicManualFovAspectControlProbe204.asi`. This is build/setup evidence only; no runtime or visual result is established yet.

Batch 2 runtime result: the isolated manual run captured one stable target with baseline `fov=90.65574`, `aspect=3.5555556`, `flags=0x05`. F1–F5 writes and F8/F6 restore operations all applied successfully, and flags remained game-owned at `0x05`; the user reports that each tested variant produced a visible change. The log confirms deterministic write plumbing and same-target correlation, but does not isolate write-order causality because the apparent `F5` then `F1` sequence included `F7` and `F6` between them. Visual framing remains user-observed evidence; a direct `F5 -> F1` comparison without restore is still pending. No production integration is authorized.

Batch 2 follow-up result: the direct `F5 -> F1` sequence was captured. `F5` set aspect to `1.7777778` while flags remained `0x05`; the following F1 set FOV to `127.3927`, producing only a wider 16:9 view. Separate F3 (`FOV -> aspect`) and F4 (`aspect -> FOV`) tests both produced the desired cinematic framing and converged to the same final state `127.3927 / 3.5555556`; no visual order difference was reported. This rejects write ordering as the critical factor and positively validates the combined direct-field state as the practical cinematic correction in this manual probe. Final EXIT restore returned the target to the captured baseline. No production integration is authorized.

### Batch 2 — Isolated manual runtime test

- Enable only this manual probe.
- Disable stable gameplay and all other diagnostic ASIs.
- Use one cinematic and compare F1–F8 at a stable camera moment, restoring with F6 before leaving the test.
- Record visual framing and transition behavior separately from the log.

### Batch 3 — Candidate decision

- Promote only a write order/target whose visual effect is reproducible and whose restore is clean.
- If no control changes framing, close this manual hypothesis without broadening the search.

## Validation

- Batch 1: build success, unique ENTER signature, executable EXIT anchor, bounded hotkey handling and no writes outside explicit controls.
- Batch 2: log must show dynamic baseline, requested writes, before/after state and restore; visual conclusions require the user’s observation.
- No stable gameplay or release validation is implied.

## Risks and safe failure

- Manual writes may affect the active cinematic camera; F6 restores only the probe-owned baseline.
- If ENTER identity or baseline is unavailable, controls remain inactive and the ASI logs setup refusal.
- The probe must avoid writing flags so native lifecycle ownership remains intact.

## Stop conditions and phase gates

- Stop after one bounded manual runtime session.
- Do not add repeated writes, timers, polling or automatic corrections.
- Do not merge this artifact into the stable gameplay ASI.
- Do not start production integration until a separate design/review pass.

## Final review

- Classify each control as observed, inconclusive or rejected.
- Compare changed paths with this plan.
- Perform read-only Git review after source/build changes.
- Keep the experimental artifact separate from stable release output.
