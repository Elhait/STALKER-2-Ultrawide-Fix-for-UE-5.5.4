# Panini Render-Pass Artifact Isolation — Result

Status: Bounded isolation pass complete — no tested CVar isolated the
artifact; responsible rendering contribution remains unknown.

## Fixed control

- Same 32:9 cinematic scene.
- `r.LensDistortion.Panini.D 0.25` kept active while testing each candidate.
- One candidate changed at a time and restored after comparison.

## User-run results

| Candidate | Result |
|---|---|
| `r.ContactShadows 0` | No visible change |
| `r.TemporalAA.Upsampling 0` | No visible change |
| `r.Lumen.ScreenProbeGather.ScreenTraces 0` | No visible change |
| `r.Lumen.Reflections.ScreenTraces 0` | No visible change |

The previously completed Panini tuning tests also remain negative for artifact
removal: positive `D` values reproduce the artifact, `S` does not remove it,
`r.LensDistortion.Panini.ScreenFit` from `0` to `10` has no observable effect,
and `r.Shadow.Denoiser 0` does not change it.

## Classification

```yaml
Panini availability: PASS
Panini remap execution: PASS
Contact shadows: NOT CAUSAL IN TESTED SCENE
Temporal upsampling: NOT CAUSAL IN TESTED SCENE
Lumen GI screen traces: NOT CAUSAL IN TESTED SCENE
Lumen reflection screen traces: NOT CAUSAL IN TESTED SCENE
Shadow denoiser: NOT CAUSAL IN TESTED SCENE
CVar-only isolation: EXHAUSTED
Root cause: UNKNOWN
Production suitability: BLOCKED
```

These tests do not identify the offending pass. They only show that the
artifact survives the tested shadow, temporal, Lumen GI and Lumen reflection
toggles. The auxiliary-buffer/screen-space mismatch remains a hypothesis, not
a confirmed root cause.

## Decision

Close this bounded CVar isolation pass without further blind renderer-CVar
tuning. Keep the current Full Hor+ path as the production baseline. Any future
progress requires a concrete lower-level render-pass observation, engine-side
anchor or a separately approved instrumentation task.

No production source, ASI, INI, build output or release asset was changed.

