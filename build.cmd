@echo off
call "E:\Work\Visual Studio Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
cl /nologo /LD /std:c++latest /O1 /MT /EHsc /utf-8 /DNDEBUG /Iexternal\safetyhook /Iexternal\spdlog\include src\gameplay_aspect_fix.cpp external\safetyhook\safetyhook.cpp external\safetyhook\Zydis.c /link user32.lib /OUT:STALKER2GameplayAspectFix.asi
