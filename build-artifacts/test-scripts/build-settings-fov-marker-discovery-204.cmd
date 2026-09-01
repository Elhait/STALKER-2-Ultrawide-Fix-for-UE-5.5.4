@echo off
setlocal
pushd "%~dp0..\obj"
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail
cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\spdlog\include ..\..\src\settings_fov_marker_discovery_204.cpp /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2SettingsFovMarkerDiscovery204.asi"
if errorlevel 1 goto :fail
echo Build succeeded: %~dp0..\test-asi\STALKER2SettingsFovMarkerDiscovery204.asi
popd
exit /b 0
:fail
echo ERROR: settings FOV marker discovery build failed.
popd
exit /b 1
