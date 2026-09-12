# Cinematic Aspect-Ratio Axis Constraint Ownership — Batch 1 Report

Status: Batch 1 complete — no current-build axis-constraint owner established;
Batch 2 blocked.

## Identity gate

- Executable: `Stalker2-Win64-Shipping.exe`, Steam 2.0.4.
- Executable SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Image base: `0x140000000`.
- `.text`: RVA `0x1000`, virtual size `130818504`, raw size `130818560`.
- PE section layout: 11 sections, section alignment `0x1000`, image size
  `0xB951000`; current section table matches the established 2.0.4 image
  identity.
- Known current-build anchors: gameplay writer, cinematic aspect setter
  `RVA 0x6B7CB05`, cinematic live-FOV boundaries `RVA 0x2EE6936` /
  `0x2EE69A7`, and ADS anchors `RVA 0x6ABE7E` / `0x6AC01B`.
- Identity: `PASS`.

## Bounded inventory result

The approved inventory was limited to existing current-build source/research
evidence and the semantic ownership order:

```text
FMinimalViewInfo
→ CameraCutPlaybackCapability override
→ cinematic evaluation / camera-cut application
→ UCameraComponent override
→ ULocalPlayer fallback
```

No current-build data-flow mapping was found for an enum-like axis constraint,
override flag, camera-cut-specific synthesized value or a read/application
site relative to authored/effective FOV and projection construction.

The known cinematic aspect/FOV boundaries remain valid state/lifecycle
anchors, but existing evidence does not show that they read or apply an axis
constraint. The absence of UE API names in the stripped binary is not treated
as evidence that the feature is absent.

## Candidate classification

- `FMinimalViewInfo`: `UNRESOLVED` — engine-level view-description concept is
  documented, but no current-build field or data flow is established.
- `CameraCutPlaybackCapability`: `UNRESOLVED` — documented UE5 cinematic
  override concept, but no current-build camera-cut equivalent is mapped.
- `UCameraComponent` override: `UNRESOLVED` — no current-build component
  ownership or override flag evidence.
- `ULocalPlayer` fallback: `UNRESOLVED` — no current-build read/application
  evidence and no proof that it is effective during cinematics.
- Existing cinematic live-FOV/aspect hooks: `REJECTED AS AXIS-CONSTRAINT
  OWNER` — they establish FOV/aspect lifecycle state, not an axis-policy
  application boundary.

No candidate can currently be classified as `CONFIRMED OWNER` or
`CONFIRMED DOWNSTREAM BOUNDARY`.

## Batch 1 conclusion

`Identity: PASS`.

`Batch 1: PASS — bounded ownership inventory complete`.

`Batch 2: BLOCKED` because no safe current-build owner or concrete reversible
test boundary was established. This result does not reject the UE axis-
constraint hypothesis; it marks it as unresolved pending a new concrete
runtime/engine anchor. No guessed enum write, runtime tracer, source change or
production modification was performed.

The current Full Hor+ production path remains unchanged.

