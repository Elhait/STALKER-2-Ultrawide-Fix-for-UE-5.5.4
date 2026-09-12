# Cinematic Axis-Policy Output Ownership — Batch 1

Status: `PASS — immediate output ownership bridge established; lifecycle tracer not yet run`.

## Identity

- Program: `Stalker2-Win64-Shipping.exe (2.0.4)`
- SHA-256: `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`
- Image base: `0x140000000`
- Known writer: `0x140AF4022`
- Identity: `PASS`
- Method: bounded read-only reference and call-site instruction audit.

## Call-site evidence

The current image contains a direct call from `FUN_1453BA300`:

```text
0x1453BA30B  MOV RSI, R8
0x1453BA311  MOV RDI, RCX
0x1453BA322  CALL 0x140AF4022
0x1453BA327  MOV RAX, qword ptr [RDI]
0x1453BA32A  MOV RCX, RDI
0x1453BA330  MOV R8, RSI
0x1453BA333  CALL qword ptr [RAX + 0x638]
```

The known writer establishes `RCX` as the source object and `R8` as the
output object. The caller preserves those arguments as `RDI` and `RSI`, then
passes the same pair to the immediate virtual downstream call. This is a
reproducible object-identity bridge, not a displacement-only inference.

## Result

```yaml
writer caller: CONFIRMED — FUN_1453BA300
source argument: RDI, originating from caller RCX
output argument: RSI, originating from caller R8
immediate downstream call: CONFIRMED — [RAX + 0x638]
same source/output pair preserved: CONFIRMED
downstream axis/FOV/aspect semantics: NOT YET DECODED
runtime lifecycle observation: NOT YET RUN
```

The direct data reference at `0x14B1DC778` and an unrelated data reference at
`0x14866CCD8` were not treated as callers. Only the executable call site at
`0x1453BA322` was promoted.

## Gate for the next batch

Batch 1 passes the output-ownership gate. The next bounded action is to inspect
the immediate downstream virtual call while preserving the `RDI`/`RSI` object
identity and classify whether it consumes camera/view fields or merely forwards
the pair. Runtime tracing may proceed only through an existing or separately
approved diagnostic-only read boundary; no memory writes are authorized.
