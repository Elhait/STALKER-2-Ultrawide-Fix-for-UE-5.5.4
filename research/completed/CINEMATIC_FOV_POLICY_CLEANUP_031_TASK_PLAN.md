# Cinematic FOV Policy Cleanup and Dynamic Aspect Validation — v0.4.0

## Objective

Remove the public `Cinematics.FovCorrection` option and make cinematic FOV
behavior derive from `AspectRatio`, while migrating existing INI files by
removing the obsolete key and its generated comment.

## Established evidence and current state

- `AspectRatio=Native` currently bypasses the aspect store but still allows the
  FOV hook to use the runtime ultrawide aspect, producing an incorrect enlarged
  FOV in native 16:9 cinematics.
- `Auto` and forced aspect modes already share the validated cinematic aspect
  and live-FOV boundaries.
- The previous candidate could resolve the correct runtime aspect at the FOV
  boundary but read native 16:9 from the object at the later aspect-store
  boundary; the two boundaries must use the same cached runtime aspect.
- The generated INI currently contains `FovCorrection=true`.
- Existing users may have a 0.3.0 INI containing `FovCorrection=true/false`.

## Approved scope

- Update `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- Make `Native` bypass both cinematic aspect and FOV correction.
- Make FOV correction automatic for `Auto`, `21:9` and `32:9`; preserve native
  authored FOV for forced `16:9`.
- Remove `FovCorrection` from newly generated configuration.
- Migrate existing INI files by removing the obsolete key and its exact
  generated comment without rewriting unrelated content.
- Update the public README, release notes, Nexus description and GitHub release
  body to remove the obsolete option.
- Build the 0.3.1 candidate and perform source/build validation.

## Explicit non-goals

- No new RE, hook boundary or signature changes.
- No changes to gameplay transition logic, aspect formulas or cinematic
  lifecycle coordination beyond the policy decision.
- No runtime claim until a separate user test validates the candidate.
- No upload, Git staging, commit or history rewrite.

## Expected files or areas

- Unified cinematic source and its build script.
- `README.md`, `RELEASE_NOTES.md`, `NEXUS_DESCRIPTION.md` and
  `GITHUB_RELEASE_BODY.md`.
- Test ASI/INI output only as generated build artifacts.

## Batches and validation

### Batch 1 — Policy and INI migration

- Add idempotent removal of `FovCorrection` and its exact generated comment.
- Make `Native` bypass cinematic FOV transformation.
- Remove the option from generated INI and public documentation.
- Validate source references and migration behavior with focused checks.

### Batch 2 — Build and static verification

- Build the unified 0.3.1 candidate.
- Verify the generated INI contains only the supported policy fields.
- Run `git diff --check` and inspect the affected diff.

## Risks and safe failure

- Malformed or inaccessible INI: retain existing fail-safe defaults.
- User comments and unrelated keys: preserve them; remove only the obsolete
  key and exact generated comment.
- Native mode regression: do not transform authored FOV when `Native` is set.
- Build or validation failure: do not promote the candidate to release.

## Stop conditions and phase gates

- Stop if migration cannot preserve unrelated INI content.
- Stop if the source still exposes the obsolete public option.
- Stop before runtime/release promotion until the user validates `Native`,
  `Auto` and forced framing modes.

## Final review

- Compare changed paths against this plan.
- Record build/static results and the need for user runtime validation.
- Preserve existing staged and untracked user work; do not mutate Git state.

## Status

Batch 1 and Batch 2 completed. The 0.3.1 candidate builds successfully, public
configuration no longer exposes `FovCorrection`, and the source contains an
idempotent migration for the obsolete key. The `Auto` resolver now accepts
native 16:9 as a valid aspect and the cinematic store has a fail-safe native
fallback, preventing invalid zero-aspect writes. The cinematic aspect store
now uses the same cached runtime aspect as the FOV boundary in `Auto` mode, so
native post-store 16:9 does not overwrite the validated ultrawide target.
The latest candidate build SHA-256 is
`C657D9EE55C1B3951441331CA8F29525DD4ACFFB5544AEDFC8DC1703A2292D82`.
Follow-up Batch 3 is approved to make the `Auto` cache source-specific and
live-resolution aware: valid aspects from the gameplay/runtime camera,
including 16:9, may update the cache; the cinematic store remains excluded.
Batch 3 source/build work is complete. The follow-up candidate SHA-256 is
`7404D5288E8F60E925C497B055143EF97EA79798E511850DE908ABBB32376453`.
The follow-up requires one refinement: the cache must ignore the native 16:9
value produced by the fix's own Auto restore while still accepting 16:9 from a
new runtime camera source. Build and static validation passed. Runtime
validation passed with hot resolution changes in one session: 16:9 → 21:9 →
32:9 → 16:9 → 21:9 → 32:9, including cinematic EXIT recovery into gameplay.
Policy regression on the native 5120×1440 display also passed for `Native`,
forced `16:9`, forced `21:9` and forced `32:9`; user reported all views were
visually correct. Forced `21:9` uses the validated cinematic policy value
`2.33333`, while `Auto` uses the actual runtime camera aspect.
Follow-up Batch 4 changes forced `21:9` to the canonical validated PC
`3440×1440` aspect `2.3888889`, aligning it with `Auto` on that display.
Build succeeded and the focused forced-`21:9` runtime check passed on
5120×1440: `aspect=2.38889`, `FOV=106.688`, with matching aspect-store output.
The planned policy validation is complete for the tested 2.0.4 executable.
The task is complete for the validated Steam 2.0.4 / UE 5.5.4 target. The
release-level result is `v0.4.0` because the implementation adds dynamic
runtime aspect switching within one game session, not only a compatibility
bugfix.
