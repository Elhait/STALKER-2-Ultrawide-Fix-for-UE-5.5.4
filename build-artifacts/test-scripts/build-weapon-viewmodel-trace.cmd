@echo off
setlocal
pushd "%~dp0..\obj"

if not exist "..\..\external\safetyhook\safetyhook.cpp" (
    echo ERROR: Missing external\safetyhook\safetyhook.cpp
    pause
    popd
    exit /b 1
)
if not exist "..\..\external\spdlog\include\spdlog\spdlog.h" (
    echo ERROR: Missing external\spdlog\include\spdlog\spdlog.h
    pause
    popd
    exit /b 1
)

call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail

cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\weapon_viewmodel_trace.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2WeaponViewmodelTrace.asi"
if errorlevel 1 goto :fail

echo Build succeeded: %~dp0..\test-asi\STALKER2WeaponViewmodelTrace.asi
pause
popd
exit /b 0

:fail
echo ERROR: Weapon/viewmodel tracer build failed.
pause
popd
exit /b 1
