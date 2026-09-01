# Testing And Research Summary

## Current unified mod — v0.4.0

The current artifact is `STALKER2UltrawideFix.asi`, intended to replace the
older `STALKER2GameplayAspectFix.asi`. Do not load both files together; remove
the older ASI before installing the unified one.

```ini
[Gameplay]
Enabled=true

[Cinematics]
; Auto, Native, 16:9, 21:9, 32:9
AspectRatio=Auto
```

The INI is created automatically beside the ASI when missing. `Auto` uses the
runtime camera aspect and updates when the game resolution changes during the
same session. `Native` bypasses both cinematic hooks; forced 16:9, 21:9 and
32:9 modes provide custom cinematic framing with matching FOV. The obsolete
`FovCorrection` option is removed from generated INIs and migrated out of
existing INIs.

## Research progression

The project evolved through these bounded evidence phases:

```text
Gameplay camera-state discovery
→ gameplay writer identification
→ dynamic signature resolution
→ camera rebuild / re-arm lifecycle
→ cinematic lifecycle discovery
→ shared authoritative camera state
→ cinematic aspect-writer provenance
→ live cinematic FOV consumption boundary
→ Hor+ FOV feasibility
→ combined cinematic correction
→ unified gameplay/cinematic coordinator
→ post-cinematic handoff investigation
→ 21:9 gameplay regression analysis
→ dynamic runtime Auto aspect policy
```

The detailed historical plans are preserved in the
[`research archive`](research/), while the active follow-up work remains in
the [`backlog`](backlog/).

## Confirmed 2.0.4 evidence

### Gameplay

- The gameplay camera writer is resolved through a unique executable `.text`
  signature and validated by decoding `MOVSS [RBX+0x30], XMM0`.
- The generalized ultrawide predicate accepts the observed aspect above native
  16:9 and preserves that source aspect during the existing two-pass correction.
- 21:9 startup, manual `21:9 → 16:9 → 21:9`, death/load camera rebuild and
  32:9 regression were user-tested successfully.
- The player's selected FOV is preserved.
- The separate weapon/viewmodel FOV issue after loading on 21:9 is a known
  game-side problem and is outside this fix.

### Cinematics

- Legacy 2.0.3 and current 2.0.4 transition topology was reconstructed;
  current signature resolution is based on semantic instruction patterns, not
  fixed cinematic RVAs.
- The cinematic aspect store and ENTER/EXIT live-FOV consumer callsites are
  uniquely signature-resolved and fail closed on ambiguity or validation
  failure.
- The validated current boundaries are the aspect store equivalent of
  `RVA 0x6B7CB05` and live-FOV callsites equivalent to
  `RVA 0x2EE6936`/`0x2EE69A7` in the tested 2.0.4 image.
- On 21:9, runtime aspect `2.38889` produces correct cinematic framing and
  Hor+ FOV. On 32:9, runtime aspect `3.55556` produces Hor+ FOV about
  `126.87` from authored FOV `90`.
- Forced 16:9, 21:9 and 32:9 cinematic framing was user-tested. Forced 32:9
  at 2560x1440 correctly produced cinematic letterbox bars.
- `Auto` was user-tested without restarting through
  `16:9 → 21:9 → 32:9 → 16:9 → 21:9 → 32:9`; each cinematic aspect store and
  ENTER FOV boundary used the current aspect. Cinematic EXIT recovery into
  gameplay also passed at each tested aspect.
- Forced 21:9 uses the canonical 3440x1440 aspect `2.3888889`, producing
  cinematic FOV about `106.688` from authored FOV `90`.
- Native cinematic EXIT FOV recovery remains game-owned and untouched.
- Startup logs record uppercase SHA-256 identities for the loaded ASI and game
  executable. The tested 2.0.4 game identity was
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.

## Compatibility boundary

Gameplay and cinematic boundaries use guarded signature resolution and runtime
validation on the tested 2.0.4 executable. This supports resilience against
address relocation within a compatible build, but does not guarantee support
for 2.0.5 or another patch. A new executable identity requires fresh resolver
and runtime validation.

The validated v0.4.0 candidate was build-validated with ASI SHA-256:
`A763D3D275991FC2CDBC34A3CB5585FEF0A1CC13E0ECF0B2367188D123338ABF`.

## Closed and deferred research

- Static interpolation/scalar-shape candidate ranking was closed after
  runtime rejection of unrelated candidates.
- Legacy transition-hub mapping and live-FOV consumption recovery are closed;
  the current live-FOV boundary is confirmed.
- Aspect writer provenance and immediate-patch feasibility are closed for the
  tested path.
- Post-EXIT atomic B/C scheduling was tested and closed as a production
  solution: it preserved mechanics but did not remove the visible seam.
- Downstream writer/projection candidate searches were closed for the current
  evidence set without a promoted renderer consumer.
- Weapon/viewmodel ownership research remains deferred pending a new validated
  object or downstream projection anchor.
- Dynamic resolution changes during a running session are validated for
  `AspectRatio=Auto` across 16:9, 21:9 and 32:9. Configuration file changes
  still require a game restart.

## Testing limits

- Build success proves compilation and linking only.
- A signature match is not hook proof without decode and runtime evidence.
- Current cinematic and policy results are validated on Steam 2.0.4 only.
- `Native`, `Auto`, forced `16:9`, forced `21:9` and forced `32:9` were all
  runtime-tested on the native 5120x1440 display or in the Auto hot-switch
  sequence.
- The brief post-cinematic projection/FOV handoff seam remains a known
  limitation of the experimental cinematic integration.

## Release checklist

- Remove the older `STALKER2GameplayAspectFix.asi` before installing the unified
  ASI.
- Include only `STALKER2UltrawideFix.asi`, its INI and the required release
  documentation in the release package.
- Keep research ASIs, historical binaries, logs and Ghidra projects out of the
  release archive.
- Preserve the runtime identity line in support reports.
