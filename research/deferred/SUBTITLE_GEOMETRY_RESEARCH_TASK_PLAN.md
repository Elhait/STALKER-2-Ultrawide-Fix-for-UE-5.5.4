# Subtitle Geometry Research Task Plan

## Objective

Determine whether a safe C++/Slate research seam can obtain post-layout geometry for the live `SubtitleView` composition and the viewport on Steam 2.0.5 / UE 5.5.4, so horizontal centering can be calculated dynamically for arbitrary subtitle lengths.

## Established evidence and current state

- The issue reproduces without `STALKER2CameraTweaks` on Steam 2.0.5.
- UE4SS located one live `SubtitleView` instance.
- `SubtitleView::SetRenderTranslation()` moves the complete subtitle composition while leaving dialogue-choice UI unaffected.
- The composition width varies with speaker name, punctuation, wrapping and dialogue text.
- Child alignment, justification, width override and fixed offsets were rejected as production solutions.
- UE4SS Lua did not expose usable post-layout `FGeometry` values.

## Approved scope

- Inspect the existing C++/build structure and available UE/UE4SS-facing interfaces.
- Design and, if supported by the existing project contracts, implement a research-only geometry probe kept separate from production behavior.
- Obtain or classify viewport geometry and post-layout `SubtitleView` geometry.
- Report whether a dynamic correction formula is technically feasible.

## Explicit non-goals

- No changes to the production subtitle, camera, gameplay or cinematic paths.
- No fixed pixel offsets, resolution tables or heuristic centering.
- No changes to `STALKER2CameraTweaks.asi`, release assets or release documentation.
- No generic UObject parent traversal, recursive UE4SS dumping or unsafe reflection loops.
- No claim of a production fix unless reliable post-layout geometry is demonstrated.

## Expected files or areas

- Existing C++ source and build configuration only for bounded inspection.
- A separate research source/report under the research area only if required and supported by the current build contracts.
- This task plan and the corresponding research result archive.

## Batches

### Batch 1 — Contract and interface inspection

- Inspect current source includes, build flags and available runtime interfaces.
- Identify whether Slate/UMG geometry APIs are already linked or safely callable.

Validation: read-only source/build review; no binary or runtime changes.

### Batch 2 — Minimal research probe design

- Define the smallest safe probe for the live `SubtitleView` and viewport.
- Keep it isolated from production hooks and avoid generic UObject traversal.

Validation: compile-level review only if a research probe is necessary; production source remains untouched.

### Batch 3 — Runtime feasibility capture

- Run only if Batch 2 establishes a safe, bounded probe.
- Capture post-layout viewport and subtitle geometry at one or more resolutions.

Validation: user-run runtime evidence tied to Steam 2.0.5 and exact probe identity.

## Risks and rollback / safe-failure

- UE object lifetime, Slate thread affinity and private engine layout may make direct access unsafe.
- Any invalid pointer, unresolved symbol, unsupported API or crash risk stops the probe.
- Research code must be removable without touching the production build path.
- If geometry cannot be obtained reliably, reject the subtitle position fix rather than ship a heuristic.

## Stop conditions and phase gates

- Stop before implementation if no safe supported interface exists.
- Stop immediately on ambiguous ownership, unsupported ABI assumptions or contradictory executable identity.
- Do not proceed to runtime capture without a bounded probe and identity check.
- Do not promote any research result into production during this task.

## Expected final Git review

- Compare changed paths with this plan.
- Confirm production source, ASI, release assets and release docs are untouched.
- Classify the outcome as feasible, rejected or blocked, with validation limits.
- Archive this plan under `research/completed`, `research/rejected` or `research/deferred` when the pass ends.

## Batch 1 result

- The current ASI has no Unreal/Slate headers, UObject bridge, Slate API binding or viewport geometry interface.
- `build.cmd` links only `user32.lib` and `bcrypt.lib` in addition to the local hook/logging dependencies.
- Existing source is a native memory-hook ASI, not an Unreal UI integration layer.
- Adding a C++ Slate probe would require a new ABI/interface anchor that has not been established.

## Decision

Deferred at the interface-inspection gate. No research probe, production source, ASI, release asset or documentation was changed. The subtitle fix remains rejected unless a safe post-layout geometry interface is independently established.
