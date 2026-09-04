# v0.5.0 Release Preparation — A7 Final Consistency Audit

Date: 2026-09-04
Mode: audit-only; no content, staging or release-payload edits performed.

## Cross-boundary checks

- A0/A1 scope and claims match the renamed product identity and the current
  four-policy dialogue contract.
- A2 candidate identity is carried consistently into A5 assets and A6:
  `STALKER2CameraTweaks.asi`, SHA-256
  `90CFF6F4B641F365496179EBF21490EDDD07781E72F3CCE0A47592E4B570F92F`.
- A4 documentation consistently describes `Native`, `Adaptive`, `Reduced` and
  `Disabled`, with `Reduced` as the default and hotkeys disabled by default.
- F9/F10 are documented as selecting the next applicable cinematic/dialogue
  lifecycle; manual INI edits require restart while runtime hotkey selections
  do not.
- Runtime claims are limited to Steam 2.0.4; static resolver validation is
  limited to Steam 2.0.2–2.0.4. Older runtime support is not claimed.
- Weapon/viewmodel FOV and Panini remain outside the v0.5.0 fixed scope.
- Old product names appear only where installation guidance tells users to
  remove superseded files; historical provenance is not treated as current
  identity.
- Feasibility-only `OpticalReduced`, diagnostic terminology and temporary F8
  bindings are absent from current/public/release surfaces.

## Release payload

- Archive:
  `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.5.0.zip`
- ZIP SHA-256:
  `C99957D2F2158B7C15EBC25F806E0CDD7BD9536D4DF0A91930F632596918DC9E`
- Clean extraction contains exactly:
  `LICENSE.md`, `README.md`, `STALKER2CameraTweaks.asi`,
  `STALKER2CameraTweaks.ini`, `THIRD_PARTY_NOTICES.md`.
- Extracted ASI is byte-identical to the A2/A5 candidate.
- No logs, source, object files, test builds, research files or nested archives
  are present in the payload.

## Evidence boundary

The exact renamed ASI SHA above is build/package validated. Runtime dialogue
invalidation and EXIT-recovery evidence comes from the immediately preceding
production-style build with unchanged functional logic; the final renamed
candidate was not separately launched in this audit. This is retained as an
explicit limitation and is not promoted to an exact-binary runtime claim.

## Result

```yaml
A7: PASS
Mode: audit-only
Inconsistencies: none found
Edits performed by A7: none
Next checkpoint: A8 Git scope/diff review
```

No commit, tag, push or publication was performed.
