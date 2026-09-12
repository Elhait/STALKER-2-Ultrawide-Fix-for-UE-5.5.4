# Cinematic Axis-Constraint Cooked Content Audit — Batch 1

Status: `COMPLETE / POSITIVE METADATA ANCHOR` — package inventory and UE
reflection metadata are available; serialized game-content usage is pending.

## Scope and safety

This was a read-only inventory of the installed Steam 2.0.4 content
containers, available mappings/tools and IoStore reflection metadata. No
`.pak`, `.utoc`, `.ucas`, executable, source, ASI, INI or release asset was
modified. No package was unpacked during this batch.

## Package inventory

Installed content root:

`E:\Steam\steamapps\common\S.T.A.L.K.E.R. 2 Heart of Chornobyl\Stalker2\Content\Paks`

- Legacy `.pak`: 41 files, approximately 271 MB total.
- IoStore `.ucas`: 42 files, approximately 159.7 GB total.
- IoStore `.utoc`: 42 files, approximately 68.4 MB total.
- The containers use Oodle compression where reported by `retoc`.
- `pakchunk0-Windows.utoc` reports 12,924 packages and 22,044 chunks.
- `global.utoc` contains the `ScriptObjects` global reflection container.

The installed content is therefore not a simple legacy-Pak-only layout. The
directory indexes expose package paths through IoStore and can be inspected
without extracting the full `.ucas` payload.

## Tools and format evidence

- `retoc` 0.1.5 is available at
  `E:\Work\Slaker2 mods\Instruments\retoc-0.1.5\retoc.exe` and supports
  IoStore listing, unpacking, script-object printing and asset-registry
  parsing.
- `repak` 0.1.7 is available at
  `E:\Work\Slaker2 mods\Instruments\repak-0.1.7\repak.exe` and successfully
  reads legacy `.pak` metadata. `pakchunk0-Windows.pak` reports format V11
  with 5,170 file entries.
- No matching `.usmap` or equivalent property schema was found in the
  workspace inventory. Serialized property interpretation must therefore
  remain gated until a compatible mapping/schema is located or a supported
  conversion path is established.

## Positive metadata anchor

`retoc print-script-objects` on:

`E:\Steam\steamapps\common\S.T.A.L.K.E.R. 2 Heart of Chornobyl\Stalker2\Content\Paks\global.utoc`

produced 205,224 script-object metadata lines and included these relevant
objects/functions:

- `EAspectRatioAxisConstraint`
- `SetAspectRatioAxisConstraint`
- `LocalPlayer` and `Default__LocalPlayer`
- `CameraComponent` and `Default__CameraComponent`
- `CineCameraComponent`
- `PlayerCameraManager` and `Default__PlayerCameraManager`
- `CalculateProjectionMatrix`
- `MovieSceneCameraCutSection` and `Default__MovieSceneCameraCutSection`

The metadata also records stable global/outer indices for these objects. This
is a concrete engine reflection anchor and is stronger than searching stripped
native text for the original UE API names.

## Directory-index observations

Relevant package paths are present across the IoStore indexes, but simple name
matching is noisy. `pakchunk0` includes camera/FOV/material and technical
assets; other chunks include cinematic content, camera modifiers and many
dialogue/cutscene assets. Names alone do not establish ownership or serialized
axis-policy values.

The `global.utoc` script-object metadata is therefore the strongest Batch 1
result. It identifies the reflection vocabulary needed to guide Batch 2, but
it does not yet show whether STALKER-specific cooked assets set
`AspectRatioAxisConstraint`, whether the value is inherited from a native CDO,
or whether camera-cut evaluation synthesizes it at runtime.

## Phase result

`Batch 1: COMPLETE / POSITIVE METADATA ANCHOR`.

Batch 2 is now unblocked for a narrow property/content search, subject to a
mapping/schema gate. The next search should prioritize camera/cinematic asset
subtrees and relevant class/property metadata rather than extracting all
containers. If no compatible schema is available, that limitation must be
reported explicitly and no raw numeric enum value should be promoted.
