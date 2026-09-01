@echo off
setlocal
pushd "%~dp0..\obj"

if not exist "..\..\external\safetyhook\safetyhook.cpp" goto :missing
if not exist "..\..\external\spdlog\include\spdlog\spdlog.h" goto :missing

call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail

cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\cutscene_fov_semantics_trace.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2CutsceneFovSemanticsTrace.asi"
if errorlevel 1 goto :fail

echo Build succeeded: %~dp0..\test-asi\STALKER2CutsceneFovSemanticsTrace.asi
pause
popd
exit /b 0

:missing
echo ERROR: Missing SafetyHook or spdlog dependency.
pause
popd
exit /b 1

:fail
echo ERROR: Cutscene FOV semantics tracer build failed.
pause
popd
exit /b 1
