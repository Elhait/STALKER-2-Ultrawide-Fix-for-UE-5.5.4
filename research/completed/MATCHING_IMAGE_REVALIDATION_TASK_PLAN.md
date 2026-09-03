# Matching-Image 2.0.4 Revalidation — Task Plan

Status: Completed — current-image production safety and bounded P1/P2 revalidation

## Objective

Revalidate the production safety contracts and the highest-improvement-potential
cinematic branches against the canonical Ghidra `2.0.4` image. Determine
whether any previously rejected `P1/P2` branch provides a concrete native or
simpler mechanism worth a separate implementation task.

## Established evidence and current state

- Runtime executable SHA-256:
  `2ECC5D19FE37F97E3F7F2467D652B299B5A47F010FA49FD803A49A4A6930A409`.
- Expected `.text` size: `130818560`.
- Expected image base: `0x140000000`.
- Canonical Ghidra project: `Dump/STALKER2-Ghidra`.
- Known runtime anchors: ADS IN `0x6ABE7E`, ADS OUT `0x6AC01B`, primitive
  setter runtime RVA `0x5665FA6`.
- The preceding blast-radius audit found stale static evidence and explicit
  `2.0.3` branch-closing claims that cannot be used against `2.0.4` without
  matching-image revalidation.

## Approved scope

1. Establish and record the matching-image identity gate before interpretation.
2. Revalidate production-facing static contracts only:
   gameplay writer, cinematic aspect store, ENTER/EXIT FOV boundaries, shared
   consumer contract, signature uniqueness, instruction decoding and fail-closed
   guards.
3. Add `P1–P5` improvement-potential ranking to the revalidation record.
4. Revalidate the highest-potential cinematic branches:
   downstream projection consumer, camera aspect refresh, FOV downstream state,
   transient/durable FOV split and constrained/native projection state.
5. Compare native ownership, post-cinematic seam behavior, coordinator/state
   complexity and intervention surface.
6. Produce one bounded revalidation report and one final task-log entry after
   the approved analysis and review are complete.

## Explicit non-goals

- Do not modify production C++/ASI/INI logic.
- Do not improve or refactor a production contract during the safety pass.
- Do not create or run a new runtime tracer in this task.
- Do not launch the game or claim new runtime behavior.
- Do not expand into weapon/viewmodel research.
- Do not silently rewrite or delete historical research files.
- Do not promote a branch to implementation without concrete matching-image
  evidence of a material advantage.

## Files and areas expected to be touched

- This task plan.
- A new revalidation report under `02-Research/Ghidra/reports`.
- Existing matching-image Ghidra scripts only if a bounded static query needs
  correction; no production source.
- `backlog/TASKLOG.md` once, after final review and plan comparison.

## Batches

### Batch 1 — Matching-image identity gate — completed

Verify SHA-256, `.text` size, image base/section layout and actual ADS anchor
locations in `Dump/STALKER2-Ghidra`. Record an explicit `Identity: PASS`.

Validation: any mismatch in the first three identity values stops the task;
anchor mismatch stops interpretation until resolver semantics are reconciled.

Result: `Identity: PASS` is established by the fresh current-image identity
probe. The initial headless attempt was blocked by a project lock, which was
then traced to the agent's own leftover headless Java/Ghidra process, not a
user GUI session. The agent-owned process was terminated and its stale lock
removed. No foreign/user Ghidra process was terminated and no scratch project
was created.

### Batch 2 — Production safety contracts — completed

Classify each contract as `PASS`, `FAIL` or `REVALIDATE` without changing it.
Record decoded instruction, uniqueness, ownership evidence and fail-closed
behavior where available.

Validation: matching-image static evidence only; no runtime implementation
changes. A successful static match is not runtime proof.

Result: source and existing current-build evidence provisionally classify the
gameplay writer, instruction/decode contract, ambiguity refusal, cinematic
aspect store, ENTER/EXIT boundaries, Hor+ policy and recovery fail-closed
behavior as `PASS` within the tested 2.0.4 contract. Fresh matching-image
reconciliation is required before this becomes the final status. The shared
downstream projection consumer remains `REVALIDATE`.

### Batch 3 — Improvement-potential ranking — completed

Rank relevant branches using:

- `P1` — may replace or simplify the production mechanism;
- `P2` — may remove a known visual/state issue;
- `P3` — may improve resolver/ownership robustness;
- `P4` — restores the technical model only;
- `P5` — historical/documentation only.

Validation: every P1/P2 ranking must state the concrete expected advantage and
the evidence supporting or rejecting it.

Result: P1/P2 ranking and promotion gates are recorded in
`02-Research/Ghidra/reports/matching-image-revalidation-2026-09-02.md`.

### Batch 4 — P1/P2 cinematic revalidation — completed

Recheck the bounded rejected branches on the matching image. Preserve negative
results, reopen only when current evidence supports it, and do not implement
any candidate in this task.

Validation: report original claim, current identity, current result, branch
consequence and whether a separate implementation task is justified.

The previous lock blocker was agent-owned and has been cleared. Fresh
matching-image reanalysis completed on the canonical `2.0.4` entry. The exact
current ENTER setter was confirmed at `RVA 0x6B7CB05`; its local data-flow has
no FOV/projection owner. The bounded camera-field intersection produced 516
generic functions and no causal projection candidate. Existing current-build
reviews of the downstream projection, refresh, transient/durable, constrained/
native and two-pass branches produced no concrete P1/P2 advantage. Old 2.0.3
results remain historical and are not promoted as current 2.0.4 evidence.

Result: no P1/P2 branch was reopened or promoted; production source remains
unchanged.

### Batch 5 — Final review and task-log record — completed

Reviewed changed paths, identity headers, stale-image provenance and the queue
generated by the report. Added one factual `TASKLOG.md` entry after comparing
the result with this plan.

## Risks and safe-failure behavior

- If identity is not `PASS`, stop all static interpretation.
- If ownership or cross-version mapping remains uncertain, mark it
  `REVALIDATE`/`UNKNOWN`; do not infer a native mechanism.
- If a P1/P2 branch lacks concrete advantage, keep it historical and do not
  reopen production work.
- Runtime behavior remains unchanged because production files are out of scope.

## Stop conditions and phase gates

- Identity gate failure: stop immediately.
- Production contract failure or contradiction: record it and stop before
  improvement ranking unless the failure is purely documentary.
- No matching-image evidence of concrete P1/P2 advantage: keep current
  implementation unchanged and close this bounded analysis.
- Stop after the revalidation report and one final task-log entry; further
  implementation or runtime tracing requires a separate approved task.

## Expected final Git review

Confirm that only the task plan, revalidation report and the single factual
task-log entry changed. Confirm no production ASI/source, runtime tracer or
historical research conclusion was silently rewritten.
