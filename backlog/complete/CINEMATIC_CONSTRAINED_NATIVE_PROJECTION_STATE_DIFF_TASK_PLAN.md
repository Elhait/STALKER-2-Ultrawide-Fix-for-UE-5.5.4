# Cinematic Constrained-vs-Native Projection State Diff — Task Plan

Status: Blocked/Closed — Batch 1 and the single approved Batch 2 ownership check completed without a validated cinematic projection anchor. No runtime tracer or source implementation was authorized.

## Objective

Use the current Ghidra 2.0.3 project to identify the smallest camera/projection state difference between the two confirmed cinematic outcomes:

- constrained `16:9` / `2560x1440`, where cinematic framing is visually correct;
- native `32:9` / `5120x1440`, where rendering is native but cinematic FOV/framing is incorrect.

The target is a concrete state field, derived projection value or game-owned function that explains why correct FOV is coupled to the constrained render state.

## Established evidence and current state

- Tested executable is game version 2.0.3 with SHA-256 `81961B7281C7CF528CE49C549CE086FCC684BD676F32FAF042BC743D939E3C69`.
- Cinematic A/B object and stable gameplay source were runtime-confirmed as the same object in multiple runs.
- The gameplay two-pass can be triggered at the validated camera-writer boundary on that object.
- The two-pass produces correct-looking cinematic FOV only while the camera is in constrained `16:9` state.
- Restoring only `+0x254` to native `32:9` restores the wide render but not the correct cinematic FOV.
- `state+0x54` receives transformed cinematic FOV in the earlier Hor+ path, but its active downstream blend/projection ownership was not established and that branch is closed as a causal owner.
- The current Ghidra project is durable at `E:\Work\Slaker2 mods\Dump\STALKER2-Ghidra`; analysis/runtime user state must remain on `E:`.

### Batch 1 findings (2.0.3)

- `FUN_14010C076` is an object constructor/default initializer. It initializes `+0x254`, `+0x259` and `+0x262`, but does not establish their cinematic projection semantics.
- `FUN_1401BD890` reads mode/flag bytes including `+0x259` and `+0x262` and packs them into renderer/state bitfields. It does not read the runtime aspect field `+0x254`; ownership is therefore incomplete.
- `FUN_140280F58` reads `+0x254/+0x259` as inputs to a graph/lighting-style task and is rejected as a camera projection owner.
- `FUN_140311CB4` and `FUN_14038E760` are scene/lighting or resource-generation paths; their matching offsets are false positives for this task.
- `FUN_140186BE8` is the strongest remaining candidate: it computes and stores multiple 4x4 matrix values, uses viewport-like integer parameters and derived scale values, and writes matrix-related fields around `+0x230..+0x25C`. Its callers pass separate render/scene structures, so cinematic ownership and relation to the shared camera object remain unproven.
- Read-only caller inspection of `FUN_140186BE8` showed repeated calls from render/scene orchestration functions; no direct reference to the validated cinematic object or `+0x254` camera-aspect state was established.

## Approved scope

- Inspect the 2.0.3 Ghidra program and existing research artifacts.
- Start from the shared camera object and its confirmed `+0x254/+0x259` state family.
- Compare readers/writers and nearby state fields associated with constrained/native transitions.
- Follow only concrete projection-relevant data flow: FOV semantics, tan-half-FOV or equivalent scale, view rectangle, aspect-dependent X/Y scale, projection mode and temporary view/projection structures.
- Use the two runtime control states as the interpretation target, not as permission to add new writes.
- Record exact executable identity, addresses/RVAs, decoded instructions, callers/callees and confidence for every promoted candidate.

## Explicit non-goals

- Do not modify stable gameplay behavior or the accepted A/B letterbox contract.
- Do not reopen `state+0x54` as a blend owner without contradictory evidence.
- Do not perform a broad global matrix/helper scan or generic FOV string search.
- Do not create a runtime tracer, source hook or diagnostic build before a concrete state/projection anchor is established.
- Do not infer field semantics from matching offsets alone.
- Do not use timers, frame delays, hard-coded FOV multipliers or guessed aspect writes.

## Expected files and areas

- `Dump/STALKER2-Ghidra` — current Ghidra project, read-only analysis target.
- `02-Research/Ghidra/ghidra-scripts` — reusable scripts only if a bounded query is required.
- `02-Research` reports/notes — bounded findings if a durable report is needed.
- This task plan — status, evidence and phase results.

## Batches

### Batch 1 — Shared object and state-family diff

- Confirm the shared object layout and known `+0x254/+0x259` access paths in the 2.0.3 program.
- Identify nearby fields and methods that differ between constrained and native state transitions.
- Trace the relevant callers/callees without promoting generic camera helpers.

Validation: static decode/decompile review tied to the 2.0.3 program and exact RVAs; no source changes. Result: bounded state-family plumbing confirmed; no companion projection state yet promoted.

### Batch 2 — Projection-state candidate validation

- For each bounded candidate, prove whether it affects a temporary projection/view structure or a durable camera state.
- Determine whether it carries constrained/native aspect, FOV-derived scale or projection mode into the render path.
- Reject candidates that provide only correlation, setup or generic container behavior.

Result: `FUN_140186BE8` was rejected. Its callers and surrounding data flow are tied to movable-point-light/shadow and `ViewSpace...` scene-resource processing, not the validated cinematic camera object or its aspect/FOV state. No further matrix builder was opened.

Validation: cross-reference/data-flow evidence and an explicit ownership classification. Runtime tracing remains gated.

### Batch 3 — Architecture decision

- Promote only a candidate with concrete ownership and a safe, cinematic-limited intervention boundary.
- If no such candidate is established, close the task as blocked and preserve the negative result; do not implement a guessed workaround.

Result: blocked. No native cinematic projection-state difference or safe intervention boundary was established from this bounded pass.

Validation: plan comparison and read-only Git review. Any later runtime test or implementation requires a separate approved plan.

## Risks and rollback or safe failure

- Similar camera fields may belong to different state objects or lifetimes.
- Decompiled structures may be incomplete or misleading without constructor/destructor and caller evidence.
- A projection helper may be shared by gameplay and cinematic paths; shared use is not sufficient without a cinematic discriminator.
- Static ambiguity, unresolved indirect calls or missing executable identity must produce an unresolved candidate, not a hook.
- This research task is read-only for the executable/project; rollback is deletion of only newly created notes/scripts within an explicitly approved cleanup scope.

## Stop conditions and phase gates

- Stop Batch 1 if the shared object/type cannot be established from bounded evidence.
- Stop if the only findings are generic renderer/matrix helpers, string matches or offset similarity.
- Do not open runtime tracing until a concrete candidate has ownership, state/context and projection relevance.
- Close as blocked if neither a native projection-state difference nor a safe game-owned refresh/projection boundary can be established.

## Final review

- Perform read-only Git review after each changed research artifact batch.
- Compare actual changed paths with this plan and separate completed, remaining, deferred, blocked and not-runtime-validated results.
- Update `backlog/TASKLOG.md` only after the bounded research phase is completed or formally blocked and reviewed.
