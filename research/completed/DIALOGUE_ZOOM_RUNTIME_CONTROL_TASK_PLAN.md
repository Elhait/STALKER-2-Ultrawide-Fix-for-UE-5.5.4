# Dialogue zoom runtime control — task plan

## Objective

Research a native/runtime way to control gameplay dialogue camera zoom without
changing cinematic defaults, with the future public configuration:

```ini
[Dialogue]
; Native, Reduced, Disabled
Zoom=Reduced
```

The required user-facing contract is relative to the actual gameplay FOV
selected in the settings, not the default constant. Let `G` be the current
user-selected gameplay FOV and `D` the native dialogue target (currently
`DialogFOVDefault=70.0`): `Native` preserves the native transition to `D`,
`Disabled` uses `G` as the effective dialogue target, and `Reduced` attenuates
the native delta between `G` and `D` rather than selecting a fixed absolute
FOV.

## Established evidence and current state

- The reference `Sota_NoDialogueZoom_FOV_90_P.pak` contains a
  `CoreVariables.cfg` bpatch setting both `DialogFOVDefault=90.0` and
  `CutsceneFOVDefault=90.0`.
- This establishes `DialogFOVDefault` as a dialogue FOV baseline reference, but
  also confirms that the reference mod changes the unrelated cinematic baseline.
- The desired implementation must leave `CutsceneFOVDefault` untouched and must
  remain separate from the unresolved weapon/viewmodel post-cinematic branch.
- `FOVDefault=90.0` is only the game default. It must not be used as the
  `Disabled` target when the player selected another gameplay FOV.
- `SettingsFOVParameterName=DisplayFOV` and
  `CurrentFOVParameterName=CurrentFOV` are executable-search anchors for
  distinguishing the selected/current gameplay FOV from the default constant.
- On the canonical current 2.0.4 Ghidra entry, `DialogFOVDefault`,
  `CutsceneFOVDefault` and `FOVDefault` reference `FUN_143105725`, while
  `DisplayFOV` and `CurrentFOV` reference `FUN_14365F5A2` (with an additional
  `CurrentFOV` reference in `FUN_143105725`). These are constrained semantic
  ownership candidates, not dialogue ENTER/EXIT proof. See
  `02-Research/reports/dialogue-fov-executable-ownership-audit-204-2026-09-02.md`.

## Approved scope

1. Use the reference patch only to establish names, values and ownership clues.
2. Identify the native dialogue transition in current Steam 2.0.4 runtime/static
   evidence.
3. Determine whether `Native`, `Reduced` and `Disabled` can be expressed through
   a bounded game-owned transition or target adjustment.
4. Produce a bounded research report and a recommendation before any production
   implementation.

## Explicit non-goals

- Do not copy or install the Sota `.pak`.
- Do not change `CutsceneFOVDefault` or cinematic authored FOV.
- Do not modify the stable production ASI in the discovery phase.
- Do not combine this with weapon/viewmodel research.
- Do not assume a fixed FOV such as 80 degrees for `Reduced`.
- Do not hard-code 90 degrees for `Disabled`; resolve the actual selected
  gameplay FOV at runtime.
- Do not create a broad renderer scan or unrelated camera tracer.

## Batches and validation

### Batch 1 — reference and terminology

Record the reference path and exact patch content. Separate dialogue baseline
from the collateral cinematic change.

### Batch 1.5 — existing-feature semantic audit

While resolving `CoreVariables` and adjacent packaged GameData, record concrete
named parameters and values related to existing functionality: gameplay FOV or
aspect, cinematic/cutscene FOV or aspect, dialogue camera behavior,
first-person/viewmodel FOV, letterbox/widescreen, projection, blend and
transition. Map each item to its package/path and flag plausible relations to
the current ASI for later validation. Do not infer runtime ownership or replace
production logic from names alone.

### Batch 2 — data-first dialogue anchor discovery

Inspect packaged `GameData/CoreVariables` references and other dialogue/FOV
data-driven anchors first. Use those semantic anchors to constrain the
identity-gated executable search, then confirm ownership and lifecycle through
bounded runtime evidence. A matching GameData variable name is an anchor, not
proof of runtime ownership.

Result: the installed Steam 2.0.4 `pakchunk0-Windows.pak` was inspected
read-only. `CoreVariables.cfg` contains `DialogFOVDefault=70.0`,
`CutsceneFOVDefault=90.0` and `FOVDefault=90.0`. Related settings contain
`FieldOfView=90.0`, `AspectRatio=EAspectRatio::Auto` and
`bUseLetterbox=false`. No dedicated native dialogue mode, cinematic aspect
policy or first-person/viewmodel FOV control was found in the bounded config
set. See `02-Research/reports/dialogue-zoom-gamedata-audit-204-2026-09-02.md`.

### Batch 3 — policy feasibility

Evaluate whether the native transition supports pass-through (`Native`), partial
attenuation (`Reduced`) and delta suppression (`Disabled`) without hard-coded
cinematic changes.

### Batch 2.75 — bounded runtime differential gate

The next runtime gate uses FOV `110` to separate the persistent selected value
from the native dialogue target. The read-only diagnostic records manual phase
markers `F8=A` (stable gameplay), `F9=B` (dialogue steady state) and `F10=C`
(stable gameplay after dialogue exit). It logs camera live FOV changes and
tracks writable-memory candidates first observed at the `110` baseline; it does
not write camera, FOV, dialogue or configuration state.

Expected discriminator:

```yaml
A: DisplayFOV/G=110, CurrentFOV/C≈110
B: G=110, C≈70 if CurrentFOV is live dialogue state
C: G=110, C≈110
```

The artifact is diagnostic only and was loaded separately from the production
ASI. The clean continuous camera trace is sufficient to establish the native
dialogue lifecycle and the `110 -> 70 -> 110` world-FOV curve. The generic
writable-memory candidate pass did not identify a live `DisplayFOV` or
`CurrentFOV` representation that follows the transition; this is not runtime
ownership proof for those semantic parameter names.

### Batch 2.5 — current-image executable ownership audit

The canonical 2.0.4 identity gate passed: SHA-256, `.text` size, ImageBase and
known runtime anchors are recorded in
`02-Research/reports/dialogue-fov-ownership-audit-204-2026-09-02.md`.

`FUN_143105725` is a current-image defaults/config initializer. It reads the
`FOVDefault`, `CutsceneFOVDefault` and `DialogFOVDefault` names through config
helpers and initializes global float destinations, including a distinct
dialogue-related destination observed after the `DialogFOVDefault` call. It
also registers the parameter-name values `DisplayFOV` and `CurrentFOV`. This
does not prove dialogue ENTER/EXIT ownership or the persistent selected FOV.

`FUN_14365F5A2` passes `DisplayFOV` and `CurrentFOV` to the same parameter/config
helper. Its bounded local path shows parameter registration/lookup semantics,
not a proven value direction or camera transition owner; full decompilation
timed out because the function is very large.

Gate result:

```yaml
PASS-A persistent selected FOV G: PARTIAL / UNRESOLVED
PASS-B dialogue target D: PASS as a data/config anchor
PASS-C transition owner: FAIL / UNRESOLVED
```

No runtime tracer or production implementation is justified. The dialogue
transition owner remains an open, separate bounded research question.

### Batch 2.8 — bounded writer-oriented target-assignment audit

The canonical 2.0.4 identity gate passed again before static analysis. The
bounded audit followed the validated camera writer to its immediate upstream
path and identified:

```text
FUN_1453BA300
  -> FUN_1432DE848
       -> FUN_1432DE984
            -> [camera + 0x230]
                 -> FUN_140AF4022
```

`FUN_1432DE848` is an update/normalization boundary and `FUN_1432DE984` is a
compact FOV calculator using a nearby camera field group. This establishes a
current-image camera update path but does not identify the dialogue-specific
target assignment. No static assignment of `DialogFOVDefault=70.0` or paired
ENTER/EXIT owner was found. Full details are in
`02-Research/reports/dialogue-camera-target-assignment-audit-204-2026-09-02.md`.

Gate result:

```yaml
Identity: PASS
Camera writer/update boundary: IDENTIFIED
Dialogue target assignment: NOT FOUND
Paired ENTER/EXIT owner: NOT FOUND
Production ASI changed: NO
Runtime tracer created: NO
Branch: OPEN / UNRESOLVED
```

The compact-field branch is now `DEFERRED`, not rejected: its scans were
inconclusive because the full-image method exceeded the reasonable time bound.
Do not resume it while the reference-assisted dialogue-call mapping remains
open. If revisited later, it must remain limited to writers/readers constrained
by the observed dialogue ENTER/EXIT behavior; do not expand into a generic
camera/renderer search.

### Batch 2.9 — reference-assisted dialogue runtime map

The WIDEBOY/FWS reference archive for Nexus mod 2337 was inspected read-only.
Its Dialogue FOV Slider is a runtime Lua/assembly hook, not a `CoreVariables`
`.pak` patch. It intercepts a dialogue FOV call, tracks the direction of the
incoming FOV samples and conditionally replaces the incoming value with a
plugin-owned dialogue target before the original code executes. The reference
does not establish a current 2.0.4 address or a clean paired native ENTER/EXIT
owner, and its above-gameplay-FOV behavior is documented as imperfect.

Full details are in
`02-Research/reports/wideboy-2337-dialogue-fov-reference-audit-2026-09-02.md`.

The next gate is limited to mapping the reference hook semantics to the
validated current 2.0.4 dialogue call boundary. Do not install a production
hook, copy the reference implementation, or expand into generic camera or
renderer search. A current equivalent must first be identity-gated and its
incoming FOV contract confirmed. If no bounded equivalent is found, stop this
reference-map branch and classify it as unresolved.

Static mapping succeeded on the canonical 2.0.4 image. The WIDEBOY dialogue
signature resolves uniquely at pattern RVA `0xD20F6E`, with a hook boundary at
RVA `0xD20F77`:

```text
MOVAPS XMM1,XMM6
CALL [RAX + 0x608]
```

The containing function is `FUN_140D20DFE`. Its bounded decompilation shows a
calculated FOV value being passed through the `+0x608` virtual-call boundary,
which is consistent with the WIDEBOY reference but does not yet prove whether
the value is a dialogue target or an intermediate blend sample.

Gate result:

```yaml
Identity: PASS
Current equivalent: STATIC MATCH
Match uniqueness: PASS
Incoming FOV contract: STATIC CANDIDATE
Runtime incoming-value semantics: UNCONFIRMED
Production ASI changed: NO
```

The next permitted action is one read-only runtime trace at this exact current
2.0.4 boundary, logging incoming FOV, object/vtable identity, dialogue phase
and camera `+0x230`. No production hook or copied reference assembly is
allowed before that runtime contract is confirmed.

The separate diagnostic artifact was built successfully:

```yaml
Artifact: STALKER2DialogueFovBoundaryRuntimeTrace204.asi
SHA-256: 409ED05772B9D2747084A6DB8B2744463D1F403271B35DCF2E1884AA3186FB90
Build: PASS
Runtime validation: PENDING
Writes: NONE
```

It validates both unique current-image signatures before installing its
read-only observers.

The user-run trace confirmed the runtime contract. One stable object/vtable
pair received incoming `XMM6/XMM1` values following the complete
`approximately 110 -> 70 -> 110` curve, while camera `+0x230` followed the
same curve with a one-sample observation offset. This proves that the mapped
boundary carries live/intermediate native blend samples, not a one-shot target
assignment. The boundary is therefore a valid reference-derived native blend
boundary, but direct replacement is not yet justified without explicit
ENTER/EXIT lifecycle handling and selected-FOV restoration.

```yaml
Runtime validation: PASS
Incoming semantics: LIVE_INTERMEDIATE_SAMPLES
Object/vtable stable: YES
Production ASI changed: NO
Implementation decision: PENDING
```

### Batch 2.95 — sample-transform feasibility gate

An experimental diagnostic was approved with one deliberately narrow write:
only `XMM1` at the mapped current 2.0.4 dialogue boundary may be transformed.
The diagnostic must not write `camera+0x230`, compact camera fields,
`DialogFOVDefault`, GameData or any recovery timer/state.

The classifier uses a provisional stable pre-dialogue baseline `G`, requires a
native-target-zone observation before entering the transform-active state, and
tracks `GameplayStable`, `DialogueEntering`, `DialogueActive` and
`DialogueExiting`. `Reduced` uses `G + (incoming-G) * 0.5`; `Disabled` uses
`G + (incoming-G) * 0`. F7 toggles the two experimental policies; F5 and F6
are not used by this diagnostic.

The cadence audit rejected the former ten-stable-sample baseline model. The
current feasibility build captures `G` from the first valid boundary sample,
keeps the lifecycle through the internal steady-state pause, and completes
only when ascending recovery returns within `1.0` degree of `G`. It applies the
sample-domain transform only while that lifecycle is active and records every
valid boundary sample and inter-hit interval.

Artifact:

```yaml
Artifact: STALKER2DialogueFovSampleTransformFeasibility204.asi
SHA-256: 294AB26B12F1FA82494B9B9576ED531B4BF2DFE8F852A12E861BECAE570C8DD8
Build: PASS
Runtime validation: PENDING
Production ASI changed: NO
Allowed write: XMM1 only at RVA 0xD20F77
```

The required matrix is: stable gameplay at `G≈110`, dialogue ENTER/steady/EXIT,
ADS after dialogue, ADS before dialogue followed by release and dialogue, and a
second dialogue without restarting the game. Any ADS false-positive or failed
post-EXIT restoration stops the feasibility gate; do not patch the classifier
with ad-hoc thresholds or promote it to production.

### Batch 4 — final review

Record confirmed evidence, hypotheses, unresolved points and the smallest next
gate. No production implementation is justified until a concrete owner and
safe lifecycle boundary are identified.

## Risks and safe failure

- A default-value patch may affect cinematic FOV and create compatibility issues.
- Dialogue and cinematic camera states may share a transition path; ownership must
  be proven before proposing a hook.
- If no bounded native owner is found, stop at `UNRESOLVED` rather than scanning
  generic camera/renderer code.

## Stop conditions

- Stop if the dialogue mechanism cannot be separated from `CutsceneFOVDefault`
  with current evidence.
- Do not infer runtime ownership solely from a matching GameData variable name.
  A data-layer match is a semantic anchor; lifecycle ownership still requires
  static/runtime confirmation.
- Stop if static/runtime evidence does not produce a concrete dialogue transition
  owner suitable for a bounded gate.
- Do not implement `Reduced` from an arbitrary fixed FOV.
- A config/default initializer or parameter-name registration function is not a
  dialogue transition owner without a bounded runtime/lifecycle connection.

## Expected final Git review

Review only the task plan, bounded research helpers/reports and reference notes.
No production ASI, gameplay/cinematic logic or release asset may change in this
discovery phase.
