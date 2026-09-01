# Reverse-Engineering Research Archive Reorganization Task Plan

## Objective

Reorganize historical reverse-engineering task plans after the v0.4.0 release so that `backlog` contains only active work, while completed, rejected and deferred research remain discoverable in a structured `research` archive.

## Established evidence and current state

- The v0.4.0 release is already published.
- `backlog` currently contains 36 files in its root and 41 files in `backlog/complete`.
- The root therefore mixes active-looking plans, completed plans, historical research and administrative plans.
- Existing research documents are valuable evidence and must be preserved; this is an organization change, not deletion.
- `TESTING_AND_RESEARCH.md` is the current-state summary and should remain the concise entry point.

## Approved scope

- Inventory and classify every existing `backlog` task plan.
- Create `research/completed`, `research/deferred`, `research/rejected`, `research/reports` and `research/evidence` only where justified.
- Keep `backlog/README.md` and `backlog/TASKLOG.md` at the backlog root.
- Move plans using recoverable Git-tracked path changes; do not delete research content.
- Update repository-relative Markdown links affected by moved files.
- Add `research/README.md`, `backlog/README.md` and a concise research progression section to `TESTING_AND_RESEARCH.md`.
- Update the workspace `AGENTS.md` scope wording to distinguish the stable unified release from deferred research.

## Explicit non-goals

- No C++ source, ASI, INI, release archive or build artifact changes.
- No new reverse-engineering, runtime validation or Ghidra analysis.
- No rewriting of historical task-plan conclusions beyond path/link corrections.
- No deletion of files, old evidence or historical release material.
- No forced Git operations, commit, push or branch changes.

## Expected files and areas

- `backlog/` and `backlog/complete/`
- new `research/` archive directories and indexes
- `TESTING_AND_RESEARCH.md`
- workspace `AGENTS.md`
- repository Markdown files containing links to moved backlog plans
- this plan file, removed or retained according to the final review decision

## Batches

### Batch 1 — inventory and classification

- Enumerate all backlog files and extract their title/status signals.
- Search all tracked Markdown files for references to backlog paths.
- Produce an explicit move map before changing paths.

Validation: every existing backlog plan has exactly one proposed destination or an explicit keep-in-place reason; all affected links are identified.

### Batch 1 classification map

Keep as active work in `backlog/active/`:

- `CUTSCENE_FOV_DOWNSTREAM_STATE_TASK_PLAN.md`
- `CUTSCENE_HORPLUS_IMPLEMENTATION_TASK_PLAN.md`
- `CUTSCENE_HORPLUS_TRANSITION_BOUNDARY_TASK_PLAN.md`
- `LETTERBOX_REAUDIT_TASK_PLAN.md`
- `WEAPON_VIEWMODEL_FOV_REFERENCE_TASK_PLAN.md`
- `CINEMATIC_RUNTIME_STATE_DIFFERENTIAL_TRACE_TASK_PLAN.md`

Move to `research/deferred/`:

- `POST_EXIT_HANDOFF_STATE_CLASSIFICATION_AT_A_TASK_PLAN.md`
- `CINEMATIC_FOV_LEGACY_CROSS_VERSION_STATIC_RE_204_TASK_PLAN.md`
- `CINEMATIC_FOV_CONVERGENCE_TARGET_RE_204_TASK_PLAN.md`

Move bounded completed work to `research/completed/`:

- `CINEMATIC_AUTO_F3_EXIT_FINGERPRINT_204_TASK_PLAN.md`
- `CINEMATIC_F7_TIMING_RUN_204_TASK_PLAN.md`
- `CINEMATIC_FOV_ASPECT_CAUSAL_TRANSITION_204_TASK_PLAN.md`
- `CINEMATIC_GLOBAL_ULTRAWIDE_INTEGRATION_204_TASK_PLAN.md`
- `CINEMATIC_MANUAL_FOV_ASPECT_CONTROL_PROBE_204_TASK_PLAN.md`
- `CINEMATIC_FOV_POLICY_CLEANUP_031_TASK_PLAN.md`
- `CINEMATIC_RUNTIME_FOV_DISCOVERY_204_TASK_PLAN.md`
- `CINEMATIC_STATE_TRANSITION_PROBE_204_TASK_PLAN.md`
- `COMPLETE_BACKLOG_AND_TESTING_SUMMARY_TASK_PLAN.md`
- `GAMEPLAY_FOV_DIFFERENTIAL_DISCOVERY_204_TASK_PLAN.md`
- `IMPLEMENTATION_HISTORY_TASK_LOG_TASK_PLAN.md`
- `LEGACY_FOV_CALLGRAPH_DATAFLOW_RECONSTRUCTION_204_TASK_PLAN.md`
- `LOG_NOISE_REDUCTION_TASK_PLAN.md`
- `NEXUS_DESCRIPTION_0_3_0_TASK_PLAN.md`
- `RELEASE_0_3_0_TASK_PLAN.md`
- `SETTINGS_FOV_MARKER_DISCOVERY_204_TASK_PLAN.md`
- all existing files currently in `backlog/complete/`

Move negative or blocked branches to `research/rejected/`:

- `CINEMATIC_AUTOMATIC_POST_SETTER_FEASIBILITY_204_TASK_PLAN.md`
- `CINEMATIC_FOV_OWNER_DISCOVERY_ARTIFACT_204_TASK_PLAN.md`
- `CINEMATIC_FOV_RUNTIME_OWNERSHIP_CORRELATION_204_TASK_PLAN.md`
- `CINEMATIC_FOV_SEMANTIC_TRANSITION_RE_204_TASK_PLAN.md`
- `CINEMATIC_FOV230_WRITE_ORIGIN_204_TASK_PLAN.md`
- `CINEMATIC_POST_ENTER_ANCHOR_BATCH_204_TASK_PLAN.md`
- `CINEMATIC_PRECALL_IDENTITY_RETURN_CORRELATION_204_TASK_PLAN.md`
- `CINEMATIC_SCALAR_FP_RUNTIME_DISCOVERY_204_TASK_PLAN.md`
- `CUTSCENE_CAMERA_ASPECT_REFRESH_TASK_PLAN.md`
- `CUTSCENE_DOWNSTREAM_PROJECTION_CONSUMER_TASK_PLAN.md`

Keep `TASKLOG.md` at the backlog root and keep this migration plan until final review. No other root plan file remains unclassified by this map.

### Batch 2 — archive structure and path migration

- Create the approved research directories.
- Move only files classified as completed, deferred or rejected.
- Keep active plans in `backlog/active/` and keep `TASKLOG.md` at the backlog root.

Validation: no source content is lost; moved files are present at their target paths; no unintended files are included.

### Batch 3 — documentation indexes and cross-references

- Add/update `backlog/README.md` and `research/README.md`.
- Update `TESTING_AND_RESEARCH.md` with the research progression and archive navigation.
- Update `AGENTS.md` scope wording if the final release/research distinction is confirmed.
- Correct all repository-relative links.

Validation: `git diff --check`, link/path search for stale backlog references, and Markdown diff review.

### Batch 4 — final review

- Inspect `git status`, diff summary, full relevant diff and recent commits.
- Compare actual changed paths with this plan.
- Record completed, remaining, deferred, blocked and not-runtime-validated items.
- Remove this plan only if that removal is explicitly included in the final review; otherwise retain it as the migration record.

## Risks and rollback / safe-failure behavior

- Path moves can break historical links. Mitigation: search references before and after migration and preserve redirect-style references where useful.
- A plan can be misclassified. Mitigation: keep ambiguous plans in place until evidence supports a move.
- Large path-only diffs can obscure content changes. Mitigation: use separate migration and documentation commits if the diff becomes difficult to review.
- Rollback is recoverable through Git path restoration; no destructive deletion is authorized.

## Stop conditions and phase gates

- Stop before Batch 2 if any file has ambiguous status or an affected link cannot be mapped safely.
- Stop if the proposed structure requires rewriting historical research rather than organizing it.
- Stop if unrelated source, release or build files appear in the diff.
- The task is complete only after final Git review confirms the actual paths and links match this plan.

## Expected final Git review

The final review must confirm that the repository has a clear active backlog, a navigable research archive, no unintended deletions, corrected internal links, and no changes to the released implementation or package.


## Final review result — 2026-09-01

- `backlog/` now contains only `README.md`, `TASKLOG.md` and `active/`.
- Six active plans are in `backlog/active/`.
- Three deferred plans are in `research/deferred/`.
- Ten negative or blocked branches are in `research/rejected/`.
- Fifty-seven completed plans are in `research/completed/`.
- `research/reports/` and `research/evidence/` were created as reserved archive areas and contain no files yet.
- No files were deleted; the old empty `backlog/complete/` directory was removed after its contents were moved.
- Repository-relative links to moved plans were checked; remaining old-path mentions are historical text inside archived plans or `TASKLOG.md`, not active Markdown links.
- No C++ source, ASI, INI, release archive or build artifact was changed.
- `git diff --check` passed with only the existing LF/CRLF normalization warning for `TESTING_AND_RESEARCH.md`.

Status: complete for archive organization and documentation navigation. Runtime validation was not applicable.
