# Panini CVar Runtime Feasibility — Result

Status: Bounded CVar tuning complete — Panini is available and active, but
the current path has a persistent rendering artifact and is not production
suitable without further render-pass investigation.

## User-run matrix

The same 32:9 cinematic scene was used with the console-enabler and the
existing mod build.

- `r.LensDistortion.Panini.D 0`: baseline; duplicate shadow artifact absent.
- Positive `D` values, including `0.25`, `0.5` and `1.0`: Panini visibly
  changes the image, but the detached/duplicated tree-like shadow artifact is
  present at every tested positive strength.
- `r.LensDistortion.Panini.S 0`, `0.25`, `0.5` and `1`: artifact persists.
- `r.LensDistortion.Panini.ScreenFit 0–10`: no visible change and artifact
  persists.
- `r.Shadow.Denoiser 0`: no change to the artifact in the tested scene.

## Evidence classification

```yaml
CVar availability: PASS
Panini execution: PASS
Visible remap: PASS
Panini.D tuning: FAIL to remove artifact
Panini.S tuning: FAIL to remove artifact
Panini.ScreenFit tuning: no observable effect
Shadow.Denoiser test: not causal in tested scene
CVar-only solution: EXHAUSTED
Production suitability: BLOCKED
```

The result establishes a persistent rendering compatibility issue, not the
identity of the offending pass. The most likely class is a contribution or
auxiliary buffer that is not spatially transformed with the Panini-processed
scene color, but shadow, temporal, depth, normal and other screen-space paths
remain hypotheses until isolated.

## External context and limits

Epic documents Panini as an optional post-processing pass in the upscaling
path, with possible center blur and top/bottom fitting effects. Epic issue
`UE-368981` describes inconsistent Panini treatment of SceneDepth,
CustomDepth, WorldNormal and related buffers, but the issue is listed for UE
5.6/5.7. It is therefore a strong architectural analogue, not direct proof
of the STALKER 2 UE 5.5.4 root cause.

## Next gate

Do not continue blind CVar tuning. A separate `PANINI_RENDER_PASS_ARTIFACT_
ISOLATION` task is required before any renderer or ASI work. It should use a
fixed scene and `D=0.25`, change one rendering contribution at a time, and
remain read-only with no production changes.

