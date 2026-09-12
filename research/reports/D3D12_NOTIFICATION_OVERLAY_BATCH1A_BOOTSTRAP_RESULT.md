# D3D12 Notification Overlay — COM-Safe Swapchain Interception Batch 1

## Scope

Implemented the first bounded ownership/provenance sub-batch:

- hook DXGI `CreateDXGIFactory`, `CreateDXGIFactory1` and `CreateDXGIFactory2`;
- attach a private per-instance `IDXGIFactory2` COM table using the exact
  25-entry interface ABI span;
- observe the bounded DXGI creation family:
  `CreateSwapChain`, `CreateSwapChainForHwnd`, `CreateSwapChainForCoreWindow`
  and `CreateSwapChainForComposition`;
- preserve the `CreateSwapChain*` creation argument and returned swapchain;
- query the creation argument as `ID3D12CommandQueue`;
- compare `queue->GetDevice()` with `swapchain->GetDevice()` using canonical
  COM identity;
- record HWND association, dimensions, format, buffer count, swap effect,
  flags and final `ACTIVE_GAME`/`AMBIGUOUS`/`NON_GAME` classification;
- fail closed for the overlay session without touching camera/FOV logic.

No ImGui, command-list submission, notification rendering, F9/F10 integration,
mouse capture, controller input, `winmm.dll` proxy or camera-core dependency is
included in this batch.

## Files

- `src/d3d12_notification_overlay_feasibility_204.cpp`
- `build-artifacts/test-scripts/build-d3d12-notification-overlay-feasibility-204.cmd`
- `build-artifacts/test-asi/STALKER2NotificationOverlayFeasibility.asi`

## Build validation

- Visual Studio 2022 x64 C++23 build: **PASS**
- Output size: 800,256 bytes
- Output SHA-256: `298F3455697F1D51E7648995CEA9EDD9EB108CD5B8275749733162DF3E98B8C3`
- Runtime: **NOT YET VALIDATED**
- Implementation: SafetyHook remains limited to DXGI export hooks; COM
  interception uses a private per-instance table and does not patch shared
  or read-only DXGI vtables.
- Batch 1.1: added exact requested-IID/returned-interface logging and explicit
  `IDXGIFactory2[25]` ABI logging to distinguish factory-path versus timing.
- The prior experimental build with composition slot `17` was invalid and is
  excluded from evidence. The current build restores the documented
  `IDXGIFactory2` composition slot `24`; relevant slots are
  `10=CreateSwapChain`, `15=CreateSwapChainForHwnd`,
  `16=CreateSwapChainForCoreWindow`, `24=CreateSwapChainForComposition`.

## Runtime evidence required

The new ASI must be loaded in the controlled game test. The required runtime
result is a forensic log showing the creation API, game HWND classification,
queue QI, queue-device identity, swapchain-device identity and final ownership
classification. Do not proceed to `Present`, resize or rendering in this task.

Historical runtime evidence from the rejected strategy is retained as context:
the prior direct `SafetyHook::VmtHook` path caused an access violation in
`dxgi`. That path is not used by this build.

The expected log is `STALKER2NotificationOverlayFeasibility.log` beside the
ASI. This build does not render anything yet; a successful result only proves
the ownership/lifecycle observation boundary needed before adding resources.

## Stop conditions

- Do not load this together with the production camera ASI during the first
  test unless the test setup explicitly verifies that the experimental ASI
  cannot affect the core.
- Stop on a crash, invalid queue/device provenance or repeated hook failure.
- A runtime `PASS` requires `ACTIVE_GAME` plus queue/swapchain device COM
  identity match; an arbitrary swapchain pointer is not sufficient.
- Do not add ImGui or render resources until this bootstrap boundary is
  runtime-confirmed.
