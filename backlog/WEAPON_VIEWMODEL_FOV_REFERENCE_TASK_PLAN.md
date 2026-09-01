# Weapon/Viewmodel FOV Reference Analysis

Status: In progress — Batch 1 reference analysis is complete; independent runtime comparison of the new BigChenga build remains pending.

## Objective

Analyse the unreleased BigChenga reference build for weapon/viewmodel FOV behavior and determine whether its findings identify a safe, reusable path for the deferred weapon-FOV problem.

## Established evidence and current state

- Reference source: `C:\Users\enton\Downloads\elhait.zip`, latest FWS profile `Stalker2.lua`.
- The reference profile identifies `UCameraComponent +0x234` as `FirstPersonFieldOfView`, `+0x230` as world FOV, `+0x254` as aspect and `+0x262 bit 2` as the first-person-FOV enable flag.
- Its camera signature resolves the same gameplay camera hook family / camera-writer region used by the stable gameplay aspect path; exact ownership equivalence with the current native implementation remains a separate validation point. Its added policy writes `+0x234` and enables the first-person flag.
- Its weapon-FOV policy derives first-person FOV from the live world FOV and a learned runtime baseline; it does not use a fixed FOV multiplier or a fixed `80/90` input.
- Its weapon/equipment policy discovers the player mesh through assignment ancestry and writes `+0x265` for the body, held item and attachments. This is separate from cinematic projection and requires independent ownership/runtime validation.
- The archive also contains an older UE4SS implementation that is explicitly resource-heavy and is reference evidence only.

## Approved scope

- Read-only analysis of the supplied reference archive and comparison with existing source/research evidence.
- Targeted runtime comparison of the reference behavior only if a runnable reference build is supplied and the user explicitly tests it.
- Identify reusable structural findings for a future native implementation.

## Explicit non-goals

- No copying of the FWS/Lua implementation into the stable ASI.
- No changes to stable gameplay aspect logic or cutscene letterbox/Hor+ logic in this task.
- No UE4SS integration, polling-heavy workaround or hard-coded FOV multiplier.
- No claim that the reference fixes the user's executable until manual runtime evidence is supplied.

## Batches

### Batch 1 — Reference/static analysis

- Inspect the archive contents and documented field map.
- Verify the camera hook family and distinguish weapon/viewmodel policy from cinematic FOV policy.
- Classify findings as confirmed reference structure, likely design, unresolved ownership or rejected approach.

Validation: archive contents, source text and comparison with current project evidence. No module execution.

Result:

- `confirmed reference structure`: same gameplay camera hook family, `+0x234` first-person FOV, `+0x262 bit 2` enable flag and runtime world-FOV input.
- `likely design`: adaptive first-person FOV derived from live world zoom and a learned baseline, avoiding ADS timing replay.
- `unresolved`: whether the assignment ancestry reliably identifies every player weapon/viewmodel object in the current runtime and whether the camera policy remains stable across rebuilds.
- `rejected for implementation`: the resource-heavy UE4SS polling script and any direct 1:1 Lua port into the stable ASI.

### Batch 2 — Optional manual reference comparison

- Compare the reference build's startup, pre-ADS, ADS and post-ADS behavior with the current runtime scenario.
- Record whether the stale weapon FOV is corrected at startup, after weapon swap, on ADS and after world/camera rebuild.

Validation: user-supplied runtime observations/logs at the exact executable version. No compatibility claim from UI appearance alone.

### Batch 3 — Future native design decision

- Only after runtime evidence, decide whether a separate experimental native task is justified.
- Reuse the shared resolver/hook infrastructure only if this does not alter stable gameplay behavior.

Validation: separate approved implementation plan. No source implementation in this reference-analysis task.

## Risks and safe failure

- Do not load the reference hooks together with our hooks when testing for behavior; potential same-site ownership conflicts must be avoided.
- Treat the reference's field map and comments as evidence, not proof of ownership in our build.
- Stop if the only native design requires broad object polling, guessed ancestry or hard-coded FOV compensation.

## Stop conditions and phase gates

- Do not begin native implementation until the reference behavior is manually correlated with the reported pre-ADS/post-ADS symptom.
- Keep weapon/viewmodel work separate from cutscene Hor+ projection research.
- If reference runtime evidence is unavailable, retain the findings as static/reference evidence and defer implementation.

## Expected final review

Perform read-only Git review after any tracked plan or research artifact changes. Update `backlog/TASKLOG.md` only after the reference comparison and any authorized manual validation are complete.
