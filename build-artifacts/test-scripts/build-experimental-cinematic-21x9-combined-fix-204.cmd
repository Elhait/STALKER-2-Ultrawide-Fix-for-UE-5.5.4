@echo off
setlocal
pushd "%~dp0..\obj"
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail

cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\experimental_cinematic_21_9_combined_fix_204.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib bcrypt.lib /OUT:"%~dp0..\test-asi\STALKER2UltrawideFix.asi"
if errorlevel 1 goto :fail
echo Build succeeded: %~dp0..\test-asi\STALKER2UltrawideFix.asi
popd
exit /b 0

:fail
echo ERROR: experimental cinematic 21:9 combined fix build failed.
popd
exit /b 1
