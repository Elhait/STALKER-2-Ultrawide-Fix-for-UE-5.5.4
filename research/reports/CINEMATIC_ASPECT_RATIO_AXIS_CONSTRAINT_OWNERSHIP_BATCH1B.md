# Cinematic Aspect-Ratio Axis Constraint Ownership — Batch 1B

Status: `BLOCKED` — no current-build structural equivalent of the effective
axis-policy handoff was established.

## Identity and evidence boundary

- Executable: `Stalker2-Win64-Shipping.exe`, Steam 2.0.4.
- Executable SHA-256:
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Image base: `0x140000000`.
- `.text`: RVA `0x1000`, virtual size `130818504`, raw size `130818560`.
- Known current-build anchors: cinematic aspect setter `RVA 0x6B7CB05`,
  cinematic live-FOV boundaries `RVA 0x2EE6936` / `0x2EE69A7`, and ADS
  anchors `RVA 0x6ABE7E` / `0x6AC01B`.
- Identity: `PASS`.

The UE 5.5 API documentation establishes a valid engine-side correspondence
target: `ULocalPlayer::GetProjectionData`, `FMinimalViewInfo`'s optional
constraint, `ULocalPlayer::AspectRatioAxisConstraint` and the
camera-cut-specific override. This is a source-level contract, not proof that
the stripped STALKER 2 binary retains the same call path or field layout.

## Approved bounded search

The batch searched only existing current-build evidence and the structural
projection target. It required a current-build context combining projection
data construction with effective FOV, viewport/aspect state and an
axis-dependent X-vs-Y semantic branch. Historical 2.0.3 projection logs were
used only for provenance and were not promoted as 2.0.4 evidence.

## Result

No current-build candidate met the promotion threshold. The available
evidence did not establish a function or boundary that combines viewport or
sub-rect dimensions, aspect derivation, effective cinematic FOV input,
perspective projection construction and an axis-policy discriminator
equivalent to MaintainYFOV versus MaintainXFOV.

The current 2.0.4 revalidation reports identify the known cinematic aspect/FOV
boundaries but no shared downstream projection owner. The current camera-field
intersection remains generic offset evidence and does not provide causal
projection ownership. The historical 2.0.3 candidate `FUN_140186BE8` and its
callers were not promoted: their available context is not current-build
projection proof and does not establish axis-policy semantics.

## Candidate classification

- Effective axis-constraint owner: `UNRESOLVED`.
- Current-build projection boundary with axis semantics: `NOT FOUND`.
- Historical projection-like candidate: `REJECTED / HISTORICAL ONLY`.
- Generic projection/math helpers: `REJECTED` as insufficient without an
  axis-policy discriminator and caller/object context.
- Renderer/projection implementation path: `DEFERRED / REQUIRES A NEW
  EVIDENCE CLASS`.

## Phase result

`Batch 1B: COMPLETE — no promotable current-build boundary found`.

This is a bounded negative result, not a rejection of the UE axis-constraint
hypothesis. The engine contract is confirmed, but the current STALKER 2
2.0.4 equivalent and a safe reversible observation boundary remain
unresolved. Batch 2 (`MaintainYFOV` runtime policy test) therefore remains
blocked. No guessed enum write, runtime tracer, source change, build or
production modification was performed.

Production Full Hor+ remains unchanged and is still the control baseline.

## Next-step boundary

The current static ownership/correspondence direction is exhausted for the
available evidence. Further progress requires a genuinely new evidence class,
not a broader or renamed static scan. Possible examples include independent
engine-side symbol or metadata correspondence, known UE function byte
correspondence, a concrete renderer/projection anchor, runtime observation,
frame-capture evidence or other independently verifiable evidence. Runtime
writes remain unauthorized until a safe boundary is established.
