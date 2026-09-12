# Release Preparation Task Plan — v0.5.2

## Objective

Prepare a verified `STALKER2CameraTweaks` v0.5.2 release archive from the
current unified ASI production source and current validated binary.

## Established evidence and current state

- The unified source is `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
- The production build is `STALKER2CameraTweaks.asi`; the old gameplay-only
  output is superseded and must not be packaged.
- Runtime evidence supplied for the current game update records successful
  gameplay, cinematic and dialogue hook installation on Steam build 2.0.5.
- Static resolver evidence covers Steam builds 2.0.2, 2.0.3 and 2.0.4; this is
  portability evidence, not runtime validation for those older builds.
- No new camera behavior is being implemented in this release-preparation
  task.

## Approved scope

- Update release-facing version and compatibility wording to v0.5.2.
- Update the packaged default INI header if required.
- Build the unified production ASI from the current production source.
- Create and verify `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.5.2.zip`.
- Record binary/archive hashes and preparation evidence.

## Explicit non-goals

- No gameplay, cinematic or dialogue logic changes.
- No research instrumentation or experimental ASI packaging.
- No deletion of old release archives or repository artifacts.
- No Git add, commit, tag, push or publication to GitHub/Nexus.
- No claim of runtime support for versions not covered by supplied evidence.

## Files or areas expected to be touched

- `README.md`
- `RELEASE_NOTES.md`
- `GITHUB_RELEASE_BODY.md`
- `NEXUS_DESCRIPTION.md`
- `release-assets/README.md`
- `release-assets/STALKER2CameraTweaks.ini`
- `release-assets/STALKER2CameraTweaks.asi`
- `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.5.2.zip`
- `research/reports/RELEASE_PREPARATION_v0.5.2.md`
- `backlog/TASKLOG.md`
- build output generated from `build.cmd`

## Batches

### A0 — Scope and evidence lock

Confirm the v0.5.2 delta, current executable/runtime evidence and supported
wording before editing release-facing files.

### A1 — Release documentation and INI metadata

Update only version, release delta and evidence/compatibility wording. Preserve
the existing configuration surface and defaults.

### A2 — Production binary gate

Build the unified ASI, verify it contains no research-only output, record its
SHA-256 and associate it with the supplied Steam 2.0.5 runtime log.

### A3 — Release asset construction

Refresh the packaged ASI, INI and supporting documents, then create the v0.5.2
archive with only the intended release files.

### A4 — Archive and final consistency review

Extract/list the archive, verify exact contents, compare packaged files with
the selected source assets, compute the archive hash and perform a read-only
Git review against this plan.

## Validation

- Documentation/version search finds no stale v0.5.0/v0.5.1 release header in
  current release-facing files.
- Build command completes successfully and produces the expected ASI name.
- ASI SHA-256 is recorded separately from the archive SHA-256.
- Archive contains exactly the five intended release files and no debug,
  object, log, test or experimental files.
- Runtime claim remains limited to supplied Steam 2.0.5 evidence; static
  cross-patch claims remain explicitly separate.
- Final `git status`, diff summary and changed-path review match this plan.

## Risks and rollback / safe failure

- Documentation may overstate compatibility; resolve by retaining separate
  runtime and static-validation wording.
- Packaging the wrong ASI is mitigated by source correspondence and hash
  checks before archive creation.
- If build, contents or consistency validation fails, stop without publishing
  and leave the prior release artifacts untouched.

## Stop conditions and phase gates

- Stop before packaging if the production source/binary correspondence is
  unclear.
- Stop if the supplied runtime evidence cannot support the proposed public
  claim.
- Stop if archive contents differ from the six-file allowlist.
- Phase A is `READY FOR USER COMMIT/PUBLISH APPROVAL` only after A0–A4 pass.

## Expected final Git review

Inspect repository status, relevant diff/statistics, changed paths and recent
commits. Report completed, remaining, deferred, blocked and not-runtime-
validated items. Do not commit or publish.
