@echo off
setlocal
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 exit /b %errorlevel%
cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG ^
  /I"E:\Work\Slaker2 mods\01-Projects\STALKER-2-Ultrawide-Fix-for-UE-5.5.4\external\safetyhook" ^
  /I"E:\Work\Slaker2 mods\01-Projects\STALKER-2-Ultrawide-Fix-for-UE-5.5.4\external\spdlog\include" ^
  "E:\Work\Slaker2 mods\01-Projects\STALKER-2-Ultrawide-Fix-for-UE-5.5.4\src\experimental_cinematic_21_9_combined_fix_204.cpp" ^
  "E:\Work\Slaker2 mods\01-Projects\STALKER-2-Ultrawide-Fix-for-UE-5.5.4\external\safetyhook\safetyhook.cpp" ^
  "E:\Work\Slaker2 mods\01-Projects\STALKER-2-Ultrawide-Fix-for-UE-5.5.4\external\safetyhook\Zydis.c" ^
  /link user32.lib bcrypt.lib ^
  /OUT:"E:\Work\Slaker2 mods\01-Projects\STALKER-2-Ultrawide-Fix-for-UE-5.5.4\build-artifacts\research\STALKER2CameraTweaks-IntegratedPostExitTrace205.asi"
exit /b %errorlevel%
