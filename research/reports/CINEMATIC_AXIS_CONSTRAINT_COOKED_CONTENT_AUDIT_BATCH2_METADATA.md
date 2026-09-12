# Cinematic Axis-Constraint Cooked Content Audit — Batch 2 Metadata

Status: `PARTIAL PASS / BOUNDED LEAD` — reflection metadata is correlated to
current-build executable strings; native function binding and serialized game
content usage remain unresolved.

## Identity

- Executable: `Stalker2-Win64-Shipping.exe`, Steam 2.0.4.
- Executable SHA-256:
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Image base: `0x140000000`.
- Identity: `PASS`.

## 2A — Cooked ownership/content correlation

The IoStore directory indexes expose many camera, cinematic, cutscene and
FOV-related asset paths across the content chunks. The names are not enough to
promote a game-content owner: many hits are technical materials, camera
helpers, animation names or UI assets. No STALKER-specific serialized
`AspectRatioAxisConstraint` value or camera-cut owner has yet been decoded.

No matching `.usmap` or equivalent property schema is available in the
workspace. This limits raw property-value interpretation but does not block
relationship analysis or the reflection/native track.

Status: `IN PROGRESS / OWNER NOT YET ESTABLISHED`.

## 2B — Reflection-to-native correspondence

The `global.utoc` `ScriptObjects` container contains the following reflected
objects/functions:

- `EAspectRatioAxisConstraint`
- `SetAspectRatioAxisConstraint`
- `LocalPlayer` and `Default__LocalPlayer`
- `CameraComponent` and `Default__CameraComponent`
- `CineCameraComponent`
- `PlayerCameraManager` and `Default__PlayerCameraManager`
- `CalculateProjectionMatrix`
- `MovieSceneCameraCutSection` and `Default__MovieSceneCameraCutSection`

The metadata relationship gives a useful ownership refinement:
`SetAspectRatioAxisConstraint` is nested under the reflected `CameraComponent`
outer. `CalculateProjectionMatrix` is nested under `KismetRenderingLibrary`,
so it must not be treated automatically as the `FMinimalViewInfo` projection
owner.

The same names are present in the current 2.0.4 executable string pool:

```text
SetAspectRatioAxisConstraint  file offset 154378338
EAspectRatioAxisConstraint    file offset 154378558
CalculateProjectionMatrix     file offset 154266683
AspectRatioAxisConstraint     6 matches
```

These are valid current-image metadata/string correspondence leads. They do
not identify a native code address, prove a UFunction registration binding,
prove a property offset or prove that the cinematic path invokes the setter.
The string offsets are not executable RVAs and must not be used as hook
targets.

Status: `PARTIAL PASS / BOUNDED LEAD`.

## Phase result

Batch 2 has produced a concrete new evidence class and is not blocked by the
absence of `.usmap` alone. The next bounded work is:

1. use the reflection indices/outer relationships to narrow the relevant
   engine class/function family;
2. inspect available native registration, name-pool, export or byte-pattern
   correspondence without guessed addresses; and
3. continue 2A only toward identifiable STALKER-specific camera/cinematic
   assets, not generic camera-name matches.

Runtime writes, runtime MaintainYFOV testing and production changes remain
unauthorized until a safe native or content boundary is established.
