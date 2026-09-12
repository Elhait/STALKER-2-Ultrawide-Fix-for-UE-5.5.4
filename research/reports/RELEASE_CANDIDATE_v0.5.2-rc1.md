# STALKER2CameraTweaks v0.5.2-rc1

## Disposition

The current build is recorded as a stable release candidate, not a final release.

## Candidate identity

- ASI: `STALKER2CameraTweaks.asi`
- ASI SHA-256: `9A6AAF56E09D27AA5D7F894D0C2D43E14E421B32AD78A1209DA3F8B816C80E01`
- Candidate archive: `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.5.2-rc1.zip`
- Archive SHA-256: `8E343E9577F4F4E2C370C33EE04A8669EF801579654EA5CEDBE64C6EEED6BFEE`
- Game SHA-256 observed in runtime log: `E7B481A97C02D80581FAB0BECE940214A88EBE30211088A00129845A039F9293`

## Runtime evidence

The supplied runtime log for this exact ASI reports:

- gameplay, cinematic aspect, cinematic FOV and dialogue hooks: `PASS`;
- runtime telemetry: `PASS`;
- gameplay FOV preservation at `90` after initialization;
- dialogue phase telemetry and native zoom/recovery;
- cinematic ENTER/EXIT and clean gameplay recovery;
- no post-EXIT timer guard or repeated FOV clamp.

## Candidate archive contents

```text
LICENSE.md
README.md
STALKER2CameraTweaks.asi
STALKER2CameraTweaks.ini
THIRD_PARTY_NOTICES.md
```

## Limits

- This candidate is not yet a final published release.
- The cinematic FOV handoff remains a research topic; no new seamless-transition improvement is claimed.
- The archive was checked for contents and hashes. No new game run was performed during packaging.
- Existing v0.5.2 and historical assets were preserved; no files were deleted.
