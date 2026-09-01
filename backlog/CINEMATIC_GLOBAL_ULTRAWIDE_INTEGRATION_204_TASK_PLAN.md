# Global Ultrawide Experimental Integration 2.0.4 — Task Plan

## Objective

Produce one experimental ASI that reuses the validated stable gameplay aspect implementation and adds the validated cinematic one-shot correction after native FOV convergence.

## Established evidence and current state

- Stable gameplay aspect correction is runtime validated and must remain behaviorally unchanged.
- Cinematic ENTER convergence reaches FOV `90.0` in about one second.
- One combined correction to FOV `127.3927` and aspect `32:9` after convergence produces correct cinematic framing.
- Native EXIT returns FOV to the ENTER gameplay baseline and leaves aspect `32:9` with flags `0x04`.
- Repeated cinematic writes are not required.

## Approved scope

- New experimental global ASI only.
- Reuse `gameplay_aspect_fix.cpp` unchanged through a wrapper translation unit.
- Add separate cinematic ENTER/EXIT hooks and one-shot post-convergence diagnostic trigger.
- Keep separate global diagnostic logging.
- Keep the existing stable output and release source untouched.

## Explicit non-goals

- No modification of `STALKER2GameplayAspectFix.asi` source or release artifact.
- No native FOV transition RE in this batch.
- No PAGE_GUARD, VEH, manual assembly bridge, new broad tracer or legacy offset search.
- No production claim; the convergence detector remains diagnostic polling.
- No release packaging.

## Expected files or areas

- `src/global_ultrawide_experimental_204.cpp`
- `build-artifacts/test-scripts/build-global-ultrawide-experimental-204.cmd`
- experimental ASI output under `build-artifacts/test-asi`

## Batches and validation

### Batch 1 — integration wrapper

- Include the stable gameplay implementation without editing it.
- Add validated cinematic hooks and one-shot correction after stable FOV detection.
- Observe the same inner for three seconds after EXIT.

Validation: source review and compile-time consistency.

### Batch 2 — local build

- Build the separate experimental ASI.
- Verify stable source/output paths are not overwritten.

Validation: compiler success and artifact inspection only; runtime behavior remains unvalidated until a game run.

### Batch 3 — runtime handoff

- Run only the new global ASI.
- Test gameplay → cinematic → gameplay and a repeated cinematic in one session.
- Confirm stable gameplay transition logs and cinematic one-shot/EXIT logs.

Validation: user-supplied runtime logs and visual result; no assumption from build success.

## Risks and safe failure

- Any ENTER/EXIT signature mismatch must refuse cinematic setup safely.
- The stable gameplay resolver remains authoritative for gameplay and must fail closed as before.
- The cinematic component must perform at most one correction per captured inner and must not write after its EXIT observation expires.
- If either component cannot initialize, do not convert the failure into a production artifact.

## Stop conditions and phase gates

- Stop if the wrapper requires changing stable gameplay behavior.
- Stop if runtime shows hook conflict, stale state, FOV overshoot or broken re-arm.
- Do not merge into the stable ASI until repeated runtime regression passes are complete.

## Final review

Compare actual changed paths with this plan, review Git status/diff read-only, distinguish build validation from runtime evidence, and leave stable gameplay source/release files untouched.
