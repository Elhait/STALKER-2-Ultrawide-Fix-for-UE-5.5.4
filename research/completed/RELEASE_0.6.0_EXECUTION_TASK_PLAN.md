# STALKER 2 Ultrawide Fix 0.6.0 — Release Preparation Execution Plan

## Objective

Complete the release-preparation playbook for 0.6.0 using the validated
production `STALKER2CameraTweaks.asi`, without publishing, committing or
changing production behavior.

## Established evidence and current state

- Production ASI runtime-tested on Steam 2.0.5.
- Production SHA-256:
  `A0A5D0823C79B3A6E785F7244439402E5CED04CE37CE670F75FE8CCFE1932372`.
- Gameplay atomic apply and post-cinematic `RecoveryStart` handoff passed.
- Native FOV recovery remains untouched; native bypass research is deferred.
- Resolver portability was statically validated on Steam 2.0.2–2.0.5;
  runtime support is claimed only for 2.0.5.

## Approved scope

- Execute release playbook phases A0–A8 for version 0.6.0.
- Align claims, source documentation, release assets and archive contents.
- Record evidence, limits and Git review results.

## Explicit non-goals

- No source or production ASI behavior changes.
- No new runtime research or Ghidra analysis.
- No publication, commit, tag, upload or force-push.
- No deletion of historical research or test artifacts.

## Expected files and areas

- `README.md`, `RELEASE_NOTES.md`, `TESTING_AND_RESEARCH.md`,
  `GITHUB_RELEASE_BODY.md`, `NEXUS_DESCRIPTION.md`.
- `release-assets/` README, INI, license/notices, production ASI and archive.
- `backlog/TASKLOG.md` and release-preparation evidence.

## Batches and validation

1. A0–A1: lock release scope and build an evidence/claim matrix from current
   plans, runtime log and binary identity.
2. A2–A3: verify production binary correspondence, diagnostic exclusion,
   provenance, plan classification and attribution.
3. A4: align all release-facing documentation; validate version, claims,
   limitations and supported versions.
4. A5–A6: verify release assets, construct the archive and extract it for an
   exact-content audit.
5. A7–A8: perform final consistency and read-only Git review.

Each batch requires a bounded report before the next dependent batch begins.

## Risks and safe failure

- Stale claims, mismatched assets or unverifiable binary provenance block the
  dependent batch and are corrected only in their owning scope.
- Existing user changes must be preserved; unexpected paths stop the review.
- Archive validation must fail closed if test ASIs, tracers or research files
  are included.

## Stop conditions and phase gates

- Do not mark a phase PASS without its required evidence.
- Stop on identity mismatch, source/binary mismatch, stale mandatory claim,
  unexpected production change or unsafe archive content.
- Stop before publication or Git state changes unless explicitly requested.

## Final review

- Compare every changed path with this plan.
- Run read-only `git status`, relevant diff review and `git diff --check`.
- Report completed, remaining, deferred, blocked and not-runtime-validated
  items; archive this plan only after all approved phases are complete.

## Current phase status — 2026-09-12

- A0 Scope lock: PASS.
- A1 Evidence/claim review: PASS with documentation mismatches recorded.
- A2 Production binary gate: PASS.
  - Runtime-tested game binary: SHA-256
    `A0A5D0823C79B3A6E785F7244439402E5CED04CE37CE670F75FE8CCFE1932372`.
  - Replaced the stale release-assets copy with the exact runtime-tested
    binary; source and destination now both hash to
    `A0A5D0823C79B3A6E785F7244439402E5CED04CE37CE670F75FE8CCFE1932372`.
  - Define audit classified the test-named gates as stale names for validated
    production paths; no rename or rebuild was performed.
- A3 Plans/provenance/attribution: PASS.
  - The completed production integration plan is archived under
    `research/completed`.
  - Native FOV bypass and other unfinished research remain separated from the
    production/release scope.
  - Attribution is present in the user-facing documentation and
    `THIRD_PARTY_NOTICES.md`; no stale moved-plan reference was found.
- A4 Documentation: PASS.
  - README, release notes, testing summary, GitHub release body and Nexus
    publication text are aligned with the 0.6.0 evidence boundary.
  - Runtime 2.0.5 and static-only 2.0.2–2.0.5 claims are separated.
- A5 Release assets: PASS.
  - Release directory contains only the five intended current files.
  - Historical ZIPs were preserved under `build-artifacts/archive` and are
    excluded from the release set.
- A6 Archive construction/extraction: PASS.
  - Created `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.6.0.zip`.
  - Archive contains exactly the five intended files and extracted ASI SHA
    matches the authoritative `A0A5...` binary.
- A7 Final consistency audit: PASS.
  - Release-facing documents and release-assets metadata contain no stale
    0.5.x claims or pending production-validation wording.
  - Archive content, extraction and authoritative ASI SHA are consistent.
- A8 Git review: PASS.
  - Read-only status/diff review completed against the approved scope.
  - Release-facing diff check passed; unrelated pre-existing work was
    preserved and not included in the release batch.

## Final status — 2026-09-12

Release preparation A0–A8: COMPLETE.

The 0.6.0 package is ready for user review. Publication, commit, tag and
upload remain intentionally outside this execution pass.
