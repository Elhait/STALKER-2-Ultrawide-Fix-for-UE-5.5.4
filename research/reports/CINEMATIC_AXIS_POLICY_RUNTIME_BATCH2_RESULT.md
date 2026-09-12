# Cinematic Axis Policy — Runtime Batch 2 Result

## Runtime identity and setup

- Log: `STALKER2CinematicAxisPolicyLifecycleTrace204.log`
- Runtime identity gate: **PASS**
- Steam build target: **2.0.4**
- Tracer mode: **read-only**
- Hook installation: **PASS**
- Runtime writes: **none**

## Observed values

The same source/output pair was observed throughout the trace:

```text
source = 0x22CD6EE5A40
output = 0x22AEEF469C0
```

| Lifecycle observation | source `+0x258` | source `+0x259` | gate `& 0x2` | output `+0x60` | output `+0x64` |
|---|---:|---:|---|---:|---:|
| Gameplay | `0x01` | `0x04` | false | `0x00` | `0` |
| Cinematic samples | `0x01` | `0x05` | false | `0x00` | `0` |
| Exit-pending sample | `0x01` | `0x04` | false | `0x00` | `0` |
| Restored gameplay | `0x01` | `0x04` | false | `0x00` | `0` |

## Result

**COMPLETE / NEGATIVE LIFECYCLE EVIDENCE for the tested path.**

- The axis-policy source byte `+0x258` remained `0x01` across gameplay,
  cinematic and exit observations.
- The writer gate `source +0x259 & 0x2` never became active.
- The output axis-policy fields `+0x60/+0x64` remained `0x00/0`.
- The only observed source-flag change was `+0x259: 0x04 ↔ 0x05`, which toggles
  bit `0x1`, not the confirmed propagation gate `0x2`.
- The source/output object identity remained stable in this trace.

This does not disprove the UE axis-constraint mechanism. It shows that the
confirmed writer boundary did not activate the known axis-policy propagation
path during this capture. Therefore the trace does not provide evidence that
cinematic lifecycle changes the effective axis policy at this level.

## Lifecycle-marker limitation

The log contains repeated `EXIT`/`ENTER` pairs close together. They may reflect
multiple camera-cut evaluations or nested/repeated callbacks rather than one
clean cinematic session. The value result is still usable because the same
source/output pair and field values were observed, but the marker sequence
should not be treated as a precise single-session timeline.

## Decision

```yaml
Batch 1 same-object handoff: PASS
Batch 1.1 vslot +0x638: BOUNDED PARTIAL / unresolved
Batch 2 read-only lifecycle: COMPLETE / negative activation evidence
Axis policy activation at confirmed writer: NOT OBSERVED
Projection consumer: UNRESOLVED
Runtime writes: NOT PERMITTED
Production Full Hor+: unchanged
```

Further progress requires a genuinely new anchor for downstream interpretation
or a cleaner independently identified camera-cut lifecycle boundary. Another
raw displacement scan or random CVar sweep is not justified by this result.
