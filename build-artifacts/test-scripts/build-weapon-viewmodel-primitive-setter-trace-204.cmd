@echo off
setlocal
pushd "%~dp0..\obj"

if not exist "..\..\external\safetyhook\safetyhook.cpp" goto :fail
if not exist "..\..\external\spdlog\include\spdlog\spdlog.h" goto :fail
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail

cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\weapon_viewmodel_primitive_setter_trace_204.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2WeaponViewmodelPrimitiveSetterTrace204.asi"
if errorlevel 1 goto :fail
echo Build succeeded: %~dp0..\test-asi\STALKER2WeaponViewmodelPrimitiveSetterTrace204.asi
pause
popd
exit /b 0

:fail
echo ERROR: Primitive setter trace build failed.
pause
popd
exit /b 1
