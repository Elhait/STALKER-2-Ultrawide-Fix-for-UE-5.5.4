# Cinematic Camera-Cut / View Override — Batch 1 Result

## Identity

- Canonical executable: Steam 2.0.4
- SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- Identity: **PASS**

## Bounded target

The audit inspected the existing current-build dormant anchor
`FUN_140280FC8` (`0x140280FC8`) because prior evidence showed it consumes the
established camera-state field family.

## Confirmed dormant-anchor semantics

The function:

- reads `RCX + 0x254` as a float input;
- reads `RCX + 0x258`, `RCX + 0x259`, `RCX + 0x25A` and `RCX + 0x25B` as
  indexed/selector inputs;
- uses the values with a current-build table at `0x149ECE910`;
- scales the table outputs by the `+0x254` input;
- constructs an output object containing multiple float values;
- has callers that update camera-state-like fields before re-entering this
  helper (`FUN_14027F6A2`, `FUN_14028086C`, and current-build `0x145...`
  callers).

This is a **STRONG DORMANT ANCHOR / CONFIRMED FIELD-INTERPRETATION HELPER**. It
is no longer merely a displacement collision.

## Missing camera-cut bridge

The confirmed ENTER/EXIT function containing the known anchors is
`FUN_142EE68DA`. The bounded audit did not establish a direct call from that
camera-cut lifecycle function into `FUN_140280FC8`, nor a same-object bridge
between the camera-cut object and the dormant anchor's `RCX` object.

Therefore the dormant anchor cannot be promoted to:

- confirmed camera-cut override owner;
- confirmed projection consumer;
- safe runtime hook boundary.

## Decision

```yaml
Batch 1 identity: PASS
Field interpretation dormant anchor: CONFIRMED / bounded lead
Camera-cut provenance: NOT ESTABLISHED
Same-object bridge: NOT ESTABLISHED
Batch 2 runtime observation: BLOCKED
Runtime writes: NOT PERMITTED
Generic renderer/matrix scan: STOPPED
```

The dormant anchor is retained only if an independent
camera-cut/object-identity link appears. No further broad caller or renderer
search is justified in this batch.

## Re-entry conditions

Resume only if one of these new ownership anchors appears:

1. the camera-cut object is proven to reach `0x140280FC8` as the same `RCX`;
2. a confirmed cinematic lifecycle caller chain reaches an immediate caller of
   `0x140280FC8`; or
3. independent type/ownership evidence identifies its `RCX` object as the
   relevant camera/view structure.

## Evidence

- Script: `02-Research/Ghidra/ghidra-scripts/AuditCameraCutOverrideCandidate204.java`
- Run log: `02-Research/Ghidra/reports/camera-cut-override-candidate-204.log`
