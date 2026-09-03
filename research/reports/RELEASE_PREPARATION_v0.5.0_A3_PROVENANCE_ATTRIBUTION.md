# Release Preparation v0.5.0 — A3 Plans, Provenance and Attribution

Status: PASS

## Plan classification

The current repository contains the following task-plan layout:

- Permanent reusable process: `backlog/RELEASE_PREPARATION_TASK_PLAN.md`.
- Active backlog: 9 plans in `backlog/active/`.
- Completed archive: 67 plans in `research/completed/`.
- Deferred archive: 4 plans in `research/deferred/`.
- Rejected archive: 10 plans in `research/rejected/`.

Completed dialogue, hotkey, configuration, cross-patch and production-core
plans are in `research/completed/`. Weapon/viewmodel work that is not part of
v0.5.0 remains active or deferred and was not promoted by this release run.
The reusable release playbook remains in `backlog` and was not moved.

## Provenance and path review

- The moved plans are present in their assigned archive directories.
- Active and deferred plans remain outside the completed archive.
- The release reports for A0, A1, A2 and this A3 pass are in
  `research/reports/`.
- Current release reports use repository-relative archive paths consistently.
- Historical entries in `backlog/TASKLOG.md` retain their original paths as
  factual historical record; they are not live links requiring migration.
- Research plans that refer to the workspace-level `02-Research` analysis
  area continue to resolve to the existing workspace research area and were
  not rewritten as repository paths.
- No new completed evidence, runtime result or external reference claim was
  discovered beyond the A1 evidence/claim matrix. A1 remains valid.

## Attribution classification

| Reference | Role in this project | Code/assets copied | Required treatment |
| --- | --- | --- | --- |
| Lyall's STALKER2Tweak | Existing helper/ASI scaffolding provenance | Limited scaffolding portions | Retain MIT attribution in `THIRD_PARTY_NOTICES.md` and public credits |
| WIDEBOY Fixes by BigChenga | Research reference and semantic lead for dialogue/camera FOV behavior | No | Describe as a research reference; state that the production implementation was independently reverse engineered and runtime validated |
| Sota No Dialogue Zoom reference | Semantic/data-layer reference for `DialogFOVDefault` and dialogue baseline behavior | No | Treat as research evidence, not a distributed dependency or implementation source |
| SafetyHook, Zydis and spdlog | Vendored build dependencies | Yes, as existing vendored dependencies | Retain their license/provenance notices in `THIRD_PARTY_NOTICES.md` |

The current `README.md`, `NEXUS_DESCRIPTION.md` and
`THIRD_PARTY_NOTICES.md` contain the corresponding attribution distinctions.
No external WIDEBOY or Sota code/assets are included in the release scope, so
no additional external license file is required for those references.

## A3 gate

All task plans are classified, the permanent release playbook is preserved,
completed research remains historically available, active/deferred work is not
promoted accidentally, and attribution distinguishes scaffolding, research
references and vendored dependencies. No new evidence claim was discovered,
so A1 does not need reopening.

Plans, provenance and attribution: PASS.

Next permitted batch: A4 — Documentation.
