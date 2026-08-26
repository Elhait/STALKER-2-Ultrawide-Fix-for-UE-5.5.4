# Experimental Letterbox Task Plan

Status: Closed — runtime validation stopped after the experimental ASI caused an access violation during a cutscene.

## Current status

**Stopped after failed runtime validation.** The experimental ASI must not be used in-game in its current form.

The implementation reached the first runtime test, but `STALKER2ExperimentalLetterboxFix.asi` caused an `EXCEPTION_ACCESS_VIOLATION` reading `0xffffffffffffffff` during a cutscene. Removing the experimental ASI restored normal cutscene operation.

No runtime compatibility or letterbox behavior is confirmed.

## Established evidence

- BigChenga reference evidence identified two letterbox setter signatures, A and B.
- Static analysis of the Steam 2.0.2 executable found unique A/B matches.
- Both targets decode as:

  `MOV dword ptr [RAX+0x254], 0x3FE38E39`

- A and B are adjacent entrypoints with the same setter sequence and different return contracts.
- Their common class owner and upstream caller relationship remain unproven.
- The stable gameplay ASI works without the experimental letterbox ASI.
- After a cutscene, the stable gameplay fix restores aspect state, but weapon FOV can remain incorrect until opening and closing the pause menu forces a camera refresh. This is a separate gameplay-camera timing issue.

## Failed implementation

Files created for the experimental implementation:

- `src/cutscene_letterbox_fix.cpp`
- `build-letterbox.cmd`

The implementation attempted to:

- resolve and decode both A/B targets dynamically;
- install no hooks until both targets were validated;
- update one shared display-aspect state from a worker thread;
- apply the display aspect after the original setter write;
- roll back hook A if hook B installation failed.

The current design placed the A callback at the `RET` immediately following the setter. The runtime crash makes this hook boundary unsafe or otherwise invalid for the current SafetyHook/runtime combination. Do not retest this binary without redesigning the hook placement.

## Next task: redesign before implementation

1. Remove or keep `STALKER2ExperimentalLetterboxFix.asi` disabled during all tests.
2. Re-audit safe hook boundaries for A and B in the exact executable build.
3. Do not hook directly on `RET` or across an unproven function boundary.
4. Establish the exact A/B function entry and return ABI.
5. Select a hook design that preserves each original return contract and executes the original setter exactly once.
6. Reconfirm context safety: use only the validated target object, avoid unproven dereferences, and preserve registers and flags outside the hook contract.
7. Preserve all-or-nothing installation: resolve and decode both targets first; roll back any installed hook if the second installation fails.
8. Add a static rejection test for zero matches, multiple matches, wrong decode and unsafe hook-boundary detection.
9. Build a new experimental ASI only after the redesign is reviewed.
10. Perform manual testing with the stable gameplay ASI isolated from the experimental ASI.

## Explicit non-goals

- No changes to stable gameplay aspect logic in this task.
- No cutscene FOV or dialogue FOV changes.
- No hard-coded weapon FOV multiplier.
- No claim of compatibility beyond the analyzed executable/build identity.
- No release packaging or stable ASI integration.
- No final `TASKLOG.md` entry or final Git review until the redesigned implementation has passed manual testing.

## Rollback and stop conditions

- If either signature is ambiguous, decode validation fails, or a safe hook boundary cannot be proven: refuse installation.
- If any hook installation fails: remove every previously installed experimental hook.
- If any cutscene crash, invalid context, or incorrect return behavior recurs: disable the experimental ASI and stop the phase.
