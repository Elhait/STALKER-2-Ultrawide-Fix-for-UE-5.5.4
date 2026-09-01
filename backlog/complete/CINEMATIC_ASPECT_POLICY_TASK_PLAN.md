# CINEMATIC_ASPECT_POLICY

Status: Complete for the tested 2.0.4 policy modes. Auto and forced 16:9,
21:9 and 32:9 modes, plus Gameplay.Enabled true/false, were user-tested
successfully. Native and FovCorrection=false remain optional unvalidated
bypass scenarios.

## Objective

Replace the cinematic `AspectFix` boolean with an explicit aspect policy while
retaining an independent cinematic FOV correction switch.

## Established evidence and current state

The unified ASI has runtime-validated cinematic aspect and live-FOV boundaries
on 2.0.4. `Auto` must preserve the tested runtime-camera behavior. `Native`
must bypass the cinematic aspect hook entirely, while FOV correction in Native
mode uses the observed runtime aspect.

## Approved scope

- Add `[Cinematics] AspectRatio=Auto|Native|16:9|21:9|32:9`.
- Add `[Cinematics] FovCorrection=true|false`.
- Auto-create the new defaults when the INI is absent.
- Keep legacy `AspectFix` and `FovFix` parsing as compatibility fallback.
- Feed the resolved policy aspect into both the cinematic aspect store and FOV
  transform without changing hook locations or coordinator behavior.

## Explicit non-goals

- No new signatures, hooks, lifecycle states or renderer logic.
- No changes to gameplay correction behavior.
- No dynamic INI reload during a running game.
- No release packaging or runtime compatibility claims beyond existing evidence.

## Expected files or areas

- `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- `backlog/CINEMATIC_ASPECT_POLICY_TASK_PLAN.md`
- rebuilt test ASI and generated INI.

## Batches and validation

### Batch 1 — policy/config implementation

Add policy parsing, default generation, logging and policy-aware aspect/FOV
selection. `Native` skips aspect-hook installation.

### Batch 2 — build/static validation

Build the unified ASI, check whitespace, inspect the bounded diff and confirm
the existing signature/resolver paths remain unchanged.

### Batch 3 — runtime matrix — COMPLETED / LIMITED

User validation confirmed Auto and forced aspect modes on 2.0.4, including
32:9 cinematic framing at 2560x1440. Native and FovCorrection=false were not
required for the current release scope.

## Risks and rollback or safe-failure behavior

Invalid or missing policy values fall back to Auto. Native bypasses the aspect
hook and therefore preserves the game's original aspect store. Failed resolver
validation remains fail-closed as before. Rollback is the previous two-boolean
configuration and policy selection code.

## Stop conditions and phase gates

Stop on build failure, resolver/source changes outside the policy layer,
unexpected hook installation changes or any regression in the existing Auto
path. Do not claim forced-mode runtime support until it is tested.

## Final Git review

Perform read-only status and diff review against this plan. Do not stage,
commit or alter unrelated intentional user changes.
