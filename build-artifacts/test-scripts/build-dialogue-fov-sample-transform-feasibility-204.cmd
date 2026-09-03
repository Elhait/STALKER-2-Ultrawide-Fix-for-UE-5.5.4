@echo off
setlocal
pushd "%~dp0..\obj"
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 goto :fail
where cl.exe >nul 2>&1
if errorlevel 1 goto :fail
cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /DDIALOGUE_FEASIBILITY_POLICY=1 /I..\..\external\safetyhook /I..\..\external\spdlog\include ..\..\src\dialogue_fov_sample_transform_feasibility_204.cpp ..\..\external\safetyhook\safetyhook.cpp ..\..\external\safetyhook\Zydis.c /link user32.lib /OUT:"%~dp0..\test-asi\STALKER2DialogueFovSampleTransformFeasibility204.asi"
if errorlevel 1 goto :fail
echo Build succeeded: F7 runtime policy toggle feasibility ASI
popd
exit /b 0
:fail
echo ERROR: dialogue FOV sample transform feasibility build failed.
popd
exit /b 1
