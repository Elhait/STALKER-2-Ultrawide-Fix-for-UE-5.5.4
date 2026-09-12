# Existing Active Swapchain Ownership — Gate A Inventory

Status: `BLOCKED / NO INDEPENDENT ACQUISITION ANCHOR`.

## Scope

Read-only inventory for one question:

> What concrete current-build or documented runtime source can yield an
> already-existing `IDXGISwapChain*` without factory expansion, `Present`
> observation, COM-vtable modification, broad renderer scanning or guessed
> global pointers?

## Findings

- The workspace contains the previously tested DXGI factory export and
  per-instance `IDXGIFactory2` interception experiment.
- The corrected factory path observed `IDXGIFactory4` creation and a queried
  `IDXGIFactory2` interface, but no `CreateSwapChain*` callback.
- No separate current-build engine/window ownership chain retaining an
  existing `IDXGISwapChain*` was found in the available source/research files.
- No documented OS/DXGI API that enumerates or returns the game's existing
  swapchain was identified in the bounded inventory.
- No independently proven retained COM-object bridge is available.

## Classification

```yaml
Gate A acquisition anchor: NOT FOUND
Gate B ownership proof: NOT REACHED
Factory creation path: already exhausted in separate branch
Renderer implementation: not started
Production camera/FOV source: untouched
```

This is not evidence that an existing swapchain cannot be acquired by any
means. It is only a bounded negative for the currently permitted evidence
classes and workspace anchors.

## Stop condition

Do not add another factory hook, `Present` hook, COM-vtable strategy, global
pointer scan or broad renderer search under this task. Resume only when a new
independent acquisition anchor is supplied, such as a current-build
engine/window ownership chain, documented runtime ownership API or proven
retained COM-object bridge.
