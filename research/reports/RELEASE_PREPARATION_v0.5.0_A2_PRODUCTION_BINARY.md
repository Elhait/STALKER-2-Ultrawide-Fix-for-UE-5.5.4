# Release Preparation v0.5.0 — A2 Production Binary Gate

Status: PASS

## Candidate

- Candidate path: `build-artifacts/test-asi/STALKER2UltrawideFix.asi`
- Size: 1,095,168 bytes
- Last modified: 2026-09-03 19:24:11 (+03:00)
- SHA-256: `B8A05E6DA92DDE89FBB213367B02F40BE3A5AACD46DC0C09E5D29DE9B13A881E`
- Source path: `src/experimental_cinematic_21_9_combined_fix_204.cpp`
- Build script: `build-artifacts/test-scripts/build-experimental-cinematic-21x9-combined-fix-204.cmd`

The `test-asi` directory name is retained repository/build-layout provenance. It
does not by itself classify the binary as a test build.

## Source and build verification

- The build script compiles the unified production source together with the
  approved SafetyHook/Zydis dependencies.
- The build defines `/DNDEBUG` and does not define
  `COMBINED_GAMEPLAY_DIAGNOSTIC`, `GAMEPLAY_ONE_SHOT_CINEMATIC_TRIGGER` or any
  feasibility-build macro.
- The production source contains the promoted policies `Native`, `Adaptive`,
  `Reduced` and `Disabled`.
- The production source contains no `OpticalReduced` policy, feasibility-only
  policy, sample-dump diagnostic or temporary F8 binding.
- The source last-modified time (19:23:11) precedes the candidate binary
  last-modified time (19:24:11). No later source modification was observed in
  the inspected source path.
- The candidate is built by the inspected unified production build script; no
  separate diagnostic source is included by that script.

## Runtime correlation

The current canonical runtime log reports the same module SHA:

```text
modSha256=B8A05E6DA92DDE89FBB213367B02F40BE3A5AACD46DC0C09E5D29DE9B13A881E
gameSha256=2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409
```

That log also records the validated dialogue boundary installation, all
production dialogue policy cycles, smooth recovery and configurable hotkey
operation on the Steam 2.0.4 executable. This is runtime evidence, not merely
build evidence.

## A2 gate

The candidate is the unified production binary for the final inspected source
state. Its SHA is independently re-established for this release run, and the
source/build inspection found no feasibility-only policy or diagnostic build
define in the production build path.

Limit: the binary output directory is historically named `test-asi`; a future
cleanup may introduce a dedicated production output directory, but that is not
required for this gate and was not performed here.

Production Binary Gate: PASS.

Next permitted batch: A3 — Plans, Provenance and Attribution.
