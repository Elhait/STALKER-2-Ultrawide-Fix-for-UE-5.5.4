# Test Build Artifacts Organization Task Plan

Status: Closed — test scripts and diagnostic ASI outputs were moved, path handling was corrected and the moved transition tracer build succeeded.

## Objective

Keep the project root limited to the main working `build.cmd` while preserving test build scripts and diagnostic ASI outputs in dedicated artifact folders.

## Established evidence and current state

- The root contains one main `build.cmd` and six diagnostic `build-*.cmd` scripts.
- The root contains two diagnostic `.asi` files: `STALKER2AdsViewmodelRefreshTrace.asi` and `STALKER2WeaponViewmodelTrace.asi`.
- The project `.gitignore` already ignores all `*.asi`; a folder-specific duplicate rule is unnecessary.
- Test build scripts currently resolve `external` and `src` relative to the project root, so their paths must be adjusted when moved.
- `release-assets/` is a separate release boundary and must remain untouched.

## Approved scope

- Keep only `build.cmd` in the project root.
- Move the six diagnostic `build-*.cmd` scripts to `build-artifacts/test-scripts/`.
- Move the two root diagnostic `.asi` files to `build-artifacts/test-asi/`.
- Update moved test scripts to resolve the project root safely from their new location.
- Preserve the existing global `*.asi` ignore rule and leave `release-assets/` unchanged.

## Explicit non-goals

- Do not move or modify `build.cmd`.
- Do not move, ignore, delete or alter release assets.
- Do not change C++ source, build behavior beyond path correction, or runtime behavior.
- Do not add a redundant `test-asi/*.asi` ignore rule.

## Expected files and areas

- `build-artifacts/test-scripts/`.
- `build-artifacts/test-asi/`.
- Six moved test scripts with corrected project-root path handling.
- Project `.gitignore` only if inspection proves the existing `*.asi` rule is absent; current evidence says no change is needed.

## Batches and validation

### Batch 1 — Inventory and plan

- Confirm exact scripts, ASI files, ignore rules and release boundary.
- Create this plan before moving files.

### Batch 2 — Move and path-preserve test artifacts

- Create the two destination folders.
- Move the six scripts and two ASI files using exact paths.
- Change only the moved scripts' working-directory calculation from their old root location to the project root.

### Batch 3 — Review

- Confirm only `build.cmd` remains in the root.
- Confirm all moved files exist in their intended destinations.
- Confirm each moved script still references project-root `external` and `src` paths.
- Confirm the existing global `*.asi` ignore rule remains sufficient and `release-assets/` is untouched.
- Run `git diff --check`; no runtime validation is required.

## Risks and rollback / safe failure

- Exact file moves are reversible to the original root paths.
- Incorrect relative paths could break test builds; validate every moved script before closure.
- If a destination file already exists, stop rather than overwrite it.

## Stop conditions and phase gates

- Stop if any destination path is occupied by a different file.
- Stop if a moved script requires broader source or project restructuring.
- Stop before changing release assets or stable build behavior.

## Expected final Git review

- Perform read-only Git status/diff review after the moves and path corrections.
- Record the factual cleanup result in `backlog/TASKLOG.md`.
