# Superseded Release Preparation Report — v0.5.0

Status: SUPERSEDED — NOT A VALID RELEASE-PREPARATION RESULT

This report is retained only as historical provenance. By explicit user
decision, the earlier release-preparation pass is treated as nonexistent. Its
gate results, archive hash, documentation status and readiness wording must not
be reused. The only accepted prior action was task-plan path organization.

The v0.5.0 release preparation must restart from A0 using the current gated
playbook. Any archive created during the superseded pass is stale until the new
run independently reaches A6/A7.

## Scope

Prepared the v0.5.0 release assets for the unified gameplay, cinematic and
dialogue FOV feature set. Commit, tag, GitHub publication and Nexus publication
were not performed.

## Approved production binary

- File: `build-artifacts/test-asi/STALKER2UltrawideFix.asi`
- SHA-256: `B8A05E6DA92DDE89FBB213367B02F40BE3A5AACD46DC0C09E5D29DE9B13A881E`
- Runtime validation: Steam 2.0.4; Native, Adaptive, Reduced and Disabled
  dialogue policies; sequential dialogue cycles; cinematic isolation; hotkeys.
- Production build command enables no feasibility/test-only macros. The binary
  contains no `OpticalReduced` policy, feasibility diagnostics or EXIT sample
  dump markers; `F8` is present only in the supported key-name table.

## Release archive

- File: `release-assets/STALKER2UltrawideFix-UE5.5.4-v0.5.0.zip`
- SHA-256: `C4BC17AD74B65DF05280000D77A9B15E7CBE029DEADB00C0AD4471BA169C3865`
- Contents: ASI, default INI, README, LICENSE and third-party notices.
- The archive was extracted and its ASI hash matched the approved binary.

## Documentation and configuration

- Public release documents were synchronized to v0.5.0.
- Dialogue modes are documented as `Native`, `Adaptive`, `Reduced` and
  `Disabled`; default is `Reduced`.
- Hotkeys are documented as optional testing controls and disabled by default;
  defaults are `F9` for cinematic and `F10` for dialogue.
- Existing release archives were preserved.

## Task-plan paths

- Completed dialogue/config/cross-patch plans were moved to
  `research/completed/`.
- Open research plans were moved to `backlog/active/`.
- The unresolved `ADS_OUTPUT38` branch was moved to `research/deferred/`.
- The reusable `backlog/RELEASE_PREPARATION_TASK_PLAN.md` remains in place.

## Remaining decision

The repository contains pre-existing staged research/build changes outside the
release asset set. They were preserved and not silently cleaned. The physical
task-plan moves are complete, but the Git index still contains earlier staged
root-path entries; reconcile that staging explicitly before committing.
