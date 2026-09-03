# Release Preparation v0.5.0 — A7 Final Consistency Audit

Status: PASS

## Cross-boundary checks

- Release-facing documents and the A5 release README identify v0.5.0 and the
  unified `STALKER2UltrawideFix.asi`.
- Public policy names are consistently `Native`, `Adaptive`, `Reduced` and
  `Disabled`; `Zoom=Reduced` is the documented/default production setting.
- `Adaptive` and optical half-strength `Reduced` are described consistently.
- F9/F10 are optional next-lifecycle hotkeys, disabled by default, with the
  supported key range documented. No temporary F8 binding is documented.
- Manual INI edits require restart; runtime hotkey selection does not.
- `AspectRatio=Auto` same-session resolution behavior is documented as
  validated; no stale contrary limitation remains.
- Runtime claims are limited to Steam 2.0.4, while dialogue resolver static
  portability is separately limited to Steam 2.0.2–2.0.4.
- Weapon/viewmodel FOV remains documented as a separate known issue and is not
  claimed as fixed.
- Attribution distinguishes Lyall scaffolding, WIDEBOY research reference,
  Sota semantic reference and vendored dependencies.
- No feasibility-only policy, diagnostic term or test artifact appears in the
  release-facing documentation, release README or production INI.

## Immutable payload checks

- Approved ASI SHA-256: `B8A05E6DA92DDE89FBB213367B02F40BE3A5AACD46DC0C09E5D29DE9B13A881E`.
- A6 archive SHA-256: `A41E973187AECF5AE4F02885C6E5ED3E32318E72ED08A2E4B2DA1616DB6E76E2`.
- Archive extraction contains exactly:

  ```text
  LICENSE.md
  README.md
  STALKER2UltrawideFix.asi
  STALKER2UltrawideFix.ini
  THIRD_PARTY_NOTICES.md
  ```

- Extracted ASI is byte-identical to the A2/A5 approved binary.
- The historical ZIPs remain excluded from the new archive.
- `git diff --check` completed without whitespace errors; only Git's existing
  LF/CRLF conversion warnings were reported.

## A7 gate

The A1 claim matrix, A2 binary, A3 provenance, A4 documentation, A5 whitelist
and A6 extracted archive are mutually consistent. This was an audit-only pass;
no content was repaired during the audit.

Final consistency audit: PASS.

Next permitted batch: A8 — Git pre-commit review.
