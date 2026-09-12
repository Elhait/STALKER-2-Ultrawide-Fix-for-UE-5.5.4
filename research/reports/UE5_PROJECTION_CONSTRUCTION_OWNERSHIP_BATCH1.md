# UE5 Projection Construction Ownership — Batch 1 Report

Status: Batch 1 complete — no confirmed current-build projection-construction
owner established; Batch 2 remains blocked.

## Identity gate

- Executable: `Stalker2-Win64-Shipping.exe`, Steam 2.0.4.
- Executable SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Image base: `0x140000000`.
- `.text`: RVA `0x1000`, virtual size `130818504`, raw size `130818560`.
- Section layout: 11 PE sections, section alignment `0x1000`, image size
  `0xB951000`; `.rdata` RVA `0x7CC4000`, `.data` `0x9E89000`, `.pdata`
  `0xB1A7000`, `.rsrc` `0xB652000`, `.reloc` `0xB667000`; the remaining
  section entries match the current executable image.
- Known current-build anchors: gameplay writer, cinematic aspect setter
  `RVA 0x6B7CB05`, cinematic live-FOV boundaries `RVA 0x2EE6936` /
  `0x2EE69A7`, and ADS anchors `RVA 0x6ABE7E` / `0x6AC01B`.
- Identity: `PASS`.

## Bounded inventory

The search was limited to existing current-build evidence and previously
bounded projection-relevant reviews. The semantic targets were FOV-derived
scale, aspect/viewport dimensions, axis-specific projection scale and
temporary view/projection state.

### Confirmed owners / boundaries

- `RVA 0x6B7CB05` is a confirmed cinematic aspect/state store, but its
  current-build local data flow does not establish view/projection construction.
- `RVA 0x2EE6936` and `RVA 0x2EE69A7` are confirmed cinematic live-FOV
  lifecycle boundaries. They are not proven to be downstream projection
  construction owners and their transformed values enter existing camera-state
  handling.

### Historical or bounded leads

- Historical 2.0.3 `FUN_140186BE8` computes matrix-like values, consumes
  viewport-like parameters and stores matrix-related fields. Its callers and
  surrounding data flow were tied to scene-resource/lighting processing, with
  no relation established to the validated cinematic object or aspect/FOV
  state. It is therefore not a current-build ownership input.
- Generic projection-like writers and camera-field intersections remain only
  bounded leads. The current matching-image review produced no causal
  cinematic projection candidate.

### Rejected

- Durable cinematic state `+0x54` and its update chain are rejected as the
  projection-construction owner because they represent persistent state
  handling rather than temporary axis-specific projection data.
- The runtime-derived virtual target at current executable `RVA 0x20939B8`
  is rejected for this purpose because the established review found no read of
  the relevant `+0x54` state.
- Generic matrix/render helpers without cinematic reachability are rejected as
  ownership evidence; matching field names or matrix arithmetic alone is not
  sufficient.

### Deferred / requires lower-level implementation

- A current-build UE5 view/projection construction boundary that combines FOV
  with aspect/viewport dimensions was not identified in this bounded pass.
- A future renderer/projection boundary remains a valid research direction. If
  found, it may require a separate renderer-hook feasibility task; that future
  implementation constraint does not make the ownership category invalid.

## Batch 1 conclusion

`Identity: PASS`.

`Batch 1: PASS — bounded inventory complete`.

No current-build candidate satisfies the required combination of projection
construction semantics, temporary state, cinematic reachability and safe
ownership evidence. Batch 2 is `BLOCKED` because there is no strongest
statically valid candidate to correlate at runtime. Batch 3 is also `BLOCKED`
pending Batch 2.

No production source, ASI, INI, build output or release asset was changed.
No runtime claim was made and no renderer or matrix hook was implemented.

