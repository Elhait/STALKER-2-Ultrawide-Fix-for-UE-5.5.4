# Testing And Research Summary

## Current unified mod — v0.6.0

The current artifact is `STALKER2CameraTweaks.asi`, intended to replace the
older `STALKER2UltrawideFix.asi` and `STALKER2GameplayAspectFix.asi`. Do not
load old and new files together; remove
the older ASI before installing the unified one.

The production configuration has four independent areas:

```ini
[Gameplay]
Enabled=true

[Cinematics]
AspectRatio=Auto

[Dialogue]
Zoom=Reduced

[Hotkeys]
Enabled=false
CinematicCycle=F9
DialogueCycle=F10
```

`Gameplay.Enabled` controls gameplay aspect correction. `Cinematics.AspectRatio`
selects automatic, native or forced cinematic framing. `Dialogue.Zoom` supports
`Native`, `Adaptive`, `Reduced` and `Disabled`; the default `Reduced` mode uses
half of the native optical zoom strength relative to the actual gameplay FOV.
`Hotkeys` are optional testing controls, disabled by default, and support
`F1-F12`, `0-9` and `A-Z`. They select the policy for the next corresponding
cinematic or dialogue and do not rebuild an already active lifecycle.

The INI is created automatically beside the ASI when missing. `Auto` uses the
runtime camera aspect and updates when the game resolution changes during the
same session. `Native` bypasses both cinematic hooks; forced 16:9, 21:9 and
32:9 modes provide custom cinematic framing with matching FOV. The obsolete
`FovCorrection` option is not part of the unified v0.6.0 configuration. Settings
from previous INI files are not migrated; managed descriptions and categories
are still synchronized when the new INI already exists.

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
→ GameData dialogue-FOV semantic audit
→ dialogue configuration/ownership and target-assignment audit
→ WIDEBOY runtime-boundary reference audit
→ historical-2.0.4 dialogue boundary discovery
→ dialogue live-sample lifecycle classification
→ Adaptive and optical half-strength feasibility
→ EXIT discontinuity diagnosis and recovery anchoring
→ production dialogue integration and hotkey/persistence validation
→ cross-patch dialogue resolver validation
→ post-cinematic atomic handoff investigation
→ physical FOV setter and native bypass investigation
→ native bypass deferred after bounded static/runtime research
→ production combined atomic integration
→ cross-patch production resolver audit 2.0.2–2.0.5
→ production runtime validation on Steam 2.0.5
→ v0.6.0 release preparation
```

The detailed historical plans are preserved in the
[`research archive`](research/), while the active follow-up work remains in
the [`backlog`](backlog/).

## Historical Steam 2.0.4 evidence

The following sections preserve evidence collected from the Steam 2.0.4
executable. They are historical research evidence, not the current v0.6.0
runtime-validation basis. Current production runtime validation is on Steam
2.0.5; older-build runtime support is not claimed.

### Gameplay

- The gameplay camera writer is resolved through a unique executable `.text`
  signature and validated by decoding `MOVSS [RBX+0x30], XMM0`.
- The generalized ultrawide predicate accepted the observed aspect above native
  16:9 and preserved that source aspect during the historical two-pass
  correction. v0.6.0 production uses the validated atomic apply instead.
- 21:9 startup, manual `21:9 → 16:9 → 21:9`, death/load camera rebuild and
  32:9 regression were user-tested successfully.
- The player's selected FOV is preserved.
- The separate weapon/viewmodel FOV issue after loading on 21:9 is a known
  game-side problem and is outside this fix.

### Native gameplay aspect reevaluation control

- A read-only UE4SS automatic dump captured the same live `CameraComponent`,
  `Stalker2.CameraManager` ownership references and `PlayerCameraManager`
  across a manual native gameplay aspect transition. Numeric object IDs may
  change after reloads; the evidence is tied to the same live objects within
  the tested session.
- The tested sequence was: `32:9` gameplay with the incorrect vertical
  framing, manual `16:9` with aspect constraint enabled and correct vertical
  framing, then return to `32:9` after the native recalculation.
- The observed states were:

  ```text
  A — 32:9 before transition:
      FOV=90, AspectRatio=3.555556, bConstrainAspectRatio=false

  B — 16:9 constrained transition:
      FOV=90, AspectRatio=1.777778, bConstrainAspectRatio=true

  C — 32:9 after transition:
      FOV=90, AspectRatio=1.777778, bConstrainAspectRatio=false
  ```

- `AspectRatioAxisConstraint` remained `MaintainXFOV` (`Axis=1`) and
  `bOverrideAspectRatioAxisConstraint` remained `false` in the captured
  states; no axis-policy transition was observed.
- The final correct 32:9 gameplay result therefore does not require the live
  camera `AspectRatio` to equal the physical display aspect. States A and C
  share `FOV=90` and `bConstrainAspectRatio=false` but have different camera
  aspect state and different visual results.
- The result confirms a native/settings-driven reevaluation sequence, not a
  single sufficient final property state. A storage-only property write is
  not equivalent to the native transition and is not promoted as a solution.
- This control intentionally excludes the cinematic CameraTweaks branch. The
  gameplay fix is not treated as the source of this native behavior; it is a
  separate production workaround that preserves the player's FOV while
  replaying the useful gameplay transition behavior.
- Confirmed: native A/B/C transition, same Camera/CameraManager/PCM
  ownership, authored FOV preserved at `90`, and no observed axis-enum
  transition. Unresolved: the exact native operation/evaluation event that
  performs the reevaluation, where the downstream view/projection rebuild is
  triggered, and whether an equivalent path can be invoked during a
  cinematic without the Hor+ FOV rewrite.

### Cinematics — historical 2.0.4 evidence

- Legacy 2.0.3 and historical 2.0.4 transition topology was reconstructed;
  current signature resolution is based on semantic instruction patterns, not
  fixed cinematic RVAs.
- The cinematic aspect store and ENTER/EXIT live-FOV consumer callsites are
  uniquely signature-resolved and fail closed on ambiguity or validation
  failure.
- The validated current boundaries are the aspect store equivalent of
  `RVA 0x6B7CB05` and live-FOV callsites equivalent to
  `RVA 0x2EE6936`/`0x2EE69A7` in the historical 2.0.4 image.
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
  executable. The historical 2.0.4 game identity was
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.

### Dialogue — historical 2.0.4 evidence

- The packaged `CoreVariables` reference established `DialogFOVDefault=70.0`
  as the native dialogue baseline. `CutsceneFOVDefault` remains separate and is
  not modified by this feature.
- Configuration/default registration and `DisplayFOV`/`CurrentFOV` parameter
  plumbing were audited but were not promoted as dialogue lifecycle owners.
- The WIDEBOY dialogue reference was used only to identify a plausible runtime
  boundary. The historical 2.0.4 equivalent resolved uniquely at hook boundary
  `RVA 0xD20F77`, where `XMM1` carries live native dialogue FOV samples.
- The boundary was proven to carry the native live stream, not a one-shot target:
  gameplay around `110` descends smoothly to `70` during dialogue and recovers
  to gameplay after EXIT. The boundary is not traversed during the tested
  ADS-only scenario.
- The former broad candidate searches for `DisplayFOV`, `CurrentFOV`, compact
  camera fields and downstream projection consumers did not identify a safer
  native target owner. They remain historical/deferred evidence, not production
  dependencies.
- Production dialogue policies are:
  - `Native` — pass through the game's original dialogue stream, targeting its
    native `70°` dialogue FOV.
  - `Adaptive` — preserve the native optical zoom strength relative to the
    actual gameplay baseline `G`, using projection-space geometry.
  - `Reduced` — apply half of the Adaptive optical zoom strength in projection
    space.
  - `Disabled` — hold the captured gameplay baseline `G` through dialogue.
- Adaptive and Reduced were validated at high gameplay FOV baselines near
  `90`, `110` and `120`; endpoint values matched the optical model and recovery
  returned to the actual captured gameplay baseline.
- The initial transformed EXIT path exposed the native EXIT sample jump. The
  validated EXIT anchor/recovery state now starts from the transformed dialogue
  endpoint and maps native recovery smoothly back to `G`, preserving native
  timing without camera writes or custom timers.
- Cinematic isolation resets dialogue transient state during cinematic active and
  recovery phases. The first dialogue after a cinematic captured the gameplay
  baseline rather than the cinematic FOV, and two sequential dialogues recovered
  without stale state.
- Historical production-candidate runtime validation on Steam 2.0.4 passed
  `Native`, `Adaptive`, `Reduced` and `Disabled`, including sequential cycles,
  cinematic-to-dialogue isolation, ADS-only specificity and configurable
  hotkey selection. This does not extend the current v0.6.0 runtime claim to
  Steam 2.0.4.

## Compatibility boundary

Gameplay, cinematic and dialogue boundaries use guarded signature resolution.
The current v0.6.0 production implementation was runtime-validated on Steam
2.0.5. The production resolver set was statically validated across Steam
2.0.2–2.0.5 despite relocated RVAs. This is static cross-patch portability
evidence only; runtime support for Steam 2.0.2–2.0.4 is not claimed. A future
executable identity still requires fresh resolver and runtime validation.

The current v0.6.0 production artifact is runtime-validated on Steam 2.0.5.
Its ASI SHA-256 is
`7F88F7A3547AB88D6E5358DE7E28757692DC4F651D72B6D31FD0BAC9E3351ACE`.
This build also contains the synchronized explanatory comments in the
auto-generated INI template; no executable logic or configuration defaults
changed.

## Closed and deferred research

- Static interpolation/scalar-shape candidate ranking was closed after
  runtime rejection of unrelated candidates.
- Legacy transition-hub mapping and live-FOV consumption recovery are closed;
  the current live-FOV boundary is confirmed.
- Aspect writer provenance and immediate-patch feasibility are closed for the
  tested path.
- Post-EXIT atomic B/C scheduling was tested and closed as a production
  solution: it preserved mechanics but did not remove the visible seam.
- The v0.6.0 atomic gameplay apply and first-descending-sample
  `RecoveryStart` handoff passed production runtime validation on Steam 2.0.5;
  the old staged `0x5` replay is not used by the production path.
- Native cinematic FOV bypass research remains deferred. The game's native
  post-cinematic FOV recovery is preserved and is not rewritten by v0.6.0.
- Downstream writer/projection candidate searches were closed for the current
  evidence set without a promoted renderer consumer.
- Dialogue parameter plumbing and compact-field target-owner searches were
  closed or deferred after the concrete live boundary was established.
- The WIDEBOY-derived dialogue boundary was independently mapped and validated;
  its direction heuristic was not copied into production.
- Dialogue feasibility and production promotion passed with the four-policy
  lifecycle model, EXIT recovery anchor, cinematic isolation and ADS-specificity
  guard.
- Weapon/viewmodel ownership research remains deferred pending a new validated
  object or downstream projection anchor.
- Dynamic resolution changes during a running session are validated for
  `AspectRatio=Auto` across 16:9, 21:9 and 32:9. Configuration file changes
  still require a game restart.
- The native gameplay aspect reevaluation sequence above is validated from a
  bounded UE4SS runtime capture. The exact triggering native function and
  downstream projection owner were not identified; no compatibility or
  cinematic behavior claim follows from this control alone.

## Testing limits

- Build success proves compilation and linking only.
- A signature match is not hook proof without decode and runtime evidence.
- The current production gameplay/cinematic handoff is runtime-validated on
  Steam 2.0.5. Older-build evidence remains static portability evidence only.
- Dialogue runtime behavior is covered by the current 2.0.5 production test
  scope; no runtime compatibility claim is made for older Steam builds.
- Dialogue runtime validation covers Native, Adaptive, Reduced and Disabled,
  high-FOV baselines, smooth ENTER/EXIT recovery, sequential cycles,
  cinematic-to-dialogue isolation, ADS-only specificity and F9/F10 policy
  selection. Runtime hotkeys apply to the next lifecycle, not an active one.
- `Native`, `Auto`, forced `16:9`, forced `21:9` and forced `32:9` were all
  runtime-tested on the native 5120x1440 display or in the Auto hot-switch
  sequence.
- The game's native post-cinematic FOV recovery remains a visible native
  transition in some scenarios and is intentionally untouched.

## Release checklist

- Remove older `STALKER2UltrawideFix.asi` and `STALKER2GameplayAspectFix.asi` before installing the unified
  ASI.
- Include only `STALKER2CameraTweaks.asi`, its INI and the required release
  documentation in the release package.
- Keep research ASIs, historical binaries, logs and Ghidra projects out of the
  release archive.
- Preserve the runtime identity line in support reports.
