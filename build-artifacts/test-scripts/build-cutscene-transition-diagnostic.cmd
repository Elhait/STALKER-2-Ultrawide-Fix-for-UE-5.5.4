@echo off
setlocal
pushd "%~dp0..\obj"

if not exist "..\..\external\safetyhook\safetyhook.cpp" goto :missing
if not exist "..\..\external\safetyhook\Zydis.c" goto :missing
if not exist "..\..\external\spdlog\include\spdlog\spdlog.h" goto :missing

call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail

cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /DCUTSCENE_TRANSITION_TRACE /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\cutscene_letterbox_fix.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2ExperimentalLetterboxTransitionTrace.asi"
if errorlevel 1 goto :fail

echo Build succeeded: %~dp0..\test-asi\STALKER2ExperimentalLetterboxTransitionTrace.asi
pause
popd
exit /b 0

:missing
echo ERROR: Missing SafetyHook, Zydis or spdlog dependency.
pause
popd
exit /b 1

:fail
echo ERROR: Transition diagnostic build failed.
pause
popd
exit /b 1
