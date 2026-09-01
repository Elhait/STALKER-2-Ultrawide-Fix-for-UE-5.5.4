# EXPERIMENTAL_CINEMATIC_21_9_COMBINED_FIX_204

Status: Complete as the 2.0.4 unified experimental ASI baseline. Combined
gameplay/cinematic behavior and the runtime aspect policy were validated by
the user; post-EXIT presentation seam research remains separate and deferred.

## Objective

Build one unified ultrawide ASI combining the validated 2.0.4 cinematic ENTER
aspect/immediate and live-FOV transforms with the runtime-validated generalized
  gameplay ultrawide predicate for 21:9 and 32:9. Cinematic aspect must use
  the game camera/render aspect rather than the desktop dimensions exposed by
  a borderless window.

## Established evidence and current state

- Cinematic aspect immediate patch at `RVA 0x6B7CB05` and live FOV transform at
  `RVA 0x2EE6936` passed isolated 32:9 feasibility and visual validation.
- The stable gameplay regression fix passed runtime validation after changing
  the 32:9-only predicate to accept the actual ultrawide source aspect and
  preserve it during B.
- Native cinematic EXIT FOV recovery remains game-owned and must be preserved.
- Atomic post-EXIT replay scheduling was tested but did not remove the visual
  seam; the standard coordinator behavior remains the experimental baseline.
- The first combined artifact incorrectly used `desktop=5120x1440` as the
  cinematic aspect source when the game render mode was `3440x1440`; its 21:9
  cinematic black bars are therefore a known artifact defect.

## Approved scope

- Create a separate experimental source and ASI.
- Combine the existing validated cinematic mechanisms with the generalized
  gameplay aspect lifecycle.
- Preserve player FOV and native EXIT recovery.
- Use the validated cinematic aspect store boundary with runtime camera aspect
  selection; do not treat desktop/window bounds as authoritative.
- Keep fail-closed signature/RVA validation.
- Resolve the cinematic aspect/FOV boundaries through the dedicated
  cross-version structural signatures; fixed RVAs remain evidence only.
- Add a side-by-side INI configuration with independent gameplay-aspect,
  cinematic-aspect and cinematic-FOV switches. Create it automatically with
  all fixes enabled when it is missing.
- Keep runtime aspect acquisition as a shared observer layer, independent of
  whether gameplay correction itself is enabled.

## Explicit non-goals

- Do not overwrite the historical `STALKER2GameplayAspectFix.asi` binary.
- Do not include the failed atomic handoff policy as a production claim.
- No weapon/viewmodel FOV changes.
- No timers, polling, renderer hooks or new projection mechanisms.
- No new cinematic RE or changes to validated cinematic boundaries.

## Expected files or areas

- New isolated source under `src/`.
- New build script under `build-artifacts/test-scripts/`.
- New unified ASI under `build-artifacts/test-asi/` named
  `STALKER2UltrawideFix.asi`.
- Runtime configuration file beside the ASI: `STALKER2UltrawideFix.ini`.
- This plan file.

## Batches and validation

### Batch 1 — isolated source composition

Copy the standard cinematic coordinator baseline and replace only its gameplay
ultrawide predicate/B-pass target with the validated dynamic behavior. Replace
the incorrect install-time desktop aspect patch with a guarded ordinary hook at
the validated cinematic store, sourcing aspect from the current/last gameplay
camera state. Rename the log to identify the combined experimental artifact.

Completed: created `src/experimental_cinematic_21_9_combined_fix_204.cpp` from
the standard coordinator baseline. The gameplay replay uses the validated
dynamic ultrawide predicate and preserves the observed source aspect for the
constrained pass. The cinematic boundaries and standard coordinator behavior
remain unchanged. Stable source was not edited by this task.

Validation: source review; stable source remains untouched.

### Batch 2 — build validation

Compile the isolated source with the existing SafetyHook/Zydis dependencies.
Validate output exists and installation guards remain present.

Completed: build succeeded with the existing Visual Studio/SafetyHook/Zydis
toolchain. Output: `build-artifacts/test-asi/STALKER2UltrawideFix.asi`.

### Batch 2.1 — runtime-aspect correction

Approved after the first combined log showed `desktop=5120x1440` while the
game render mode was `3440x1440`. The experimental source now observes the
validated cinematic aspect store at runtime and uses the current/last valid
game camera aspect; it fails safe to native 16:9 if no valid ultrawide state is
available. The FOV transform uses the same runtime aspect.

Completed: rebuilt successfully. The artifact no longer performs an
install-time desktop/client aspect patch. It hooks the validated ten-byte
cinematic store instruction, writes the current target object's valid
ultrawide aspect (or the last observed gameplay-camera aspect), and otherwise
keeps the native 16:9 value. The FOV transform reads the same cached runtime
aspect. User runtime validation is pending.

### Batch 2.2 — user configuration

Completed: added automatic creation and parsing of `STALKER2UltrawideFix.ini`
beside the ASI. The independent switches are `[Gameplay] Enabled`,
`[Cinematics] AspectFix` and `[Cinematics] FovFix`; missing or invalid values
keep the safe default of enabled. Disabled hooks are not installed, and
configuration loading does not write game state. A default INI is included
beside the built ASI for distribution. Legacy `[Features]` keys remain readable
for compatibility with an earlier test build.

When gameplay correction is disabled while cinematic FOV remains enabled, the
validated gameplay-writer boundary is retained only as a read-only shared
aspect observer so the cinematic transform can still obtain runtime aspect.

Validation: source compiled successfully with the configuration gates.

### Batch 3 — user runtime validation

Completed: user runtime-tested the combined ASI in 21:9 and 32:9. The log
recorded runtime-camera aspect `2.38889` with cinematic ENTER FOV `106.688` for
21:9, and aspect `3.55556` with ENTER FOV `126.87` for 32:9. Both modes showed
the expected gameplay constrained/restore lifecycle after EXIT and were
visually correct. The desktop `5120x1440` aspect was not used as the cinematic
authoritative value.

Known deferred issues: the previously classified post-cinematic presentation
seam and the separate 21:9 weapon/viewmodel FOV behavior after load. They are
not part of this artifact's accepted gameplay/cinematic aspect contract.

Validation: build and user runtime validation completed. The unified artifact
is suitable as a new release candidate under the name `STALKER2UltrawideFix.asi`;
the cinematic portion remains documented as experimental and the historical
stable binary is retained separately.

### Batch 3.1 — cinematic signature resolution

In progress: the unified source now resolves the validated cinematic aspect
setter and ENTER/EXIT FOV callsites from unique structural signatures rather
than using the previously validated 2.0.4 RVAs directly. The resolver decodes
the aspect store and FOV boundaries and requires both FOV callsites to target
the same consumer. The available 2.0.4 executable contains one aspect-setter
signature match and the source builds successfully.

The first runtime attempt failed closed because the initial FOV signature
shape was too strict and did not match the actual current argument setup. The
pattern was corrected from the instruction-level 2.0.4 bytes, and the unified
log is now `STALKER2UltrawideFix.log`.

Validation remaining: user runtime test of the corrected resolver build at
21:9 and 32:9. This is not yet evidence of compatibility with an untested
game patch.

## Risks and rollback or safe failure

- Cinematic and gameplay hooks in one ASI could conflict; installation must
  remain guarded and refuse safely on mismatch.
- Native EXIT recovery must not be manually overwritten.
- Disable/remove only the experimental ASI to roll back; stable source and
  release assets remain available.

## Stop conditions and phase gates

- Stop on build failure, signature mismatch or missing validated instruction
  bytes.
- Stop if the combined source changes player FOV outside the cinematic live
  transform or alters native EXIT recovery.
- Do not promote to release until the user completes runtime regression tests.

## Final review requirements

- Review Git status and relevant diff after build.
- Confirm only the planned experimental files were added/changed by this task.
- Report build status, runtime-validation limits and the separate deferred
  weapon/viewmodel FOV issue.
