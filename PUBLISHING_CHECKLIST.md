# Publishing Checklist

## GitHub

1. Create a new repository under your own account. Do not push this work to Lyall's upstream repository.
2. Upload the contents of `github-source` as the repository source.
3. Create tag `v0.1.0` with title `STALKER 2 Gameplay Aspect Fix 0.1.0`.
4. Paste `GITHUB_RELEASE_BODY.md` into the release description.
5. Upload `release/STALKER2GameplayAspectFix-2.0.2-Steam.zip` as the release asset.

## Nexus Mods

1. Use the title `STALKER 2 Gameplay Aspect Fix - Steam 2.0.2`.
2. Paste `NEXUS_DESCRIPTION.md` into the description field.
3. Upload `release/STALKER2GameplayAspectFix-2.0.2-Steam.zip` as the main file.
4. Mark the file as requiring Ultimate ASI Loader.
5. Add the original STALKER2Tweak, Ultimate ASI Loader, SafetyHook, Zydis, spdlog, Dumper-7, and Ghidra to credits where supported.

## Before Publishing

- Verify the zip contains only `STALKER2GameplayAspectFix.asi`, `README.md`, and `LICENSE.md`.
- Confirm the game still starts with only `dsound.dll` and `STALKER2GameplayAspectFix.asi` in `Win64`.
- Complete one cutscene and confirm the gameplay camera restores automatically.
- Do not include `STALKER2LetterboxFix.asi` or experimental archives.
