# Cinematic Axis-Constraint Runtime Reflection — Batch 1

Status: `COMPLETE / BLOCKED OUTCOME` — the existing ASI diagnostic
infrastructure does not provide a safe UE reflection bridge.

## Scope

The bounded review inspected the existing source and research infrastructure
for reusable runtime access to `UObject`, `UClass`, `FProperty`, `UFunction`,
global object registries or equivalent reflected APIs. No source, build,
runtime artifact, game package or production file was changed.

## Findings

- Existing diagnostic source contains lifecycle-specific camera/FOV traces,
  SafetyHook/Zydis-based instruction hooks and logging, but no UE reflection
  SDK types or resolved reflection globals.
- No existing `UObject`, `UClass`, `FProperty`, `UFunction`, `GObjects`,
  `GUObjectArray`, `FindObject` or equivalent runtime bridge was found in the
  stable source or available diagnostic files.
- The `global.utoc` ScriptObjects metadata remains a valid external metadata
  anchor, but it does not itself expose live runtime object pointers or native
  function addresses to the ASI.
- A reflection tracer would therefore require a new native correspondence
  step before it could safely resolve classes/properties at runtime.

## Decision

`Batch 1: COMPLETE / BLOCKED OUTCOME`.

No diagnostic tracer was created. No guessed reflection global, object offset,
property offset or runtime write is authorized by this result.

The next bounded evidence step is not a broad runtime pointer scan. It is
native registration/metadata correspondence for the confirmed reflected
`CameraComponent::SetAspectRatioAxisConstraint` function family, using the
current Steam 2.0.4 executable identity. If that correspondence establishes a
safe reflection entry point, a separate tracer task may be reopened.

The cooked-content 2A track remains independent and may continue with owner
relationships that do not require decoding raw property values.
