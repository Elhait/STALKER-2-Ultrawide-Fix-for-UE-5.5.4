# Testing And Research Summary

## Stable Gameplay Aspect Fix

### Confirmed source and runtime evidence

The stable gameplay module uses a dynamic `.text` signature resolver. It does not depend on a fixed RVA, executable hash, timestamp or image size. Installation requires exactly one complete signature match and successful instruction validation as:

```text
MOVSS [RBX+0x30], XMM0
```

The resolver refuses safely on ambiguity, decode failure or instruction mismatch. The player's selected FOV is preserved; the module applies the observed aspect-state transition rather than a hard-coded FOV multiplier.

Manual testing at 5120×1440 (32:9) confirmed that the stable gameplay ASI loads and reapplies the gameplay aspect correction after the tested cutscene transition. This is runtime evidence for the tested executable/session only, not a general compatibility guarantee for newer builds.

The cinematic state is 16:9 (`aspect=1.777778`) with black bars, while the native gameplay state is 32:9 (`aspect=3.555556`). These values identify the two aspect domains, but the exact direction of every transition must be established with event markers rather than inferred from sequence numbers alone.

## Runtime State-Diff Result

The read-only transition tracer sampled 12 values from the validated gameplay-camera writer context and recorded manual markers for:

```text
F8  cutscene-exit
F9  ads-enter
F10 ads-exit
F11 pause-open
F12 pause-close
```

Marker-correlated runtime evidence showed:

- `outputFov == inputFov` throughout the tested session;
- no distinct state pattern in `+0x248`, `+0x25C`, `+0x260..263` at ADS or pause correction;
- no observable weapon/viewmodel-specific state change in the 12 sampled gameplay-camera fields;
- the gameplay-camera state branch is therefore rejected as the causal owner of the weapon/viewmodel FOV symptom.

The known `[RBX+0x30]` context and its sampled neighboring fields should not be re-traced for this symptom without contradictory evidence.

## Experimental Letterbox Research

BigChenga reference analysis identified two letterbox setter paths for the analyzed Steam 2.0.2 executable. The first experimental implementation caused an access violation reading `0xffffffffffffffff` during a cutscene. Removing the experimental ASI restored normal cutscene operation.

The experimental letterbox ASI is disabled and must not be used for further runtime testing until the hook boundaries and return contracts are redesigned and revalidated. Letterbox work remains separate from the stable gameplay ASI and is not release-ready.

## Weapon/Viewmodel Research Status

The direct first-person reflection/accessor family and several statically plausible ADS/render candidates produced zero runtime hits in the tested scenarios. These paths are rejected or deferred as direct active owners; semantic names and generic renderer structures are not sufficient ownership evidence.

The remaining research must start from a different weapon/viewmodel object or downstream projection consumer. No implementation hook, delayed replay, forced gameplay transition or hard-coded weapon FOV multiplier is justified by the current evidence.

## Rejected Approaches

### INI Aspect Constraint

`AspectRatioAxisConstraint=AspectRatio_MaintainYFOV` did not fix the live gameplay camera state because the game overwrites the relevant runtime values after loading gameplay.

### Fixed FOV Values

Forcing FOV 90 changed the symptom but ignored the player's setting and did not reproduce the correct aspect transition. It was rejected.

### Gameplay-Module Letterbox Hooks

Combining the experimental letterbox hooks with the stable gameplay hook caused runtime instability. The two lifecycles remain separate.

### Broad Renderer and Object Tracking

Broad object-array, viewport, camera and generic renderer candidates did not provide reproducible weapon/viewmodel ownership. They were rejected or left unresolved rather than promoted from naming or proximity alone.

### Dumper-7 SDK Dump

Dumper-7 found `GObjects` but could not initialize `FNamePool` because the game layout was incompatible with its name decoder. It remains historical research evidence, not a basis for the stable fix.

## Testing Boundaries

- A build proves compilation only; it does not prove resolver operation or in-game behavior.
- A signature match is not hook proof without decode and runtime validation.
- Runtime conclusions apply only to the executable identity and manual test session that produced them.
- Release archives and historical test artifacts must remain separate.
- Future weapon/viewmodel research requires a new observable object/context or downstream projection anchor.
