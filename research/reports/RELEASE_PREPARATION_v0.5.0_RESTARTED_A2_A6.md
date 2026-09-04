# v0.5.0 Release Preparation — A2–A6 Automated Run

Date: 2026-09-04
Scope: renamed production identity and release payload verification only.

## A2 — Production binary

Status: PASS

- Candidate: `release-assets/STALKER2CameraTweaks.asi`
- SHA-256: `90CFF6F4B641F365496179EBF21490EDDD07781E72F3CCE0A47592E4B570F92F`
- Build configuration uses `/DNDEBUG` and does not define
  `FOV_SETTINGS_TRACE_DIAGNOSTIC`.
- Public production policy names contain `Native`, `Adaptive`, `Reduced` and
  `Disabled`; feasibility-only `OpticalReduced` is not present.
- Runtime evidence is reused from the preceding production-style build with
  the same dialogue invalidation and EXIT-recovery logic. The final renamed
  candidate differs only by the finalized technical identity/header change;
  that exact binary was not separately launched in this batch.

## A3 — Provenance and attribution

Status: PASS

- Reused completed dialogue lifecycle, optical-model, cinematic-isolation and
  cross-patch resolver evidence from the accepted A1/A3 records.
- Rename and final documentation updates introduce no new external reference
  claim or attribution category.
- Historical old-name references remain historical provenance and were not
  globally rewritten.

## A4 — Documentation

Status: PASS after correction

- Corrected the stale production-candidate hash in
  `TESTING_AND_RESEARCH.md` to the renamed candidate SHA above.
- Current documentation retains the approved four-policy dialogue contract,
  next-lifecycle hotkey semantics, opt-in hotkeys, runtime/static version
  boundaries and deferred weapon/viewmodel/Panini scope.

## A5 — Release assets

Status: PASS

Approved whitelist:

```text
STALKER2CameraTweaks.asi
STALKER2CameraTweaks.ini
README.md
LICENSE.md
THIRD_PARTY_NOTICES.md
```

- Release README was synchronized with the current renamed repository README.
- INI defaults: `Zoom=Reduced`, hotkeys disabled by default, `F9`/`F10`
  bindings retained.
- Historical archives remain outside the whitelist.

## A6 — Archive and extraction verification

Status: PASS

- Archive:
  `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.5.0.zip`
- ZIP SHA-256:
  `C99957D2F2158B7C15EBC25F806E0CDD7BD9536D4DF0A91930F632596918DC9E`
- Clean extraction produced exactly the five approved files.
- Extracted ASI SHA-256 matches the A2/A5 candidate:
  `90CFF6F4B641F365496179EBF21490EDDD07781E72F3CCE0A47592E4B570F92F`
- No nested archive, log, source, object, test or research artifact was
  included.

## Batch result

```yaml
A2: PASS
A3: PASS
A4: PASS
A5: PASS
A6: PASS
Overall: PASS
Next checkpoint: A7 final consistency audit
```

No staging, commit, tag or publication was performed.
