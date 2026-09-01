@echo off
setlocal
pushd "%~dp0..\obj"
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail

cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\cinematic_fov1405eda3a_runtime_correlation_204.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2CinematicFov1405EDA3ARuntimeCorrelation204.asi"
if errorlevel 1 goto :fail

echo Build succeeded: %~dp0..\test-asi\STALKER2CinematicFov1405EDA3ARuntimeCorrelation204.asi
popd
exit /b 0

:fail
echo ERROR: FUN_1405EDA3A runtime correlation build failed.
popd
exit /b 1
