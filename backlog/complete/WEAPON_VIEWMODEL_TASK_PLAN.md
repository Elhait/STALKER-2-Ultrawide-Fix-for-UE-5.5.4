# Weapon/Viewmodel Projection Research Task Plan

Status: Closed — the tested gameplay-camera and direct accessor paths were rejected; follow-up research moved to separate downstream ownership.

## Objective

Identify the separate weapon/viewmodel camera projection owner responsible for the visually incorrect weapon FOV after a cutscene, and compare its state before and after the pause-menu refresh.

## Established evidence

- The stable gameplay ASI corrects the observed gameplay aspect transition without changing the player's selected FOV.
- Runtime tracer evidence covered 7,944 complete `pre-481A`/`post-481A` pairs.
- `FUN_140AF481A` did not change `outputFov` in any observed pair.
- Fields `0x25C`, `0x260`, `0x261`, `0x9D0`, `0x9D4` and `0x9D8` remained stable in that run.
- The `FUN_140AF481A` hypothesis is falsified by runtime evidence.
- Weapon/viewmodel projection ownership remains unresolved.

## Approved scope

- Research-only static ownership analysis.
- Trace known gameplay camera outputs into downstream consumers and independently search for parallel first-person/viewmodel projection paths that do not consume the known gameplay output directly.
- Identify candidate weapon-camera FOV, projection-scale, aspect-derived or mode-state writers/readers.
- Compare candidates with known post-cutscene and pause-refresh execution paths.
- Produce a short list of candidates before any new runtime tracer is built.

## Explicit non-goals

- Do not modify `src/gameplay_aspect_fix.cpp`.
- Do not modify the gameplay aspect transition or add delayed replay.
- Do not add a hard-coded FOV multiplier.
- Do not implement or enable letterbox hooks.
- Do not make runtime compatibility claims from static evidence.
- Do not package or release any diagnostic artifact.

## Expected areas

- Temporary copy of the existing Ghidra project only.
- `02-Research/Ghidra` scripts or reports only if a durable research artifact is required.
- No stable source or release assets.

## Batches and validation

### Batch 1 — Static candidate discovery

- Trace known gameplay output fields into downstream consumers.
- Independently search for parallel first-person/viewmodel projection fields, writers or transforms that do not consume the known gameplay output directly.
- Record executable identity, addresses, instruction semantics and caller/data-table evidence.
- Current result: the broad `MOVSS` scan was narrowed to compact functions writing at least three fields from the `+0x30/+0x40/+0x50/+0x5c` family. Representative candidates at `0x14009BE14` and `0x1401F2128` were decompiled with callers.
- Candidate classification so far: `0x14009BE14` — `rejected` as a material/render-proxy update path; `0x1401F2128` — `unresolved`, currently a generic struct-builder candidate with no weapon/viewmodel ownership evidence.
- Validation: every candidate must have reproducible disassembly and an explicit reason it is distinct from the already falsified `FUN_140AF481A` path.

### Batch 2 — Ownership narrowing

- Identify candidate writers that can execute during cutscene exit and pause refresh.
- Prefer candidates with direct or structurally supported caller/context evidence.
- Current static classifications:
  - `0x14009BE14` — `rejected`: material/render-proxy update path.
  - `0x14010C076` — `rejected`: constructor/initializer for a camera-like object, not a transition-time writer.
  - `0x14038E760` — `rejected`: `ComputeLightGrid`/render-lighting path using `ViewSpace...` resources.
  - `0x1401F2128` — `unresolved`: generic struct builder with no weapon/viewmodel context.
  - `0x140186BE8` — `likely` generic render/view projection writer, but weapon ownership is not established.
  - `0x140311CB4` — `unresolved`: render-command/data construction path; no weapon-only evidence.
- The direct FOV+aspect scan found additional candidates, but none yet has a verified first-person/viewmodel caller or state owner.
- Weapon/viewmodel string discovery found reflection/configuration markers including `SetFirstPersonFieldOfView`, `InFirstPersonFieldOfView`, `SetEnableFirstPersonFieldOfView`, `GetWeaponInHandsMeshComponent` and a first-person predictive-camera cvar. Their inspected xrefs are reflection/data or configuration paths, not a verified projection writer.
- The first-person FOV reflection xrefs at `0x148667D10`/`0x148667D40` do not belong to a defined function in the analysed program, so they are not usable hook ownership evidence. The predictive-camera cvar xref at `0x143BF3BBC` is a settings/configuration consumer, not a weapon-camera state writer.
- Metadata-adjacent code pointers now resolve to real property accessors: `FUN_14559427E` and `FUN_145594320` update separate mode bits at `param_1+0x262`; `FUN_1455943C2` forwards a first-person FOV value through virtual slot `+0x610`; neighboring accessors use slots `+0x608` and `+0x618`. This establishes a likely first-person property/accessor chain, but not the final projection owner.
- Current classification: the accessor chain is `likely` first-person mode/FOV control evidence; the virtual-dispatch target and backing projection state remain `unresolved`.
- Batch result: a weapon/viewmodel-specific structural evidence chain is established, but the final projection owner is still unresolved. This is sufficient to plan a narrow read-only discovery tracer, not an implementation hook.
- Validation: classify each candidate as `confirmed`, `likely`, `unresolved` or `rejected` with a recorded reason; do not select an implementation hook yet.

### Batch 3 — Targeted runtime tracer plan

- Candidate observation points: the first-person FOV accessor at `0x1455943C2`, the first-person mode-bit accessors at `0x14559427E`/`0x145594320`, and the virtual dispatch owner reached through slot `+0x610`.
- Minimum observation phases: normal gameplay, immediately after cutscene exit, and immediately after pause-menu refresh.
- Minimum fields: call sequence/timestamp, caller address, incoming setter value where available, mode-bit result, object pointer validity, and the resolved virtual target address. Do not dereference unknown object fields in the first tracer.
- Installation requirements: resolve/validate only the selected accessor candidate(s), preserve the original call ABI, install all required observation hooks atomically or roll back on failure, and keep the tracer read-only with bounded logging.
- Validation: tracer scope must be read-only, bounded and separate from stable ASI; no FOV/aspect writes, delayed replay, hard-coded multiplier or letterbox hook.

### Batch 3 implementation status

- Added the separate diagnostic tracer source `src/weapon_viewmodel_trace.cpp` and build script `build-weapon-viewmodel-trace.cmd`.
- The tracer validates executable accessibility for the Steam 2.0.2 accessor RVAs, installs the three observation hooks with rollback, logs bounded accessor sequences, caller/object context, mode byte `+0x262`, vtable and `+0x610` target, and performs no game-state writes.
- Build validation passed and produced `STALKER2WeaponViewmodelTrace.asi`.
- Manual game testing completed for the available scenario: the tracer installed successfully but recorded no calls to the three first-person accessor hooks. This rejects that accessor family as the direct active runtime path for the observed transition, without proving that the functions are globally unused.
- The direct weapon/viewmodel accessor path is therefore closed as a negative diagnostic result. Pause-triggered rebuild/refresh ownership was tracked separately in `backlog/complete/ADS_PAUSE_REFRESH_TASK_PLAN.md`.

## Risks and safe failure

- Static references may represent shared camera state rather than weapon-only state; label this as unresolved until runtime correlation.
- If no distinct weapon/viewmodel owner can be established, stop and report the blocker rather than adding a timing workaround.
- Preserve the stable gameplay ASI and keep all diagnostic ASIs disabled outside explicit testing.

## Stop conditions and phase gates

- Stop on contradictory executable identity or ambiguous ownership evidence.
- Do not implement a hook until a candidate has static ownership evidence and a targeted runtime observation plan.
- If this research phase changes any tracked research script or report, perform the normal post-change Git review and record the bounded research outcome in `TASKLOG.md`.
- If no tracked files are changed, no task-log implementation entry is required. Runtime implementation remains a separate future task.
