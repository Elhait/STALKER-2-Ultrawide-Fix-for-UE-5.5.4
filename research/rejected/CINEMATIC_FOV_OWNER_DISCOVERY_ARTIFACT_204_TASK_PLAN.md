# Cinematic FOV Owner Discovery Artifact 2.0.4 — Task Plan

## Objective

Design one bounded, read-only runtime discovery artifact that produces
execution-edge and owner-lineage evidence around the already validated
cinematic lifecycle:

`ENTER -> native convergence -> steady cinematic -> EXIT -> post-steady`.

The artifact must expose a small, analyzable set of execution boundaries rather
than scan or sample generic runtime traffic.

## Established evidence and current state

- Current executable SHA-256:
  `2ecc5d19fe37f97e3f7f2467d652b299b5a47f010fa49fd803a49a4a6930a409`.
- Validated cinematic lifecycle already identifies the authoritative camera
  context and `inner` at ENTER/EXIT.
- Native ENTER FOV converges `90.65574 -> 90.0`; native EXIT recovers the
  modified value to the ENTER baseline over approximately one second.
- Manual/global experimental evidence confirms the desired corrected cinematic
  state, but the native execution owner remains unknown.
- Existing static and runtime inventories did not expose an accepted owner.
- Previous random register sampling, PAGE_GUARD/VEH tracing, adjacent dual hooks,
  layout writer hunting and broad candidate scans are closed.

## Approved scope

- Define the exact observable before implementation: bounded execution edges,
  caller/target lineage, lifecycle phase, context identity and timing.
- Use only safe ordinary instrumentation surfaces that can be validated before
  installation; no page protection or exception-driven tracing.
- Arm only from the validated ENTER anchor and disarm at EXIT/post-steady.
- Correlate records with the captured authoritative `inner` or an explicitly
  recorded one-hop context relationship.
- Cap unique edges, records and capture duration; deduplicate by caller/target
  and phase.
- Keep the first artifact read-only and separate from all correction ASIs.

## Explicit non-goals

- No FOV/aspect writes, auto-F3, correction logic or production integration.
- No global experimental ASI or stable gameplay ASI changes.
- No random thread-context snapshots, generic tick tracing, PAGE_GUARD, VEH,
  hardware watchpoints or memory-wide scans.
- No legacy offsets, old signatures, direct `+0x230` writer enumeration or
  broad scalar-FP instruction search.
- No runtime artifact build until the observable and ordinary hook surfaces pass
  offline review.

## Expected files or areas

- This task plan.
- A future research-only source/build artifact, only after the design gate
  below passes.
- Existing validated ENTER/EXIT code and runtime log conventions as read-only
  references.

## Batches and validation

### Batch 1 — observable contract

Define the minimum record:

`phase, sampled thread, caller RVA, target RVA, return RVA when safely known,
context/inner identity, event timestamp relative to ENTER, hit count, bounded
register/argument summary`.

Validation: every field must have a safe provenance; no reconstruction from
volatile post-call registers or guessed stack layout.

Result:

- Observable fixed as lifecycle phase, sampled thread, ordinary boundary,
  caller/target/return when directly available, captured context identity,
  `inner` lineage, relative timestamp and bounded hit count.
- Context must be carried from validated ENTER; it must not be reconstructed
  from volatile post-call registers or guessed stack data.
- Owner acceptance requires correlation with both ENTER and EXIT windows.

### Batch 2 — anchor and filter design

- Use validated ENTER to capture context and open the window.
- Restrict candidates to explicitly selected ordinary execution surfaces.
- Require same-inner or documented one-hop lineage and phase timing.
- End capture at validated EXIT/post-steady or the hard duration cap.

Validation: installation must fail closed if any target bytes, function identity,
context or hook surface is ambiguous.

Result:

- Validated ENTER/EXIT hooks are suitable lifecycle gates and identity anchors.
- The generic dispatcher edge and gameplay writer are already closed branches.
- Existing evidence establishes no additional ordinary current-build execution
  surface. Reusing a closed surface would reopen prior work; selecting an
  uncorrelated function would violate the observable gate.
- No bounded observation surface is available for implementation.

### Batch 3 — implementation gate

Build the artifact only if Batches 1–2 identify a safe, bounded observation
surface. Keep it read-only and produce no correction state.

Validation: offline byte/signature checks, build verification and isolated
runtime launch only after the artifact passes review.

Result:

- Implementation gate not passed.
- No ASI was built, injected or launched.
- Discovery implementation is stopped until a new safe ordinary execution
  boundary is justified.

## Risks and safe failure

- Generic execution surfaces can produce high-volume noise; cap and reject them.
- Volatile ABI state can corrupt identity; carry context explicitly from the
  validated anchor.
- Any unsafe hook geometry, ambiguous target, invalid pointer or re-entrancy
  path must disable observation and perform no writes.
- If no ordinary observation surface can be selected without broad tracing, stop
  before implementation and report the design as unresolved.

## Stop conditions and phase gates

- Do not build a tracer before the observable and anchor design is approved by
  offline review.
- Do not exceed three selected observation surfaces in one artifact.
- Do not add a write path to improve signal quality.
- If the artifact cannot produce owner lineage without generic tick noise, close
  this phase as instrumentation-unresolved and stop.

## Final review

Compare the actual changed files with this plan, review Git status/diff, record
the exact executable identity, document whether the artifact was built or
runtime-tested, and keep all stable/global ASI files untouched.

Final review status:

- Completed: observable and lifecycle/filter design.
- Deferred: discovery implementation, runtime capture and owner verdict.
- Stable gameplay source, global experimental ASI and existing runtime
  artifacts were untouched.
- Any future artifact must use a newly justified ordinary boundary and must not
  reuse the closed dispatcher/gameplay-writer or unsafe instrumentation paths.
