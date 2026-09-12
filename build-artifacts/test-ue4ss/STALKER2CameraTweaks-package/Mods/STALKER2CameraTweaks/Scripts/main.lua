-- STALKER2CameraTweaks — UE4SS parallel implementation
--
-- This is a best-effort reflected/property port of the stable ASI policies.
-- It deliberately does not call functions with complex ref/out arguments and
-- does not claim that a property write rebuilds UE's downstream projection.

local MODULE = "STALKER2CameraTweaks"
local SAMPLE_MS = 100
local DISCOVERY_MS = 1000
local NATIVE_ASPECT = 16.0 / 9.0
local DEFAULT_DISPLAY_ASPECT = 32.0 / 9.0
local EPSILON = 0.01

local config = {
    gameplayEnabled = true,
    cinematicAspect = "Auto",
    displayAspect = DEFAULT_DISPLAY_ASPECT,
    preserveAuthoredFov = false,
    dialogueZoom = "Reduced",
    hotkeysEnabled = false,
}

local cache = { camera = nil, manager = nil, pcm = nil }
local lastDiscovery = 0
local lastLogKey = nil
local lastState = nil
local cinematicActive = false
local cinematicBaselineFov = nil
local dialogueBaselineFov = nil
local dialogueActive = false
local lastGameplayFov = nil
local sequence = 0
local gameplayStage = "idle"
local gameplayCameraIdentity = nil
local gameplayAttempts = 0
local MAX_GAMEPLAY_ATTEMPTS = 4

local function emit(message)
    print("[" .. MODULE .. "] " .. message .. "\n")
end

local function valid(object)
    if object == nil then return false end
    local ok, result = pcall(function() return object:IsValid() end)
    return ok and result == true
end

local function value(fn, fallback)
    local ok, result = pcall(fn)
    if not ok or result == nil then return fallback end
    return result
end

local function name(object)
    if not valid(object) then return "<none>" end
    return tostring(value(function() return object:GetFullName() end, "<unnamed>"))
end

local function boolText(v)
    if v == true then return "true" end
    if v == false then return "false" end
    return tostring(v)
end

local function number(v)
    local n = tonumber(v)
    return n
end

local function read(object, property, fallback)
    if not valid(object) then return fallback end
    return value(function() return object[property] end, fallback)
end

local function write(object, property, newValue)
    if not valid(object) then return false, "invalid-object" end
    local ok, err = pcall(function() object[property] = newValue end)
    if ok then return true, "property" end
    return false, tostring(err)
end

local function callSimple(object, method, argument)
    if not valid(object) then return false, "invalid-object" end
    local ok, result = pcall(function() return object[method](object, argument) end)
    if ok then return true, result end
    return false, tostring(result)
end

local function setCameraProperty(camera, property, newValue, setter)
    -- Prefer the reflected setter when it has a simple value-only signature;
    -- fall back to the known reflected property if the setter is unavailable.
    if setter then
        local ok = callSimple(camera, setter, newValue)
        if ok then return true, setter end
    end
    return write(camera, property, newValue)
end

local function parseBool(text, fallback)
    if text == nil then return fallback end
    text = string.lower(text:gsub("^%s+", ""):gsub("%s+$", ""))
    if text == "1" or text == "true" or text == "yes" or text == "on" then return true end
    if text == "0" or text == "false" or text == "no" or text == "off" then return false end
    return fallback
end

local function parseConfig()
    local paths = {
        "Mods/STALKER2CameraTweaks/STALKER2CameraTweaks.ini",
        "STALKER2CameraTweaks.ini",
    }
    local file = nil
    for _, path in ipairs(paths) do
        local handle = io.open(path, "r")
        if handle then file = handle break end
    end
    if not file then
        emit("CONFIG_DEFAULTS no readable INI; using documented defaults")
        return
    end

    local section = ""
    for line in file:lines() do
        local trimmed = line:gsub("^%s+", ""):gsub("%s+$", "")
        if trimmed ~= "" and not trimmed:match("^[;#]") then
            local newSection = trimmed:match("^%[(.-)%]$")
            if newSection then
                section = string.lower(newSection)
            else
                local key, raw = trimmed:match("^([^=]+)=(.*)$")
                if key and raw then
                    key = string.lower(key:gsub("^%s+", ""):gsub("%s+$", ""))
                    raw = raw:gsub("^%s+", ""):gsub("%s+$", "")
                    if section == "gameplay" and key == "enabled" then
                        config.gameplayEnabled = parseBool(raw, config.gameplayEnabled)
                    elseif section == "cinematics" and key == "aspectratio" then
                        config.cinematicAspect = raw
                    elseif section == "cinematics" and key == "displayaspectratio" then
                        config.displayAspect = tonumber(raw) or config.displayAspect
                    elseif section == "cinematics" and key == "preserveauthoredfov" then
                        config.preserveAuthoredFov = parseBool(raw, config.preserveAuthoredFov)
                    elseif section == "dialogue" and key == "zoom" then
                        config.dialogueZoom = raw
                    elseif section == "hotkeys" and key == "enabled" then
                        config.hotkeysEnabled = parseBool(raw, config.hotkeysEnabled)
                    end
                end
            end
        end
    end
    file:close()
    emit("CONFIG Gameplay.Enabled=" .. tostring(config.gameplayEnabled) ..
        " Cinematics.AspectRatio=" .. config.cinematicAspect ..
        " Cinematics.DisplayAspectRatio=" .. tostring(config.displayAspect) ..
        " Cinematics.PreserveAuthoredFOV=" .. tostring(config.preserveAuthoredFov) ..
        " Dialogue.Zoom=" .. config.dialogueZoom)
end

local function findCamera()
    local objects = FindAllOf("CameraComponent")
    if not objects then return nil end
    local fallback = nil
    for _, object in pairs(objects) do
        if valid(object) then
            local full = name(object)
            if full:match("%.Camera$") and full:find("BP_Stalker2Character_C", 1, true) then
                return object
            end
            if fallback == nil and full:match("%.Camera$") then fallback = object end
        end
    end
    return fallback
end

local function findManager()
    local objects = FindAllOf("CameraManager")
    if not objects then return nil end
    for _, object in pairs(objects) do
        if valid(object) then return object end
    end
    return nil
end

local function discover(force)
    local now = os.clock()
    if not force and now - lastDiscovery < DISCOVERY_MS / 1000.0 and
        valid(cache.camera) and valid(cache.manager) and valid(cache.pcm) then
        return
    end
    lastDiscovery = now

    if not valid(cache.manager) then cache.manager = findManager() end
    if not valid(cache.camera) then cache.camera = findCamera() end
    if not valid(cache.pcm) then cache.pcm = FindFirstOf("PlayerCameraManager") end

    if valid(cache.manager) then
        local managerCamera = value(function() return cache.manager.CameraComponent end, nil)
        if not valid(managerCamera) then managerCamera = value(function() return cache.manager.Camera end, nil) end
        local managerPcm = value(function() return cache.manager.PlayerCameraManager end, nil)
        if not valid(managerPcm) then managerPcm = value(function() return cache.manager.PCM end, nil) end
        if valid(managerCamera) then cache.camera = managerCamera end
        if valid(managerPcm) then cache.pcm = managerPcm end
    end
end

local function cameraState(camera)
    return {
        fov = number(read(camera, "FieldOfView", nil)),
        first = number(read(camera, "FirstPersonFieldOfView", nil)),
        aspect = number(read(camera, "AspectRatio", nil)),
        axis = tostring(read(camera, "AspectRatioAxisConstraint", "<unavailable>")),
        constrain = read(camera, "bConstrainAspectRatio", nil),
        overrideAxis = read(camera, "bOverrideAspectRatioAxisConstraint", nil),
    }
end

local function pcmState(pcm)
    if not valid(pcm) then return {} end
    local current = value(function() return pcm.CameraCachePrivate.POV end, nil)
    local last = value(function() return pcm.LastFrameCameraCachePrivate.POV end, nil)
    local target = value(function() return pcm.ViewTarget.POV end, nil)
    local function pov(object)
        return {
            fov = number(read(object, "FOV", nil)),
            aspect = number(read(object, "AspectRatio", nil)),
            constrain = read(object, "bConstrainAspectRatio", nil),
        }
    end
    return { current = pov(current), last = pov(last), target = pov(target) }
end

local function fmt(v)
    if v == nil then return "<unavailable>" end
    if type(v) == "number" then return string.format("%.6f", v) end
    return boolText(v)
end

local function logState(reason, camera, pcm, phase)
    local key = table.concat({ phase, fmt(camera.fov), fmt(camera.aspect), fmt(camera.constrain),
        fmt(pcm.current and pcm.current.fov), fmt(pcm.current and pcm.current.aspect),
        fmt(pcm.current and pcm.current.constrain) }, "|")
    if key == lastLogKey then return end
    lastLogKey = key
    sequence = sequence + 1
    emit("STATE seq=" .. tostring(sequence) .. " reason=" .. reason .. " phase=" .. phase ..
        " Camera=" .. name(cache.camera) .. " FOV=" .. fmt(camera.fov) ..
        " FirstPersonFOV=" .. fmt(camera.first) .. " Aspect=" .. fmt(camera.aspect) ..
        " Axis=" .. camera.axis .. " Constrain=" .. fmt(camera.constrain) ..
        " OverrideAxis=" .. fmt(camera.overrideAxis) ..
        " PCM.Current=" .. fmt(pcm.current and pcm.current.fov) .. "/" ..
        fmt(pcm.current and pcm.current.aspect) .. "/" .. fmt(pcm.current and pcm.current.constrain) ..
        " PCM.Last=" .. fmt(pcm.last and pcm.last.fov) .. "/" .. fmt(pcm.last and pcm.last.aspect) ..
        "/" .. fmt(pcm.last and pcm.last.constrain) .. " PCM.Target=" ..
        fmt(pcm.target and pcm.target.fov) .. "/" .. fmt(pcm.target and pcm.target.aspect) ..
        "/" .. fmt(pcm.target and pcm.target.constrain))
end

local function targetAspect()
    local policy = string.lower(config.cinematicAspect)
    if policy == "native" or policy == "16:9" or policy == "16x9" then return NATIVE_ASPECT end
    if policy == "21:9" or policy == "21x9" then return 21.0 / 9.0 end
    if policy == "32:9" or policy == "32x9" then return 32.0 / 9.0 end
    return config.displayAspect
end

local function horPlus(sourceFov, aspect)
    local radians = math.pi / 180.0
    local projected = math.tan(sourceFov * 0.5 * radians) * (aspect / NATIVE_ASPECT)
    return 2.0 * math.atan(projected) / radians
end

local function apply(camera, property, newValue, setter, label)
    local old = read(camera, property, nil)
    if old == newValue or (type(old) == "number" and math.abs(old - newValue) <= EPSILON) then
        emit("APPLY label=" .. label .. " property=" .. property .. " old=" .. fmt(old) ..
            " new=" .. fmt(newValue) .. " result=already-set")
        return true
    end
    local ok, route = setCameraProperty(camera, property, newValue, setter)
    emit("APPLY label=" .. label .. " property=" .. property .. " old=" .. fmt(old) ..
        " new=" .. fmt(newValue) .. " result=" .. (ok and "accepted" or "blocked") ..
        " route=" .. tostring(route))
    return ok
end

local function applyCinematic(camera, authoredFov)
    local aspect = targetAspect()
    if string.lower(config.cinematicAspect) == "native" then return end
    apply(camera, "AspectRatio", aspect, "SetAspectRatio", "cinematic-aspect")
    apply(camera, "bConstrainAspectRatio", true, "SetConstraintAspectRatio", "cinematic-constraint")
    if not config.preserveAuthoredFov and authoredFov and authoredFov > 1.0 and authoredFov < 179.0 then
        apply(camera, "FieldOfView", horPlus(authoredFov, aspect), nil, "cinematic-horplus-fov")
    end
end

local function applyGameplay(camera, state)
    if not config.gameplayEnabled or state.aspect == nil then return end
    if gameplayCameraIdentity ~= name(camera) then
        gameplayCameraIdentity = name(camera)
        gameplayStage = "idle"
        gameplayAttempts = 0
        emit("GAMEPLAY_CAMERA_BOUND " .. gameplayCameraIdentity)
    end

    if gameplayStage == "idle" then
        if state.aspect > NATIVE_ASPECT + 0.05 then
            gameplayStage = "await-constrained"
            gameplayAttempts = 1
            emit("GAMEPLAY_TRANSITION_BEGIN observed=" .. fmt(state.aspect) .. "/false")
            -- Confirmed native order phase 1: 3.555556/0x04 -> 3.555556/0x05.
            apply(camera, "bConstrainAspectRatio", true, "SetConstraintAspectRatio", "gameplay-phase1-constrain-on")
        end
        return
    end

    if gameplayStage == "await-constrained" then
        gameplayAttempts = gameplayAttempts + 1
        if state.constrain == true then
            -- Confirmed native order phase 2: 3.555556/0x05 -> 1.777778/0x05.
            apply(camera, "AspectRatio", NATIVE_ASPECT, "SetAspectRatio", "gameplay-phase2-native-aspect")
            gameplayStage = "await-native-aspect"
        elseif gameplayAttempts >= MAX_GAMEPLAY_ATTEMPTS then
            emit("GAMEPLAY_TRANSITION_BLOCKED phase=constrain-on-not-observed")
            gameplayStage = "blocked"
        end
        return
    end

    if gameplayStage == "await-native-aspect" then
        gameplayAttempts = gameplayAttempts + 1
        if state.constrain == true and state.aspect <= NATIVE_ASPECT + 0.05 then
            -- Confirmed native order phase 3: 1.777778/0x05 -> 1.777778/0x04.
            apply(camera, "bConstrainAspectRatio", false, "SetConstraintAspectRatio", "gameplay-phase3-constrain-off")
            gameplayStage = "await-final"
        elseif gameplayAttempts >= MAX_GAMEPLAY_ATTEMPTS then
            emit("GAMEPLAY_TRANSITION_BLOCKED phase=native-aspect-not-observed")
            gameplayStage = "blocked"
        end
        return
    end

    if gameplayStage == "await-final" then
        gameplayAttempts = gameplayAttempts + 1
        if state.constrain == false and state.aspect <= NATIVE_ASPECT + 0.05 then
            emit("GAMEPLAY_TRANSITION_COMPLETE observed=90/1.777778/false visual=UNVALIDATED")
            gameplayStage = "complete"
        elseif gameplayAttempts >= MAX_GAMEPLAY_ATTEMPTS then
            emit("GAMEPLAY_TRANSITION_BLOCKED phase=final-state-not-observed")
            gameplayStage = "blocked"
        end
        return
    end

    if (gameplayStage == "complete" or gameplayStage == "blocked") and
        state.constrain == false and state.aspect > NATIVE_ASPECT + 0.05 then
        gameplayStage = "idle"
        gameplayAttempts = 0
        emit("GAMEPLAY_TRANSITION_REARM observed-wide-state")
    end
end

local function dialogueTarget(baseline)
    local nativeZoom = math.tan(90.0 * math.pi / 360.0) / math.tan(70.0 * math.pi / 360.0)
    local projection = math.tan(baseline * math.pi / 360.0)
    local mode = string.lower(config.dialogueZoom)
    if mode == "adaptive" then
        projection = projection / nativeZoom
    elseif mode == "reduced" then
        projection = projection / math.sqrt(nativeZoom)
    elseif mode == "disabled" then
        return baseline
    else
        return 70.0
    end
    return 2.0 * math.atan(projection) * 180.0 / math.pi
end

local function processDialogue(camera, state)
    if not config.gameplayEnabled or cinematicActive or state.fov == nil then return end
    if config.dialogueZoom == "Native" or config.dialogueZoom == "native" then
        dialogueActive = false
        dialogueBaselineFov = nil
        lastGameplayFov = state.fov
        return
    end
    if lastGameplayFov and state.fov < lastGameplayFov - 1.0 and not dialogueActive then
        dialogueBaselineFov = lastGameplayFov
        dialogueActive = true
        emit("DIALOGUE_START baselineFOV=" .. fmt(dialogueBaselineFov) .. " policy=" .. config.dialogueZoom)
    end
    if dialogueActive and dialogueBaselineFov then
        if state.fov > dialogueBaselineFov - 1.0 then
            emit("DIALOGUE_END restoredFOV=" .. fmt(state.fov))
            dialogueActive = false
            dialogueBaselineFov = nil
        else
            apply(camera, "FieldOfView", dialogueTarget(dialogueBaselineFov), nil, "dialogue-zoom")
        end
    end
    lastGameplayFov = state.fov
end

local function tick(reason)
    discover(false)
    if not valid(cache.camera) or not valid(cache.pcm) then
        if lastState ~= "pending" then emit("OBJECTS_PENDING Camera=" .. name(cache.camera) .. " PCM=" .. name(cache.pcm)) end
        lastState = "pending"
        return
    end
    local camera = cameraState(cache.camera)
    local pcm = pcmState(cache.pcm)
    local wideUnconstrained = camera.constrain == false and camera.aspect ~= nil and camera.aspect > 2.0

    -- This build is intentionally gameplay-only. Cinematic/dialogue mutation
    -- code remains documented for the future port, but is not executed here.
    applyGameplay(cache.camera, camera)
    local phase = wideUnconstrained and "GAMEPLAY_WIDE" or "GAMEPLAY"
    logState(reason, camera, pcm, phase .. ":STAGE=" .. gameplayStage)
end

parseConfig()

-- Optional controls are deliberately disabled by default to avoid conflicts
-- with the game, OptiScaler and ReShade. End remains a harmless diagnostic
-- marker and is intentionally not a control for camera mutation.
RegisterKeyBind(0x23, function() tick("manual") end)
LoopAsync(SAMPLE_MS, function()
    tick("poll")
    return false
end)

emit("loaded; gameplay-only native-like two-pass prototype; cinematic/dialogue mutation disabled; End logs a marker")
