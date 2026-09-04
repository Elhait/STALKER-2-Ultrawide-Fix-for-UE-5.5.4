STALKER 2 Ultrawide and Camera Tweaks for UE 5.5.4
Version 0.5.0

[b]ABOUT[/b]

A unified ultrawide, cinematic and camera/FOV fix for S.T.A.L.K.E.R. 2: Heart of Chornobyl on Steam.

The mod corrects gameplay camera behavior on 21:9 and 32:9 displays, fixes cinematic aspect ratio and Hor+ FOV, adds custom cinematic framing, and provides FOV-aware dialogue zoom controls.

[b]COMPATIBILITY WITH CAMERA / FOV MODS[/b]

Compatibility with other mods that modify the game's camera, FOV, dialogue zoom, cinematic framing, aspect-ratio camera behavior, or related camera state is not guaranteed.

Bug reports must be reproduced with STALKER2CameraTweaks as the only installed camera/FOV-related mod. Other unrelated mods are fine unless they modify the same camera systems.

[b]FEATURES[/b]

[list]
[*]Fixes gameplay aspect transitions on 21:9 and 32:9.
[*]Automatically re-arms after gameplay camera rebuilds, including death and load.
[*]Preserves the selected gameplay FOV.
[*]Corrects cinematic aspect ratio using the game's runtime camera state.
[*]Applies Hor+ cinematic FOV correction.
[*]Supports custom cinematic framing: Auto, Native, 16:9, 21:9 and 32:9.
[*]Allows 16:9 users to preview wider cinematic framing with letterbox bars.
[*]Adds FOV-aware dialogue zoom: Native, Adaptive, Reduced and Disabled.
[*]Creates and synchronizes the INI configuration automatically.
[*]Uses guarded signature resolution and fails safely when validation does not pass.
[/list]

[b]CUSTOM CINEMATIC FRAMING[/b]

The cinematic framing can be selected independently from the physical display:

[list]
[*]Auto - follows the game's current runtime camera aspect.
[*]Native - leaves the game's original cinematic aspect and FOV behavior untouched.
[*]16:9 - forces 16:9 cinematic framing.
[*]21:9 - forces 21:9 cinematic framing.
[*]32:9 - forces 32:9 cinematic framing.
[/list]

For example, forcing 32:9 on a 16:9 display produces a wider cinematic presentation with black bars above and below. The cinematic FOV is handled automatically for the selected framing policy.

[b]DIALOGUE ZOOM[/b]

Version 0.5.0 adds dialogue zoom that adapts to the actual gameplay FOV:

[list]
[*]Native - keeps the game's original dialogue zoom, currently targeting 70 degrees.
[*]Adaptive - preserves the game's native optical zoom strength relative to the current gameplay FOV.
[*]Reduced - applies half of the Adaptive optical zoom strength. This is the default mode.
[*]Disabled - keeps the gameplay FOV during dialogue.
[/list]

The native dialogue transition timing is preserved, including smooth recovery when leaving dialogue.

[b]CONFIGURATION[/b]

The mod automatically creates STALKER2CameraTweaks.ini beside the game executable.

Default configuration:

[code]
[Gameplay]
Enabled=true

[Cinematics]
AspectRatio=Auto

[Dialogue]
Zoom=Reduced

[Hotkeys]
Enabled=false
CinematicCycle=F9
DialogueCycle=F10
[/code]

Manual INI changes require a game restart. Optional runtime hotkeys are intended mainly for testing and comparing settings; they are disabled by default. CinematicCycle selects the mode for the next cinematic, and DialogueCycle selects the mode for the next dialogue. An active cinematic or dialogue is not rebuilt after a hotkey press.

Supported hotkey bindings: F1-F12, 0-9 and A-Z.

[b]REQUIREMENTS[/b]

[list]
[*]Steam version of S.T.A.L.K.E.R. 2: Heart of Chornobyl.
[*][url=https://github.com/ThirteenAG/Ultimate-ASI-Loader]Ultimate ASI Loader (x64)[/url], with dsound.dll placed in Stalker2\Binaries\Win64.
[*]Tested game build: Steam 2.0.4.
[*]Unreal Engine target: 5.5.4.
[/list]

[b]INSTALLATION[/b]

[list=1]
[*]Install Ultimate ASI Loader and place dsound.dll in Stalker2\Binaries\Win64.
[*]Remove old STALKER2UltrawideFix.asi, STALKER2GameplayAspectFix.asi and their old INI/log files. Do not load them together with the new ASI.
[*]Extract STALKER2CameraTweaks.asi and STALKER2CameraTweaks.ini into the same Win64 folder.
[*]Launch the game.
[/list]

The plugin creates STALKER2CameraTweaks.log in the game folder. Include this log when reporting a compatibility problem; it records the mod and game SHA-256 values and signature-resolution status.

[b]TESTED[/b]

[list]
[*]21:9 and 32:9 gameplay, including startup, aspect switching and death/load camera rebuilds.
[*]Auto, Native, forced 16:9, 21:9 and 32:9 cinematic framing.
[*]Runtime aspect changes after changing resolution during the same session in Auto mode.
[*]Dialogue zoom Native, Adaptive, Reduced and Disabled on Steam 2.0.4.
[*]Sequential dialogue cycles and cinematic-to-dialogue state isolation.
[*]Configurable runtime hotkeys and INI template synchronization behavior.
[*]Dialogue resolver statically validated on Steam 2.0.2, 2.0.3 and 2.0.4. This is not runtime compatibility proof for the older builds.
[/list]

[b]KNOWN ISSUES[/b]

[list]
[*]A brief camera or projection shift may still be visible immediately after some cinematics while the game hands control back to gameplay.
[*]Weapon/viewmodel FOV can be incorrect after a cinematic, load or gameplay-camera rebuild. Aiming down sights or opening a menu refreshes the game's viewmodel state. This is a separate game-side issue and is not fixed by this release.
[*]Do not combine this release with the old gameplay ASI or experimental cinematic ASI files.
[/list]

[b]GAME UPDATE COMPATIBILITY[/b]

Gameplay, cinematic and dialogue code locations are resolved dynamically using guarded signatures and instruction validation rather than fixed patch-specific addresses.

Game updates that only relocate validated code may continue to work without a mod update, while structural changes may require new signatures. If validation fails or becomes ambiguous, the affected hook fails safely rather than installing against an unknown code path.

Runtime validation for this release was performed on Steam build 2.0.4. Static resolver validation across Steam 2.0.2-2.0.4 does not establish runtime support for the older builds.

[b]DEFENDER NOTICE[/b]

Microsoft Defender may occasionally flag unsigned ASI files. The release binary was reviewed and classified as “Not malware.” If an old detection remains, update Microsoft Defender security intelligence and rescan the file.

[b]SUPPORT DEVELOPMENT[/b]

This mod is maintained by [url=https://github.com/Elhait/STALKER-2-Ultrawide-Fix-for-UE-5.5.4]Elhait[/url].

If the mod helped you and you would like to support future updates:

[list]
[*][url=https://ko-fi.com/elhait]Ko-fi[/url]
[*][url=https://donatello.to/Elhait]Donatello[/url]
[/list]

[b]SOURCE CODE & RESEARCH[/b]

[list]
[*][url=https://github.com/Elhait/STALKER-2-Ultrawide-Fix-for-UE-5.5.4]GitHub source and research history[/url]
[*][url=https://www.nexusmods.com/stalker2heartofchornobyl/mods/2337]WIDEBOY Fixes by BigChenga[/url] - research reference used during dialogue FOV investigation; this mod's dialogue implementation was independently reverse engineered and runtime validated.
[*]Third-party libraries and code attribution are listed in THIRD_PARTY_NOTICES.md.
[/list]
