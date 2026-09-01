@echo off
setlocal
pushd "%~dp0..\obj"
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail
cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\cinematic_thread_sample_discovery_204.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2CinematicThreadSampleDiscovery204.asi"
if errorlevel 1 goto :fail
echo Build succeeded: %~dp0..\test-asi\STALKER2CinematicThreadSampleDiscovery204.asi
popd
exit /b 0
:fail
echo ERROR: cinematic thread sample discovery build failed.
popd
exit /b 1
