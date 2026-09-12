# Cinematic Projection Remap Feasibility — R1 Ownership Inventory

Status: Batch 1 complete — no confirmed projection-only owner established; R2 remains blocked.

## Identity gate

- Executable: `Stalker2-Win64-Shipping.exe`, Steam 2.0.4.
- Executable SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Image base: `0x140000000`.
- `.text`: RVA `0x1000`, virtual size `130818504`, raw size `130818560`.
- PE section layout: 11 sections, image base `0x140000000`, section
  alignment `0x1000`, image size `0xB951000`:
  `.text` RVA `0x1000`, virtual size `130818504`, raw size `130818560`;
  `.rdata` RVA `0x7CC4000`, raw size `35407360`;
  `.data` RVA `0x9E89000`, raw size `482816`;
  `.pdata` RVA `0xB1A7000`, raw size `4854272`;
  `.msvcjmc` RVA `0xB649000`; `.retplne` RVA `0xB64A000`;
  `.tls` RVA `0xB64B000`; `.voltbl` RVA `0xB64D000`;
  `_RDATA` RVA `0xB64F000`; `.rsrc` RVA `0xB652000`;
  `.reloc` RVA `0xB667000`.
- Known current-build anchors reused from the matching-image validation: gameplay writer, cinematic aspect setter `RVA 0x6B7CB05`, cinematic live-FOV boundaries `RVA 0x2EE6936` / `0x2EE69A7`, and ADS anchors `RVA 0x6ABE7E` / `0x6AC01B`.
- Identity: `PASS`.

The identity gate is based on the current executable plus the completed
matching-image revalidation. This report does not promote historical 2.0.3
addresses to current-build evidence.

## Established flow and candidate classification

### Confirmed owners / boundaries

- The cinematic aspect store at `RVA 0x6B7CB05` is a confirmed cinematic
  aspect/state boundary. Its current-build local data flow does not establish a
  projection matrix or X/Y projection-scale owner.
- The live cinematic FOV consumer boundaries at `RVA 0x2EE6936` (ENTER) and
  `RVA 0x2EE69A7` (EXIT) are confirmed FOV lifecycle boundaries. Existing
  runtime evidence shows that transforming the ENTER scalar changes the
  cinematic FOV, but the value enters the existing camera-state machinery and
  is not proven to be projection-only.

### Bounded leads

- The shared cinematic state/update chain through `FUN_146B68976`,
  `FUN_1431D2094` and `FUN_1431D23AE` remains a bounded state-flow lead. It
  carries the cinematic value into durable state handling, but no downstream
  temporary view/projection consumer was established.
- A renderer/projection boundary remains a valid architectural lead for a
  future lower-level task. It is not rejected as an ownership category; it is
  simply not identified by the approved bounded chain in this batch.

### Rejected for projection-only ownership

- Durable `state + 0x54` is rejected as a projection-only intervention point:
  `FUN_1431D2094` uses it for equality/update handling and forwards to the
  durable state writer/update path.
- The runtime-derived virtual target at executable `RVA 0x20939B8` is rejected
  as the current projection owner because the established review found that it
  does not read `+0x54`.
- The previously inspected post-ENTER return-site candidate is rejected for
  implementation because its presumed object register is volatile/clobbered;
  no safe object or continuation contract was established.

### Deferred / requires lower-level implementation

- A concrete temporary view/projection matrix or non-linear X remap consumer
  was not found in the bounded current-build evidence. Reaching such a
  boundary may require a dedicated renderer-hook feasibility task. That is a
  downstream implementation constraint, not a reason to discard renderer
  ownership if future evidence identifies it.

## R1 conclusion

The current evidence confirms cinematic FOV/aspect state boundaries, not a
projection-only boundary downstream of them. No candidate currently satisfies
all of the required ownership properties: cinematic-path correlation, FOV plus
aspect/projection consumption, temporary projection data, and safe transient
intervention.

R1: `PASS — bounded inventory complete`.

R2: `BLOCKED` pending a new concrete downstream projection anchor. No broad
renderer, matrix or global FOV scan is authorized by the current plan. The
production full-Hor+ path and stable source remain unchanged.

## Evidence limits

- Static ownership conclusions are limited to the identified Steam 2.0.4 image.
- Existing 2.0.3 downstream findings remain historical context and are not
  promoted as current 2.0.4 proof.
- No runtime correlation or implementation was performed in this batch.
- No conclusion is made about whether a future Panini-like remap is feasible.
