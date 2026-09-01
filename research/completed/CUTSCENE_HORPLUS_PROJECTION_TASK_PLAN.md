# Cutscene Hor+ Projection Research

Status: Closed — Batches 1B and 2 are complete; Batch 3 selected dynamic live-FOV conversion as the implementation architecture for game version 2.0.3.

## Objective

Identify the cinematic camera/projection state responsible for the remaining non-Hor+ FOV behavior after the A/B letterbox transition changes the cutscene render path from `16:9` to native `32:9`.

The task must determine whether the game already has an aspect/FOV input or projection rebuild path that can preserve correct cinematic composition at native ultrawide resolution.

## Established evidence and current state

- The current executable target is game version 2.0.3 with SHA-256 `81961B7281C7CF528CE49C549CE086FCC684BD676F32FAF042BC743D939E3C69`.
- At native display resolution `5120x1440`, vanilla cutscenes render at `2560x1440` with a `16:9` cinematic aspect.
- With the redesigned experimental A/B letterbox hooks, cutscenes render at `5120x1440` with native `32:9` output, confirmed through NVIDIA/DLSS resolution logging.
- The redesigned A/B setter-level hooks no longer crash in the tested cutscene flow and are not the target of this research task.
- The native-render result still has incorrect cinematic FOV/projection composition.
- After the cutscene, the stable gameplay ASI restores correct gameplay aspect behavior; this is a control observation, not evidence that gameplay replay should be used for cutscenes.
- The known gameplay camera context and tested ADS/accessor branches were rejected as the causal owner of the weapon/viewmodel FOV symptom and must not be reopened here without contradictory evidence.
- The 2.0.3 A/B dispatch table is structurally preserved across the analysed versions: A and B are referenced by adjacent data slots at `0x148EB9BE8` and `0x148EB9BF0`, while nearby setup points to `0x148EB9C00`.
- The dispatch-table references lead to confirmed object/camera setup owners, including constructors with the `Failed_to_find_UCameraComponent` diagnostic, but this establishes shared dispatch/object setup only; it does not establish cinematic projection or FOV ownership.
- A bounded 2.0.3 scan found many functions writing projection-like field families such as `+0x30`, `+0x40`, `+0x50` and `+0x5C`. These are generic candidates without cinematic context, event ownership or a demonstrated relation to the A/B aspect transition.
- The supplied BigChenga WIDEBOY v2 reference archive contains a separate `FOVFix` signature and code path labelled `Cutscene HOR+ FOV`. The signature has exactly one match in the current 2.0.3 executable at file offset `0x2EE0910`; this is a concrete cinematic-specific reference candidate, not yet independently confirmed as the causal owner.
- The reference script computes a Hor+ horizontal FOV from the live display aspect while preserving the native 16:9 vertical framing, and replaces the cinematic-entry FOV value after executing the original load. The archive contains only the FWS Lua configuration/script, not a runnable plugin binary, so this is reference evidence rather than load/runtime evidence.
- Independent Ghidra validation confirms the candidate at `0x142EE1510` (`RVA 0x2EE1510`) inside `FUN_142EE14BC`. The target decodes as `MOVSS XMM0,[0x149EDE50C]`, followed by the same call sequence represented by the reference signature. The corresponding 2.0.2 target is `0x142EE0510` (`RVA 0x2EE0510`).
- The referenced input global currently contains `80.0f` (`0x42A00000`) and has a separate writer at `0x1431018E0` in `FUN_1430FF56D`. This confirms the BigChenga path starts from a game-owned global cinematic baseline, but does not yet prove that `80.0f` is a per-scene FOV or that the writer is updated for every cinematic transition.

## Approved scope

- Research-only static and targeted runtime analysis of cinematic FOV/projection state.
- Compare the three control states: vanilla `16:9` cutscene, experimental native `32:9` cutscene, and corrected post-cutscene gameplay.
- Find aspect/FOV inputs, projection scale, camera transform, projection cache or cinematic camera-mode state that differs between these controls. Consider vertical FOV, horizontal FOV and projection-scale interpretation separately; do not assume the symptom requires a literal FOV field change.
- Trace the relevant state transition and identify candidate writers/consumers with evidence tied to executable version 2.0.3.
- Use the existing A/B letterbox hooks only as an unchanged experimental control; do not modify their implementation.
- Produce one or more candidate designs only after ownership and runtime correlation are established.

## Explicit non-goals

- No changes to `src/cutscene_letterbox_fix.cpp` or the A/B resolver/hook contract.
- No changes to stable gameplay aspect logic.
- No hard-coded FOV multiplier or guessed Hor+ compensation formula. A mathematically exact conversion of a live game-owned cinematic FOV may be considered only under Batch 3's framing and multi-FOV validation criteria.
- No forced replay of the gameplay hook, delayed timing workaround or pause/ADS simulation.
- No weapon/viewmodel FOV investigation unless new evidence directly identifies it as the cinematic projection owner.
- No broad renderer scan or arbitrary function-level tracer without a concrete runtime anchor.
- No release integration, compatibility claim or manual implementation build in this research phase.

## Expected files and areas

- `02-Research/Ghidra/ghidra-scripts/` for reusable static-analysis scripts.
- `02-Research/` for a concise evidence report if the research produces durable findings.
- `src/` only if a narrowly scoped diagnostic tracer is approved after static candidate narrowing; no feature implementation in this task.
- `build-artifacts/test-scripts/` and `build-artifacts/test-asi/` only for disposable diagnostic tooling if required by Batch 2.
- This plan file, kept updated as the phase changes.

## Batches

### Batch 1 — Static cinematic projection discovery

- Start from the established cinematic aspect transition and identify nearby or downstream projection/FOV consumers without changing A/B.
- Search for cinematic camera-mode, projection-cache, aspect-derived scale and viewport/render-target state.
- Independently inspect parallel cinematic projection paths rather than assuming they consume the gameplay camera writer.
- Classify candidates as `confirmed`, `likely`, `unresolved` or `rejected`, with a concrete reason for each rejection.

Validation: Ghidra 2.0.3 evidence, decoded instructions, callers/data-flow where available and explicit separation from 2.0.2 historical addresses. No source implementation and no runtime claim.

Batch 1 current result:

- `confirmed`: A/B dispatch structure and version correspondence only.
- `confirmed context`: generic camera/object dispatch setup associated with the nearby table owners; cinematic projection relevance remains unresolved.
- `unresolved`: any downstream cinematic projection consumer and the interpretation of the cinematic FOV/projection state.
- `rejected`: generic projection-like writers as causal candidates from field-family writes alone; no cinematic-specific evidence was established.
- `reference candidate`: BigChenga `FOVFix` signature/entry path; unique static match and plausible cinematic semantics, but ownership and runtime behavior remain unvalidated in our module.
- `confirmed structural candidate`: 2.0.3 entry `0x142EE1510` loads the game-owned `80.0f` global and continues into the cinematic camera call sequence; per-scene semantics and final projection ownership remain unresolved.
- Batch 1 phase gate is now satisfied only for the BigChenga-referenced FOV candidate. Batch 2 may proceed as a narrowly targeted validation of that candidate; do not build a tracer from the generic writer list.

### Batch 2 — Targeted runtime correlation

- Create a minimal read-only tracer for the validated BigChenga-referenced cinematic FOV entry path and its game-owned global writer.
- Resolve the entry signature dynamically and verify the decoded `MOVSS XMM0,[global]` followed by the expected call sequence before installing any hook.
- Resolve the global writer by validating the RIP-relative `MOVSS [global],XMM0` target; require a unique writer and roll back all hooks on any setup failure.
- Log only entry hit count, global FOV value, FOV observed at the post-load call boundary, writer value, caller and manual event markers. Do not alter XMM registers, global state or control flow.
- Mark and compare the required control states: vanilla `16:9` cutscene, experimental native `32:9` cutscene and corrected gameplay `32:9` state. A transient broken post-cutscene state may be recorded if observed, but it is not required for this research task and must not become a weapon/viewmodel-FOV detour.
- Log only the selected candidate fields/object context and event sequence; avoid broad memory or renderer dumps.
- Determine whether the candidate changes with `1.777778 → 3.555556`, whether FOV/projection state follows, and whether the corrected gameplay state shares a downstream effect.

Validation: build evidence plus user-run runtime evidence at `5120x1440`, with exact executable identity, event markers and log limits. A changed value is an anchor, not proof of ownership or causality.

Batch 2 implementation status:

- The read-only tracer built successfully as `build-artifacts/test-asi/STALKER2CutsceneFovSemanticsTrace.asi`.
- The tracer uses the current 2.0.3 entry signature and resolves the global writer by its RIP-relative target; it does not use fixed entry or writer RVAs for installation.
- Manual validation is pending. Do not treat the build as load or runtime evidence.

Batch 2 initial runtime result:

- The tracer loaded successfully in the tested 2.0.3 run and installed both targeted hooks.
- The global writer changed the value from `80.0` to `90.0`, then wrote `90.0` again; the cinematic-entry path subsequently observed `globalFov=90` and `XMM0=90`.
- This disproves treating `80.0f` as an immutable per-scene cinematic FOV. The game-owned global is runtime-updated and must be used as the FOV input if a Hor+ conversion is implemented.
- The current call-boundary caller read returned `0x0` and is not usable as ownership evidence. The initial run's marker capture was limited because markers were sampled only when one of the two hooks was hit.
- A vanilla-versus-experimental native-render comparison is required for Batch 2 closure and is recorded below.
- The tracer was revised so `F7`–`F11` markers are sampled by an independent lightweight polling thread rather than only inside hook callbacks. The rebuilt ASI completed successfully; this removes the previous marker-capture limitation for the next run.
- Marker semantics for the combined test are `F7=cutscene-active`, `F8=cutscene-ended`, `F9=weapon-fov-before-ads`, `F10=weapon-fov-after-ads`, `F11=pause-refresh`. `F9` and `F10` describe observed weapon-FOV states around the ADS refresh, not exact input timing.

Batch 2 control-state comparison:

- Vanilla 16:9 and experimental native 32:9 runs used the same 2.0.3 executable identity and both produced `80 → 90` global-writer events followed by `cinematic-entry globalFov=90 xmm0=90`.
- Both runs captured the expected cutscene and ADS/pause marker events; the revised marker names are recorded above.
- The cinematic-entry FOV input is therefore not the observed difference between vanilla 16:9 framing and experimental native 32:9 framing. The remaining incorrect composition is downstream of, or orthogonal to, this FOV input path; the A/B hooks change native render/aspect state but do not change the observed cinematic FOV.
- Batch 2 runtime semantics objective is complete. Vanilla 16:9 and experimental native 32:9 share the same cinematic-entry FOV input; Batch 3 must choose between a native downstream projection owner and dynamic conversion of that game-owned FOV while preserving cinematic vertical framing.
- A follow-up run captured two `pause-refresh` markers without any additional cinematic-entry or global-writer hits. Pause refresh therefore does not act through this investigated cinematic global FOV path; any weapon/viewmodel correction it causes belongs to a separate state/projection branch.

### Batch 3 — Candidate architecture decision

- This batch is a decision gate, not an invitation to start another broad research loop. No new broad scan and no implementation may begin until one of the two candidate designs below is selected or the task is stopped as blocked.
- If a cinematic projection owner is correlated across the required control states, document one or two minimal candidate designs.
- Prefer the native downstream projection owner only when ownership, aspect dependency and preservation of cinematic framing are all supported by evidence.
- If native ownership remains weak or unresolved after the bounded evidence review, select dynamic Hor+ conversion at the validated cinematic FOV path as the fallback architecture, using the actual game-owned FOV and live aspect rather than hard-coded `80/90` or a fixed multiplier.
- Prefer an existing game-owned projection/aspect mechanism when one can be established. If the engine does not expose a native Hor+ rebuild at this stage, a mathematically exact conversion of the live game-owned cinematic FOV using the live aspect may be considered, provided it preserves vertical framing and is validated across multiple cinematic FOV values.
- Batch 3 may compare two candidate designs: (1) a native downstream projection owner that consumes the cinematic aspect state, or (2) dynamic Hor+ conversion at the validated cinematic FOV entry using the actual game-owned FOV and live display aspect. Neither design may use a hard-coded base FOV or fixed multiplier.
- Require any candidate design to preserve the cinematic framing intent: Hor+ should expand horizontal visibility without changing the intended vertical composition or introducing an unrelated camera/FOV shift.
- If dynamic entry conversion is considered, validate it against multiple game-owned cinematic FOV values and use the live value as input; `80.0f` or `90.0f` must not become an implementation constant.
- If neither a validated native downstream owner nor a safely validated dynamic live-FOV conversion path can be established, stop with a blocker and do not implement a workaround.

Batch 3 decision:

- No native downstream projection owner was established by the bounded static/runtime evidence. Generic projection-like writers remain rejected or unresolved and are not implementation inputs.
- Selected architecture: dynamic Hor+ conversion at the validated cinematic FOV path, using the actual game-owned cinematic FOV value at the post-load entry boundary and the live display aspect. The conversion must preserve the native 16:9 vertical framing and must be validated across multiple runtime cinematic FOV values.
- The selected architecture is a decision outcome, not runtime proof of the final implementation. Source changes require the separate `CUTSCENE_HORPLUS_IMPLEMENTATION_TASK_PLAN.md`.

Validation: evidence review against this plan. No feature implementation unless a separate implementation plan is approved.

## Risks and safe failure

- Do not modify the known-good native-render A/B hooks during this research.
- Refuse to promote a candidate from static naming, one runtime event or one changed value alone.
- Stop if the candidate depends on an unvalidated object lifetime, thread context or projection matrix ownership.
- Stop if the only apparent solution is a hard-coded multiplier, gameplay replay or timing workaround.
- Keep the stable gameplay ASI independently usable throughout testing.

## Stop conditions and phase gates

- Batch 2 is gated until Batch 1 produces at least one cinematic projection-specific structural anchor.
- The current generic projection-like writer scan does not satisfy that gate. A further static pass must start from a new cinematic-specific anchor or stop with a documented blocker; it must not become another broad renderer scan.
- Do not reopen rejected gameplay-camera or IA/accessor branches without new contradictory evidence.
- Do not modify A/B hooks or claim Hor+ compatibility during research.
- If no cinematic owner can be established after a bounded pass, record the blocker and defer implementation.
- Final Git review and `backlog/TASKLOG.md` update occur only after the scoped research result and any authorized manual validation are complete.

## Expected final review

Inspect Git status and the relevant diff, compare changed paths with this plan, and classify the outcome as completed, remaining, deferred, blocked and not-runtime-validated. Keep the existing letterbox task's native-render evidence separate from this unresolved Hor+ projection research.
