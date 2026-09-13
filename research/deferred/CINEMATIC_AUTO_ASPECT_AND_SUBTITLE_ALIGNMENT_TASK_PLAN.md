# Cinematic Auto Aspect and Subtitle Alignment — Task Plan

Status: `BATCH 1 PASS / BATCH 2 PASS / HOTFIX PACKAGED — SUBTITLE TRACK DEFERRED`.

## Objective

Investigate and correct two user-reported issues on ultrawide displays:

1. `Cinematics.AspectRatio=Auto` may capture an incorrect runtime aspect
   (`1.66667` reported on 21:9) before the first in-game cinematic, causing the
   first rendered cinematic to use the wrong framing. A later pre-rendered 16:9
   cinematic is also reported in the same sequence.
2. Cinematic subtitles are reported slightly left of screen center.

The two symptoms must be treated as separate hypotheses until runtime evidence
proves a shared owner.

## Established evidence and current state

- The user reports that on 21:9, `Auto` does not detect the expected display
  aspect and logs `1.66667`; manually forcing `AspectRatio=21:9` works.
- Current Auto implementation in
  `src/experimental_cinematic_21_9_combined_fix_204.cpp` caches the gameplay
  camera source `+0x254` in `g_lastObservedAspect` and resolves cinematic Auto
  from that cache when the cinematic aspect store is reached.
- The current code explicitly avoids treating its own native restore as a new
  runtime aspect, so stale/intermediate source ownership is a plausible cause.
- No subtitle layout owner or runtime subtitle coordinate evidence is
  currently established.
- Stable Full Hor+ behavior and production source remain unchanged until a
  bounded evidence phase establishes the correct owner and regression scope.

## Approved scope

- Verify the exact Auto aspect source and timing around first cinematic ENTER.
- Compare cached camera aspect with display, window/client and engine/render
  resolution observations where available.
- Determine whether the wrong value is stale, intermediate, or an actual game
  camera aspect that should not be used for cinematic Auto.
- Separately identify whether subtitle displacement is caused by cinematic
  framing, a UI safe-zone/anchor, subtitle canvas geometry, or another owner.
- Add only diagnostic/read-only instrumentation until both ownership questions
  are established.

## Explicit non-goals

- No immediate hard-coded `21:9` replacement for Auto.
- No changes to the stable Full Hor+ gameplay path.
- No subtitle coordinate writes or UI hooks before ownership evidence.
- No assumption that the subtitle issue shares the camera aspect owner.
- No renderer/matrix search and no broad UI scan.
- No Nexus update or stable production version bump in this task. A separate
  hotfix archive is allowed after runtime validation.

## Files or areas expected to be touched

- This task plan.
- `src/experimental_cinematic_21_9_combined_fix_204.cpp` only if a separate
  diagnostic build is required; production behavior is out of scope for Batch
  1.
- New research-only diagnostic source/build script if needed.
- One or more reports under `research/reports`.

## Batches

### Batch 1 — Auto aspect source and timing audit

Status: `COMPLETE / PASS`.

- Confirm the current 2.0.4 identity and existing cinematic aspect/FOV anchors.
- Trace/log, read-only, the value used by Auto at gameplay writer, cinematic
  ENTER, cinematic aspect store and EXIT.
- Record display resolution, client resolution, cached source aspect and the
  actual object/source pointer for each observation.
- Determine whether `1.66667` is already present before ENTER or is introduced
  by the cinematic store timing.

Pass condition: a reproducible source/timing explanation for the incorrect Auto
value.

Result: the log confirms that Auto observes `2.38889`, then the fix's own
native `1.77778` restore is cached before cinematic ENTER. See
`research/reports/CINEMATIC_AUTO_ASPECT_BATCH1_LOG_RESULT.md`.

Stop condition: if the source cannot be tied to a validated current-build
object/lifecycle boundary, stop before changing Auto behavior.

### Batch 2 — Auto correction feasibility

Status: `COMPLETE / PASS`.

- Test only the smallest reversible correction supported by Batch 1 evidence:
  prevent the fix-owned native restore from replacing the authoritative Auto
  aspect, using a lifecycle-specific cache rule.
- Compare first in-game cinematic, pre-rendered 16:9 cinematic and EXIT.
- Preserve existing FOV and two-pass lifecycle contracts.

Validation: diagnostic log plus user-provided screenshots or reproducible
runtime observations. No production merge without regression evidence.

Implementation: `src/experimental_cinematic_21_9_combined_fix_204.cpp`.
Build artifact: `build-artifacts/test-asi/STALKER2CameraTweaks.asi`.
Patch report: `research/reports/CINEMATIC_AUTO_ASPECT_BATCH2_PATCH_RESULT.md`.

Runtime result: Auto used `2.38889` on 21:9 and `3.55556` on 32:9; repeated
cinematic ENTER/EXIT and gameplay recovery passed on both tested ratios.

### Batch 2A — Hotfix packaging

Status: `COMPLETE / PASS`.

- Package the validated ASI as `v0.5.1` without changing the stable
  production release assets or source version.
- Include only the ASI, default INI, README, license and third-party notices.
- Verify archive contents and ASI identity hash.

Artifact: `release-assets/STALKER2CameraTweaks-UE5.5.4-v0.5.1.zip`.

### Batch 3 — Subtitle alignment ownership

Status: `COMPLETE / DEFERRED — no production owner established`.

- Establish whether subtitle displacement tracks the cinematic viewport/frame,
  a UI safe-zone, or an independent subtitle canvas anchor.
- Use screenshot comparison and, only if a safe boundary exists, read-only
  observation of subtitle layout inputs.
- Stop if evidence requires a broad Slate/UI renderer scan.

Result: Steam 2.0.5 vanilla reproduction was confirmed at 2560x1440 and
5120x1440. UE4SS identified `SubtitleView`, and root translation moved the
complete subtitle composition without moving dialogue choices. Child alignment
and fixed offsets were rejected. Lua did not expose usable post-layout
geometry. The bounded C++/Slate Batch 1 inspection found no safe existing
geometry interface or ABI anchor, so no probe or production change was made.

### Batch 4 — Decision

Status: `COMPLETE — Auto track passed; subtitle track deferred/rejected`.

- Promote only a reproducible Auto source fix and/or independently proven
  subtitle owner.
- Keep the two fixes separate unless a shared lifecycle/UI owner is proven.
- If ownership remains unresolved, record a bounded negative/deferred result and
  leave current production behavior unchanged.

Decision: the Auto aspect track remains passed and packaged as the hotfix. The
subtitle position fix is deferred/rejected until reliable post-layout
`FGeometry` and viewport geometry can be obtained through an independently
established safe interface. Production behavior remains unchanged.

## Risks and rollback / safe failure

- Replacing the cached aspect with display aspect could break windowed,
  resolution-change and letterbox behavior.
- Delaying aspect capture could miss the correct gameplay source or alter the
  existing two-pass transition.
- Subtitle displacement may be a UI-specific safe-zone issue unrelated to
  camera aspect.
- Diagnostics must fail closed on identity or pointer validation failure.
- No runtime writes are permitted during ownership discovery.

## Stop conditions and phase gates

- Stop on identity mismatch or unknown anchor semantics.
- Stop before Auto changes if the source/timing explanation is not reproducible.
- Stop before subtitle changes without a concrete UI ownership boundary.
- Stop before broad renderer/UI scans.
- Stop before release/build integration until source behavior and runtime
  regression evidence are reviewed.

## Expected final review

- Compare actual changed paths with this plan.
- Confirm stable gameplay/cinematic production behavior was not altered during
  diagnostics.
- Review only relevant research artifacts and diagnostic outputs.
- Do not stage, commit, reset, checkout or otherwise modify Git state.
