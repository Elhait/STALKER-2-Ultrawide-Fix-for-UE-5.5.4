@echo off
setlocal
pushd "%~dp0..\obj"

if not exist "..\..\external\safetyhook\safetyhook.cpp" (
    echo ERROR: Missing external\safetyhook\safetyhook.cpp
    echo Place the SafetyHook dependency under external\safetyhook.
    set "BUILD_ERROR=1"
    goto :fail
)
if not exist "..\..\external\safetyhook\Zydis.c" (
    echo ERROR: Missing external\safetyhook\Zydis.c
    set "BUILD_ERROR=1"
    goto :fail
)
if not exist "..\..\external\spdlog\include\spdlog\spdlog.h" (
    echo ERROR: Missing external\spdlog\include\spdlog\spdlog.h
    echo Place the spdlog dependency under external\spdlog.
    set "BUILD_ERROR=1"
    goto :fail
)

call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 (
    echo ERROR: Visual Studio Developer Command Prompt could not be initialized.
    set "BUILD_ERROR=1"
    goto :fail
)

where cl.exe >nul 2>&1
if errorlevel 1 (
    echo ERROR: cl.exe is not available in the current environment.
    set "BUILD_ERROR=1"
    goto :fail
)

cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\cutscene_letterbox_fix.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2ExperimentalLetterboxFix.asi"
if errorlevel 1 (
    echo ERROR: Letterbox build failed.
    set "BUILD_ERROR=1"
    goto :fail
)

echo Build succeeded: %~dp0..\test-asi\STALKER2ExperimentalLetterboxFix.asi
pause
popd
exit /b 0

:fail
pause
popd
exit /b 1
