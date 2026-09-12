@echo off
setlocal
pushd "%~dp0..\obj"
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail

cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\safetyhook ..\..\src\d3d12_notification_overlay_feasibility_204.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib dxgi.lib /OUT:"%~dp0..\test-asi\STALKER2NotificationOverlayFeasibility.asi"
if errorlevel 1 goto :fail
echo Build succeeded: %~dp0..\test-asi\STALKER2NotificationOverlayFeasibility.asi
popd
exit /b 0

:fail
echo ERROR: D3D12 notification overlay feasibility bootstrap build failed.
popd
exit /b 1
