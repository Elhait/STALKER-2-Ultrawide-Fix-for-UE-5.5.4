# Vendored Dependencies Cleanup Task Plan

Status: Closed — unused spdlog upstream material was removed, required headers and licenses were retained, notices were added and the representative build succeeded.

## Objective

Keep the vendored dependencies required for reproducible builds while removing unused upstream spdlog project material and documenting third-party licensing.

## Established evidence and current state

- The source includes SafetyHook, bundled Zydis and `spdlog/sinks/basic_file_sink.h`.
- Build scripts compile the local SafetyHook/Zydis sources and include `external/spdlog/include`.
- `external/spdlog` currently contains headers plus upstream examples, tests, benchmarks, CMake/CI files, scripts, logos and metadata not required by this project build.
- `spdlog/LICENSE` is present. `THIRD_PARTY_NOTICES.md` currently does not identify spdlog, SafetyHook or Zydis.

## Approved scope

- Retain `external/spdlog/include/` and `external/spdlog/LICENSE`.
- Remove only the unused spdlog upstream project material outside those retained paths.
- Preserve `external/safetyhook/` and its bundled Zydis files unchanged.
- Add concise license/provenance entries for spdlog, SafetyHook and Zydis to `THIRD_PARTY_NOTICES.md`.
- Do not convert dependencies to submodules in this task.

## Explicit non-goals

- Do not remove any header required transitively by `basic_file_sink.h`.
- Do not modify source, build scripts, `.gitignore`, release assets or dependency code.
- Do not change dependency versions or fetch replacement copies.
- Do not make runtime or compatibility claims.

## Expected files and areas

- Retained: `external/spdlog/include/`, `external/spdlog/LICENSE`.
- Removed: spdlog files/directories outside the retained paths.
- Updated: `THIRD_PARTY_NOTICES.md`.
- This plan moves to `backlog/complete/` after validation.

## Batches and validation

### Batch 1 — Dependency inventory and retention boundary

- Confirm the exact retained spdlog paths and list removable paths.
- Create this plan before cleanup.

### Batch 2 — Trim unused upstream material and add notices

- Remove exact spdlog paths outside `include/` and `LICENSE`.
- Add provenance/license notices without altering dependency source.

### Batch 3 — Review

- Confirm retained headers and license exist.
- Confirm SafetyHook/Zydis are unchanged.
- Build the main project or a representative test target to verify the retained spdlog headers remain usable.
- Run `git diff --check` and compare changed paths with this plan.

## Risks and rollback / safe failure

- Deleted upstream files can be restored from the current Git state; no broad wildcard deletion is permitted.
- If compilation reports a missing transitive header, stop and restore only the required retained file from the existing dependency copy.
- If license provenance cannot be verified from retained files, stop before claiming a complete notice.

## Stop conditions and phase gates

- Stop if a source or build script depends on a file outside the retained spdlog boundary.
- Stop if SafetyHook/Zydis would need modification to preserve the current build.
- Stop before any submodule migration or dependency version update.

## Expected final Git review

- Perform read-only Git status/diff review after cleanup and validation.
- Record the factual result in `backlog/TASKLOG.md`.
