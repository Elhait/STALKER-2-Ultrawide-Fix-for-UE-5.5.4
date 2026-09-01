# Nexus Description 0.3.0 Task Plan

## Objective

Create a reusable, current Nexus Mods description for `STALKER2UltrawideFix`
version `0.3.0`, replacing the obsolete gameplay-only 2.0.3 text.

## Established evidence and current state

- The unified artifact is `STALKER2UltrawideFix.asi`.
- The tested game target is Steam build 2.0.4 with UE 5.5.4.
- Gameplay and cinematic runtime evidence is recorded in the project testing
  summary and release documentation.
- The old Nexus text describes `STALKER2GameplayAspectFix.asi`, Steam 2.0.3
  and an earlier gameplay-only scope.

## Approved scope

- Create the reusable root `NEXUS_DESCRIPTION.md`.
- Document installation, configuration, tested scope, limitations, support and
  the Defender false-positive notice.
- Keep wording suitable for copying into a Nexus description and future edits.

## Explicit non-goals

- No source, ASI, INI or release archive changes.
- No upload or external Nexus edit.
- No new runtime testing or compatibility claim beyond the validated 2.0.4
  evidence.

## Validation and stop conditions

- Confirm the file uses `0.3.0`, `STALKER2UltrawideFix.asi`, Steam 2.0.4 and
  UE 5.5.4.
- Confirm obsolete 2.0.3/gameplay-only claims are absent except where the old
  filename is intentionally mentioned for upgrade instructions.
- Run `git diff --check` for the new documentation.

## Final review

- Review the created file against this plan.
- Preserve all existing staged and untracked user work.
- Do not stage, commit, upload or alter Git history.
