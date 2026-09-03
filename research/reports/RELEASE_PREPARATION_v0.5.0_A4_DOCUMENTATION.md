# Release Preparation v0.5.0 — A4 Documentation

Status: PASS

Revalidation: PASS after review of the reported completeness concern.

## Updated documents

- `README.md`
- `RELEASE_NOTES.md`
- `TESTING_AND_RESEARCH.md` (already contained the complete A1 evidence history;
  independently rechecked against A1 and A3)
- `GITHUB_RELEASE_BODY.md`
- `NEXUS_DESCRIPTION.md`

## Consistency checks

- All documents identify the release as v0.5.0 and the unified
  `STALKER2UltrawideFix.asi`.
- Dialogue modes are consistently documented as `Native`, `Adaptive`,
  `Reduced` and `Disabled`.
- `Adaptive` is described as preserving native optical zoom strength relative
  to the actual gameplay FOV; `Reduced` is described as half of that optical
  effect, not the superseded linear degree midpoint.
- Dialogue lifecycle timing and smooth EXIT recovery are documented without
  claiming a new renderer or camera-write implementation.
- F9/F10 are described as optional, disabled by default, configurable within
  the supported key range, and effective for the next applicable lifecycle.
- Manual INI edits are documented as requiring a restart; runtime hotkey
  selection is documented as not requiring a restart.
- Resolution changes during a running session are described consistently as
  validated for `AspectRatio=Auto`; the stale contradictory limitation was
  removed from the release notes.
- Runtime validation is limited to Steam 2.0.4. Static dialogue resolver
  portability across Steam 2.0.2, 2.0.3 and 2.0.4 is explicitly separated from
  runtime compatibility.
- Weapon/viewmodel FOV remains documented as a separate known issue and is not
  presented as fixed by v0.5.0.
- WIDEBOY attribution describes a research reference and independently
  reverse-engineered implementation. Lyall and vendored dependency notices
  remain in their appropriate attribution document.
- No `OpticalReduced` policy or feasibility-only diagnostic claim appears in
  the release-facing documents.

## A4 gate

Release-facing documentation now reflects the A1 evidence/claim matrix and
A3 provenance classification. The previously identified Nexus changelog,
resolution-scope and restart-wording inconsistencies were corrected. README,
release notes, GitHub release body, Nexus description and testing history agree
on the v0.5.0 feature scope and evidence boundaries.

Documentation: PASS.

## Revalidation note

The reported missing dialogue content was rechecked against the files currently
on disk. `TESTING_AND_RESEARCH.md` contains the dialogue research progression,
a dedicated `### Dialogue` evidence section, dialogue compatibility boundaries
and dialogue feasibility/promotion history. `NEXUS_DESCRIPTION.md` contains the
dialogue subsystem in `Features`, as well as its detailed configuration and
runtime behavior. No additional documentation edit was required by this
revalidation, and A5 remains blocked until the normal next batch is started.

Filesystem evidence from the independent revalidation:

- `E:\Work\Slaker2 mods\01-Projects\STALKER-2-Ultrawide-Fix-for-UE-5.5.4\TESTING_AND_RESEARCH.md`
  SHA-256 `D6438774BAA8A87E30EE94F37D7F298C83B190BF7DB6FB17292CDB5E6BFD7EC4`;
  dialogue progression at lines 61–69, `### Dialogue` at line 117,
  compatibility boundary at line 158, and dialogue promotion history at lines
  187–189.
- `E:\Work\Slaker2 mods\01-Projects\STALKER-2-Ultrawide-Fix-for-UE-5.5.4\NEXUS_DESCRIPTION.md`
  SHA-256 `55C6D3504928072DF07F0D301A0881EBC8C400F72019DAADC8FEE827FA27F1C7`;
  v0.5.0 at line 3, dialogue feature at lines 62–63, and `### 0.5.0` at
  line 190.
- No second copy of either filename exists under the workspace root.
- The stale sentence claiming that runtime resolution changes are not validated
  is absent from the current Nexus file; its current text states that `Auto`
  updates after resolution changes in the same session.

Next permitted batch: A5 — Release Assets.
