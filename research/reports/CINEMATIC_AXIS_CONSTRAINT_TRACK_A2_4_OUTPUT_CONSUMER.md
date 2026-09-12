# Cinematic Axis-Constraint — Track A2.4 Output Consumer Audit

Status: `PARTIAL / NO PROMOTABLE EFFECTIVE CONSUMER`.

## Identity and method

- Program: `Stalker2-Win64-Shipping.exe (2.0.4)`
- SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- Identity: `PASS`
- Method: bounded read-only instruction inventory for paired `+0x60/+0x64`
  accesses, followed by local context inspection of two strongest candidates.
- No decompiler, renames, hooks, writes or production changes.

## Confirmed source/output boundary

The known current-build camera/view writer at `0x140AF4022` still provides the
only proven provenance for the pair:

```text
source +0x259 & 0x2
    → source +0x258
    → output RBX +0x60 = axis value
    → output RBX +0x64 = 1
```

The broader inventory found many functions containing paired `+0x60/+0x64`
accesses. Because these displacements are common in unrelated structures,
pairing alone is not an ownership proof.

## Targeted candidate results

### `FUN_140B1C7A8`

This function reads `R15 + 0x60` and `R15 + 0x64`, then copies the values into
stack-backed/local structures and passes them through subsequent calls. The
shape is consistent with copying, marshaling or serialization. No independent
camera/FOV/aspect ownership was established.

Classification: `REJECTED AS EFFECTIVE PROJECTION CONSUMER`.

### `FUN_140B20318`

This function tests `RCX + 0x64` and later reads `RAX + 0x60` in arithmetic,
but its body is an iterator/state-processing routine. It has no confirmed
camera source pointer, FOV field, aspect field or projection construction
boundary. The apparent `+0x5c` context is not enough to promote it because that
displacement is also common in unrelated state layouts.

Classification: `BOUNDED LEAD ONLY`.

## Result

```yaml
output +0x60/+0x64 pair: confirmed in known writer
direct paired consumers: inventory obtained
credible camera/FOV/aspect consumer: not established
projection owner: unresolved
runtime read-only batch: deferred pending stronger consumer
runtime writes: prohibited
production Full Hor+: unchanged
```

This closes the current generic output-field inventory. Repeating global
`+0x60/+0x64` scans or expanding into matrix/renderer searches is not justified
by this evidence. A future continuation needs a new provenance anchor, such as
the validated writer's caller/output ownership chain or an independently
identified view-construction boundary.
