@echo off
setlocal
if not exist build-artifacts\research mkdir build-artifacts\research
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 1
cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /DPOST_CINEMATIC_GAMEPLAY_REPLAY_DEFER_TEST /DPOST_CINEMATIC_GAMEPLAY_REPLAY_ATOMIC_EXIT_HANDOFF_TEST /Iexternal\safetyhook /Iexternal\spdlog\include src\experimental_cinematic_21_9_combined_fix_204.cpp external\safetyhook\safetyhook.cpp external\safetyhook\Zydis.c /link user32.lib bcrypt.lib /OUT:build-artifacts\research\STALKER2CameraTweaks-PostCinematicGameplayAtomicExitHandoffTest205.asi
exit /b %errorlevel%
