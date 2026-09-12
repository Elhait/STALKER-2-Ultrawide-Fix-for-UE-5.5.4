@echo off
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if not exist build-artifacts\obj mkdir build-artifacts\obj
if not exist build-artifacts\test-asi mkdir build-artifacts\test-asi
cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /Iexternal\safetyhook /Iexternal\spdlog\include src\cinematic_hardcoded_native_state_feasibility_204.cpp external\safetyhook\safetyhook.cpp external\safetyhook\Zydis.c /Fo:build-artifacts\obj\ /link user32.lib /OUT:build-artifacts\test-asi\STALKER2CinematicHardcodedNativeStateFeasibility204.asi
