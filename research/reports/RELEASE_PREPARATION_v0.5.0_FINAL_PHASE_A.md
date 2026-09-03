# Final Phase-A Release Preparation Report — v0.5.0

Status: READY FOR USER COMMIT APPROVAL

## Gate summary

| Gate | Result |
| --- | --- |
| A0 — Scope Lock | PASS |
| A1 — Evidence / Delta / Claim Matrix | PASS |
| A2 — Production Binary | PASS |
| A3 — Provenance / Attribution | PASS |
| A4 — Documentation | PASS |
| A5 — Release Assets | PASS |
| A6 — Archive | PASS |
| A7 — Final Consistency Audit | PASS |
| A8 — Git Pre-commit Review | PASS |

Phase A result: COMPLETE.

## Approved production identity

- Production binary: `build-artifacts/test-asi/STALKER2UltrawideFix.asi`
- Production binary SHA-256: `B8A05E6DA92DDE89FBB213367B02F40BE3A5AACD46DC0C09E5D29DE9B13A881E`
- Runtime evidence: Steam 2.0.4 only.
- Static dialogue resolver portability evidence: Steam 2.0.2, 2.0.3 and 2.0.4.

## Approved archive identity

- Archive: `release-assets/STALKER2UltrawideFix-UE5.5.4-v0.5.0-A6.zip`
- Archive SHA-256: `A41E973187AECF5AE4F02885C6E5ED3E32318E72ED08A2E4B2DA1616DB6E76E2`
- Clean extraction verified with exactly the five approved production files.
- Extracted ASI SHA-256 matches the approved production binary.

## Approved commit scope

Include the following categories in the explicitly approved commit:

- Final production source for the unified fix.
- v0.5.0 canonical documentation and release notes.
- Permanent release playbook and `backlog/TASKLOG.md` updates.
- Relevant completed, deferred and active task-plan moves.
- Relevant completed research/provenance material.
- A0–A8 release-preparation reports.
- `research/reports/RELEASE_PREPARATION_v0.5.0.md`, because it is visibly marked `SUPERSEDED` and retained as historical provenance.

The current index is known to contain stale staging relative to the final working-tree documentation. B0 must explicitly rebuild staging from the approved scope and verify the final staged path list before committing.

## Excluded from the commit and release payload

- `release-assets/` payload and generated release ZIP artifacts, unless the repository convention explicitly requires them later.
- Diagnostic, tracer, observer and feasibility-only source files.
- Diagnostic/test build scripts and generated object files.
- Generated test INI files.
- Runtime logs, temporary files and unrelated research/build artifacts.

## Public-claim boundaries

- Dialogue modes are `Native`, `Adaptive`, `Reduced` and `Disabled`; production `Reduced` uses the validated optical half-strength model.
- F9/F10 hotkeys select the next applicable cinematic or dialogue lifecycle and do not rebuild an already active lifecycle.
- Hotkeys are opt-in and disabled by default.
- Manual INI edits require a game restart; runtime hotkey selection does not.
- Weapon/viewmodel FOV investigation remains deferred and is not claimed as fixed.
- Runtime compatibility claims remain limited to the validated Steam 2.0.4 executable; static resolver validation across 2.0.2–2.0.4 is not runtime compatibility proof.

## Proposed commit

Title:

`release: add dialogue zoom controls for v0.5.0`

No staging, commit, tag, push or publication was performed as part of Phase A. Those actions require the user's explicit B0 approval.
