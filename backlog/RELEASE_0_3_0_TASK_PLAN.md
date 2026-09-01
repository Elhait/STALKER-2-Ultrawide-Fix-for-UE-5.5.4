# RELEASE_0_3_0

Status: Complete for packaging. README, release files and the clean 0.3.0
archive were created and verified; publication/upload remains outside scope.

## Objective

Update the release README and create the `0.3.0` archive for the unified
`STALKER2UltrawideFix.asi`.

## Established evidence and current state

The unified ASI has user-supplied runtime validation on Steam 2.0.4 for
gameplay 21:9/32:9 behavior, cinematic aspect/FOV handling and custom forced
cinematic framing. Its current build identity must be recorded from the
release artifact, while future-patch support must not be claimed.

## Approved scope

- Inspect and update `release-assets/README.md`.
- Package only the intended `STALKER2UltrawideFix.asi`,
  `STALKER2UltrawideFix.ini` and README in the new `0.3.0` archive.
- Verify archive contents, hashes and documentation consistency.

## Explicit non-goals

- No source or build-script changes.
- No changes to existing historical release archives.
- No Git staging, commit, upload or Nexus publication.
- No new runtime testing.

## Expected files or areas

- `release-assets/README.md`
- `release-assets/STALKER2UltrawideFix.asi`
- `release-assets/STALKER2UltrawideFix.ini`
- `release-assets/LICENSE.md`
- `release-assets/THIRD_PARTY_NOTICES.md`
- new `release-assets/STALKER2UltrawideFix-v0.3.0.zip`

## Batches and validation

### Batch 1 — release inventory

Confirm the current ASI, INI, README and existing archives. Compute the ASI
SHA-256 for the README and release record.

Completed: current unified ASI and INI were identified; ASI SHA-256 is
`949B61998A49FB04276D91B64BC5D3F087989999CA2779ACD8C703E97DBF7607`.

### Batch 2 — README update

Document installation, configuration, gameplay/cinematic features, custom
framing, tested 2.0.4 scope, known weapon/viewmodel issue and compatibility
limits.

### Batch 3 — archive creation

Create a clean `0.3.0` archive containing only the ASI, INI, README, license
and third-party notices.

Completed: `release-assets/STALKER2UltrawideFix-UE5.5.4-v0.3.0.zip` created
with exactly the five intended release files.

### Batch 4 — final review

Inspect archive listing and hashes, run `git diff --check`, and perform a
read-only Git review. Do not upload or modify Git state.

Completed: archive listing, file hashes and `git diff --check` verified;
publication was not performed.

## Risks and rollback or safe-failure behavior

The archive must not include research ASIs, logs, object files, source files or
historical test artifacts. If the release artifact is missing or ambiguous,
stop rather than packaging a substitute. Existing archives remain untouched.

## Stop conditions and phase gates

Stop if the ASI/INI cannot be identified, if README claims exceed supplied
2.0.4 evidence, if the archive contains unintended files or if an existing
archive would be overwritten.

## Final Git review

Compare changed paths with this plan, identify intentional pre-existing user
changes, confirm only release documentation/archive paths were added or
updated, and report that no upload was performed.
