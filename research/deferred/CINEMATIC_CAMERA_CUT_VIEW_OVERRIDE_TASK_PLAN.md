# Cinematic Camera-Cut / View Override Path — Task Plan

Status: `BATCH 1 COMPLETE / BOUNDED LEAD / RUNTIME BLOCKED`.

## Objective

Determine whether the current Steam 2.0.4 cinematic camera-cut/view pipeline
supplies or overrides an effective aspect-ratio axis policy downstream of the
general camera writer, without relying on the inactive
`source +0x258 -> gate bit 0x2 -> output +0x60/+0x64` propagation path.

Research question:

```text
Does the cinematic camera-cut/view pipeline supply or override
AspectRatioAxisConstraint downstream of the general camera writer?
```

## Established evidence and current state

- Canonical Steam 2.0.4 identity is established:
  SHA-256 `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- The reflected CameraComponent setter and its current-build field candidate
  are known, but the confirmed general writer path did not activate the
  propagation gate during cinematic runtime observation.
- Runtime Batch 2 observed stable `source +0x258`, inactive
  `source +0x259 & 0x2`, and zero `output +0x60/+0x64` across the capture.
- Therefore the general camera writer is not accepted as the cinematic
  effective axis-policy owner.
- The `source-object vslot +0x638` handoff remains a secondary unresolved
  provenance lead; its target is polymorphic and was not resolved.

## Approved scope

- Identify current-build camera-cut/view-specific data flow and override
  boundaries.
- Start from known cinematic ENTER/EXIT anchors and already established
  camera-cut lifecycle evidence.
- Search only for a concrete current-build correspondence involving view data,
  FOV/aspect state, axis-policy-like branching, or camera-cut override input.
- Preserve object identity and caller/argument provenance whenever following a
  candidate.
- Use read-only runtime observation only after a concrete safe boundary is
  established.

## Explicit non-goals

- No writes to camera, view, projection, UObject or reflected fields.
- No modification of stable production ASI, INI, release assets or Full Hor+
  behavior.
- No broad renderer, projection-matrix, tangent or generic displacement scan.
- No guessed enum/field writes and no guessed UObject layout.
- Do not resume the exhausted `+0x258/+0x259/+0x60/+0x64` lifecycle branch.
- Do not resolve `[RAX+0x638]` through a broad vtable/class scan.

## Expected files or areas

- New bounded Ghidra helper(s) under `02-Research/Ghidra/ghidra-scripts`.
- One durable report under `research/reports`.
- A diagnostic source/build script only if a safe read-only boundary is found.
- This task plan.

## Batches

### Batch 1 — Camera-cut/view override ownership inventory

Status: `COMPLETE / BOUNDED LEAD`.

- Begin from confirmed cinematic ENTER/EXIT and camera-cut-related anchors.
- Identify candidate view-description or camera-cut override data flow.
- Require current-build identity gate and reproducible instruction/object
  provenance.
- Record whether the candidate is before/after authored FOV conversion and
  whether it can affect axis-specific projection policy.

Validation: static report with SHA-256, `.text` size, image base/layout,
known anchors, identity status, candidate addresses, decoded instructions and
provenance.

Result: `FUN_140280FC8` is a confirmed current-build field-interpretation
helper. It reads the established aspect/selector family and produces a
multi-float output object, but the bounded audit did not establish direct
camera-cut provenance or a same-object bridge from `FUN_142EE68DA`. See
`research/reports/CINEMATIC_CAMERA_CUT_VIEW_OVERRIDE_BATCH1_RESULT.md`.

### Batch 2 — Narrow runtime observation

Status: `BLOCKED — no camera-cut-safe boundary`.

- Observe only a confirmed camera-cut/view override boundary.
- Log lifecycle phase, object identity, FOV/aspect inputs and policy-like
  values without writes.

Validation: runtime log tied to the canonical 2.0.4 identity gate.

### Batch 3 — Decision

Status: `DECISION / DEFERRED pending new provenance anchor`.

- Promote only a reproducible camera-cut override or downstream effective-view
  boundary.
- The current candidate remains a bounded lead, not an owner, because its
  camera-cut provenance and object identity are unresolved.
- Close this branch as bounded negative if no independent camera-cut/object
  identity anchor appears; do not expand into renderer/matrix hunting.

## Risks and rollback / safe failure

- Camera-cut callbacks may be repeated or nested; marker order is not enough
  without object identity and field evidence.
- A generic view/projection helper may be unrelated to cinematic camera cuts.
- Any ambiguous candidate is observation-only and must not be written.
- If identity, provenance or safe read boundary fails, stop with no runtime
  artifact.

## Stop conditions and phase gates

- Stop immediately on identity mismatch or unresolved known-anchor mismatch.
- Stop if the candidate has no camera-cut provenance or dissolves into generic
  renderer/matrix code.
- Stop if no current-build native correspondence or same-object bridge exists.
- Stop before runtime if no safe read-only boundary exists.
- Stop before any write or production integration.

## Expected final review

- Review only the approved plan, research helpers, reports and any explicitly
  approved diagnostic artifact.
- Confirm stable source, Full Hor+ baseline, INI and release assets are
  untouched.
- Do not stage, commit, reset, checkout or otherwise modify Git state.
