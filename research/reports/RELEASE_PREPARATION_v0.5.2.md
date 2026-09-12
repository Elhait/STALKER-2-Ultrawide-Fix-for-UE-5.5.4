# Release Preparation — v0.5.2

Date: 2026-09-11

## Scope

Prepared the unified `STALKER2CameraTweaks` release package for v0.5.2. No
gameplay, cinematic or dialogue behavior was changed for this release-prep
batch; the source metadata header was updated so newly generated INI files
identify v0.5.2.

## Evidence and claims

| Area | Evidence | Status | Public claim |
|---|---|---|---|
| Production source | `src/experimental_cinematic_21_9_combined_fix_204.cpp` and `build.cmd` | PASS | Unified gameplay/cinematic/dialogue ASI |
| Current runtime | Supplied `STALKER2CameraTweaks.log`, mod SHA `69021D8758069F7EFE098B3C562A41E326A6DB0BF4BA88B789FB38854217DFB2`, game SHA `E7B481A97C02D80581FAB0BECE940214A88EBE30211088A00129845A039F9293` | PASS | Runtime evidence on the supplied current update (Steam 2.0.5) |
| New build | Local build output SHA `F55B17768625549D96E033FD79340A79DCA27FD9697264C37F4E7A6D6BAEEFB4` | PASS | Release binary built from current source |
| Older patches | Existing Ghidra report `cross-patch-production-resolver-validation-2026-09-02.md` | STATIC ONLY | Resolver portability checked for Steam 2.0.2, 2.0.3 and 2.0.4; no older-build runtime claim |
| UE target | Project release contract | PASS | UE 5.5.4 target |

The supplied runtime log validates the preceding current-build binary. The
newly rebuilt binary has not been injected or run in-game during this release
preparation, so the new binary itself remains not-runtime-validated here.

## Release assets

Archive:
`release-assets/STALKER2CameraTweaks-UE5.5.4-v0.5.2.zip`

Allowlisted archive contents:

- `STALKER2CameraTweaks.asi`
- `STALKER2CameraTweaks.ini`
- `README.md`
- `LICENSE.md`
- `THIRD_PARTY_NOTICES.md`

Production ASI SHA-256:
`F55B17768625549D96E033FD79340A79DCA27FD9697264C37F4E7A6D6BAEEFB4`

Archive SHA-256:
`73DD26FCAA29732E951728FE5EF01417AEB539435AA377D89F3858AC9D89A433`

## Limitations and disposition

- No publication, commit or tag was performed.
- Old release archives and unrelated working-tree changes were not removed.
- Future patches may require updated signatures; the ASI fails closed when
  validation does not pass.
- The package is ready for user review and explicit publication approval,
  subject to the stated limit that the rebuilt v0.5.2 binary has not received
  a separate post-build in-game run in this task.
