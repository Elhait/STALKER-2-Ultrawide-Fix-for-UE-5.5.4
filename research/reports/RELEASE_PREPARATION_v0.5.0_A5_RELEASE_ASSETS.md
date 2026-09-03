# Release Preparation v0.5.0 — A5 Release Assets

Status: PASS

## Approved asset set for A6

The current production asset set is:

- `release-assets/STALKER2UltrawideFix.asi`
- `release-assets/STALKER2UltrawideFix.ini`
- `release-assets/README.md`
- `release-assets/LICENSE.md`
- `release-assets/THIRD_PARTY_NOTICES.md`

The release ASI is byte-identical to the A2 candidate:

```text
SHA-256: B8A05E6DA92DDE89FBB213367B02F40BE3A5AACD46DC0C09E5D29DE9B13A881E
```

## Asset validation

- All five required production files are present.
- The distributed INI is v0.5.0 and uses the approved defaults:
  `Gameplay.Enabled=true`, `Cinematics.AspectRatio=Auto`,
  `Dialogue.Zoom=Reduced` and `Hotkeys.Enabled=false`.
- The INI contains the production dialogue cycle
  `Native -> Adaptive -> Reduced -> Disabled -> Native`.
- The INI contains the production cinematic cycle and supported binding range
  documentation; defaults remain `F9` and `F10`.
- INI section spacing and comments were normalized for the release asset.
- No `OpticalReduced`, feasibility-only option, diagnostic option or temporary
  binding remains in the release INI or compact release README.
- The compact release README reflects v0.5.0 dialogue modes, hotkey behavior,
  runtime resolution scope, restart behavior and attribution.
- `LICENSE.md` and `THIRD_PARTY_NOTICES.md` are retained.

Historical ZIP files remain in `release-assets` untouched and are explicitly
excluded from the A6 input set. No archive was created or modified in A5.

## A5 gate

The selected release asset set matches the A2 production binary and A4
documentation. Test ASIs, tracers, logs, source, research and historical ZIPs
are excluded from the A6 archive input. The only asset correction was the
release INI's formatting/comment normalization.

Release Assets: PASS.

Next permitted batch: A6 — Archive construction and extraction.
