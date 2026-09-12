-- CameraEvaluationAutoDump
-- Read-only UE4SS diagnostic mod for STALKER 2.
-- It observes live camera objects; it never edits properties or calls hooks.

local SAMPLE_INTERVAL_MS = 500
local DISCOVERY_INTERVAL_MS = 2000
local ENABLE_FUNCTION_INVENTORY = false

local sequence = 0
local lastDiscoveryAt = 0
local lastStateKey = nil
local lastPendingKey = nil
local lastPhase = nil
local lastOwnershipKey = nil
local cached = {
    camera = nil,
    manager = nil,
    pcm = nil,
}
local cameraCandidatesLogged = false
local functionInventoryLogged = {}
local installedHooks = {}
local hookLastLogAt = {}

local function now()
    return os.date("%Y-%m-%d %H:%M:%S")
end

local function safeValue(fn)
    local ok, value = pcall(fn)
    if not ok or value == nil then
        return "<unavailable>"
    end
    return tostring(value)
end

local function safeObject(fn)
    local ok, value = pcall(fn)
    if not ok or value == nil then return nil end
    return value
end

local function valid(object)
    if object == nil then return false end
    local ok, result = pcall(function() return object:IsValid() end)
    return ok and result == true
end

local function fullName(object)
    if not valid(object) then return "<none>" end
    return safeValue(function() return object:GetFullName() end)
end

local function emit(line)
    print("[CameraEvaluationAutoDump] " .. line .. "\n")
end

local function installReadOnlyHook(functionName, label, minimumIntervalSeconds)
    if installedHooks[label] then return end
    local ok, preId, postId = pcall(function()
        return RegisterHook(functionName, function(self, ...)
            local nowSeconds = os.time()
            local previous = hookLastLogAt[label] or 0
            if nowSeconds - previous >= (minimumIntervalSeconds or 0) then
                hookLastLogAt[label] = nowSeconds
                emit("HOOK_PRE label=" .. label .. " function=" .. functionName ..
                    " self=" .. fullName(self))
            end
        end)
    end)
    if ok then
        installedHooks[label] = { preId = preId, postId = postId, functionName = functionName }
        emit("HOOK_INSTALLED label=" .. label .. " function=" .. functionName)
    else
        emit("HOOK_FAILED label=" .. label .. " function=" .. functionName ..
            " error=" .. tostring(preId))
    end
end

local function findPlayerCameraComponent()
    local objects = FindAllOf("CameraComponent")
    if not objects then return nil end

    local fallback = nil
    local matches = 0
    local total = 0
    for _, object in pairs(objects) do
        total = total + 1
        local name = fullName(object)
        if name:match("%.Camera$") then
            matches = matches + 1
            if name:find("BP_Stalker2Character_C", 1, true) then
                fallback = object
            elseif fallback == nil then
                fallback = object
            end
        end
    end

    if not cameraCandidatesLogged then
        cameraCandidatesLogged = true
        emit("camera inventory: classMatches=" .. tostring(total) .. " pathCandidates=" .. tostring(matches))
    end
    return fallback
end

local function findStalkerCameraManager()
    local objects = FindAllOf("CameraManager")
    if not objects then return nil end

    local result = nil
    for _, object in pairs(objects) do
        local name = fullName(object)
        -- GetFullName() for this live object is formatted as
        -- "CameraManager /Game/...", so the native class path is not present.
        if name:match("^CameraManager%s") then
            result = object
            break
        end
    end
    return result
end

local function logRelevantFunctions(object, label)
    if not valid(object) or functionInventoryLogged[label] then return end
    functionInventoryLogged[label] = true

    if not ENABLE_FUNCTION_INVENTORY then
        emit("FUNCTION_INVENTORY_SKIPPED label=" .. label ..
            " reason=UE4SS_3_0_1_CALLABILITY_UNVERIFIED")
        return
    end

    local ok, errorMessage = pcall(function()
        local class = object:GetClass()
        local className = fullName(class)
        local count = 0
        emit("FUNCTION_INVENTORY_BEGIN label=" .. label .. " class=" .. className)
        local iterator = class.ForEachFunction
        if type(iterator) ~= "function" then
            emit("FUNCTION_INVENTORY_UNAVAILABLE label=" .. label ..
                " reason=UE4SS_API_NOT_EXPOSED")
            return
        end
        iterator(class, function(functionObject)
            local name = fullName(functionObject)
            local lower = string.lower(name)
            if lower:find("aspect", 1, true) or
               lower:find("camera", 1, true) or
               lower:find("view", 1, true) or
               lower:find("fov", 1, true) or
               lower:find("projection", 1, true) or
               lower:find("viewport", 1, true) or
               lower:find("resolution", 1, true) or
               lower:find("constraint", 1, true) or
               lower:find("calc", 1, true) then
                count = count + 1
                if count <= 120 then
                    emit("FUNCTION_CANDIDATE label=" .. label .. " name=" .. name)
                end
            end
            return false
        end)
        emit("FUNCTION_INVENTORY_END label=" .. label .. " candidates=" .. tostring(count))
    end)
    if not ok then
        emit("FUNCTION_INVENTORY_FAILED label=" .. label .. " error=" .. tostring(errorMessage))
    end
end

local function discover(force)
    local tick = os.time()
    if not force and tick == lastDiscoveryAt and valid(cached.camera) and valid(cached.manager) and valid(cached.pcm) then
        return
    end
    lastDiscoveryAt = tick

    if not valid(cached.camera) then cached.camera = findPlayerCameraComponent() end
    if not valid(cached.manager) then cached.manager = findStalkerCameraManager() end
    if not valid(cached.pcm) then cached.pcm = FindFirstOf("PlayerCameraManager") end

    -- Prefer the coordinator's own references when UE4SS exposes them. The
    -- property spelling is build/game specific, so try only known reflected
    -- names and keep the global discovery fallback.
    if valid(cached.manager) then
        local managerCamera = safeObject(function() return cached.manager.CameraComponent end)
        if not valid(managerCamera) then
            managerCamera = safeObject(function() return cached.manager.Camera end)
        end
        if valid(managerCamera) then cached.camera = managerCamera end

        local managerPcm = safeObject(function() return cached.manager.PlayerCameraManager end)
        if not valid(managerPcm) then
            managerPcm = safeObject(function() return cached.manager.PCM end)
        end
        if valid(managerPcm) then cached.pcm = managerPcm end
    end

    -- One-time, bounded inventory of reflected functions on the already
    -- identified ownership chain. This only produces candidate names for a
    -- later explicit hook decision; it does not register hooks or call them.
    logRelevantFunctions(cached.camera, "CameraComponent")
    logRelevantFunctions(cached.manager, "CameraManager")
    logRelevantFunctions(cached.pcm, "PlayerCameraManager")

    if not valid(cached.camera) or not valid(cached.manager) or not valid(cached.pcm) then
        local pendingKey = fullName(cached.camera) .. "|" .. fullName(cached.manager) .. "|" .. fullName(cached.pcm)
        if pendingKey ~= lastPendingKey then
            lastPendingKey = pendingKey
            emit("objects pending: Camera=" .. fullName(cached.camera) ..
                " CameraManager=" .. fullName(cached.manager) ..
                " PCM=" .. fullName(cached.pcm))
        end
    else
        lastPendingKey = nil
    end
end

local function readCameraState(camera)
    if not valid(camera) then
        return {
            path = "<none>", fov = "<unavailable>", firstPersonFov = "<unavailable>",
            aspect = "<unavailable>", axis = "<unavailable>", constrain = "<unavailable>",
            overrideAxis = "<unavailable>",
        }
    end
    return {
        path = fullName(camera),
        fov = safeValue(function() return camera.FieldOfView end),
        firstPersonFov = safeValue(function() return camera.FirstPersonFieldOfView end),
        aspect = safeValue(function() return camera.AspectRatio end),
        axis = safeValue(function() return camera.AspectRatioAxisConstraint end),
        constrain = safeValue(function() return camera.bConstrainAspectRatio end),
        overrideAxis = safeValue(function() return camera.bOverrideAspectRatioAxisConstraint end),
    }
end

local function classifyObservedPhase(camera)
    local aspect = tonumber(camera.aspect)
    local constrain = camera.constrain
    if aspect == nil then return "UNKNOWN" end

    -- These labels describe the previously captured native control sequence;
    -- they do not assert that a final storage state is sufficient by itself.
    if aspect > 2.0 and constrain == "false" then return "A_WIDE_BEFORE" end
    if aspect >= 1.70 and aspect <= 1.90 and constrain == "true" then return "B_16_9_CONSTRAINED" end
    if aspect >= 1.70 and aspect <= 1.90 and constrain == "false" then return "C_POST_REEVALUATION" end
    return "OTHER"
end

local function readPov(pov)
    if pov == nil then
        return { fov = "<unavailable>", firstPersonFov = "<unavailable>", aspect = "<unavailable>", constrain = "<unavailable>" }
    end
    return {
        fov = safeValue(function() return pov.FOV end),
        firstPersonFov = safeValue(function() return pov.FirstPersonFOV end),
        aspect = safeValue(function() return pov.AspectRatio end),
        constrain = safeValue(function() return pov.bConstrainAspectRatio end),
    }
end

local function readPcmState(pcm)
    if not valid(pcm) then
        return { path = "<none>", current = readPov(nil), last = readPov(nil), viewTarget = readPov(nil), target = "<none>" }
    end
    local current = safeObject(function() return pcm.CameraCachePrivate.POV end)
    local last = safeObject(function() return pcm.LastFrameCameraCachePrivate.POV end)
    local viewTarget = safeObject(function() return pcm.ViewTarget.POV end)
    return {
        path = fullName(pcm),
        current = readPov(current),
        last = readPov(last),
        viewTarget = readPov(viewTarget),
        target = safeValue(function() return pcm.ViewTarget.Target:GetFullName() end),
    }
end

local function readManagerState(manager)
    if not valid(manager) then return { path = "<none>", camera = "<none>", pcm = "<none>" } end
    return {
        path = fullName(manager),
        camera = safeValue(function() return manager.CameraComponent:GetFullName() end),
        pcm = safeValue(function() return manager.PlayerCameraManager:GetFullName() end),
    }
end

local function snapshot(reason, force)
    discover(force)
    local camera = readCameraState(cached.camera)
    local manager = readManagerState(cached.manager)
    local pcm = readPcmState(cached.pcm)
    local phase = classifyObservedPhase(camera)
    local ownershipKey = camera.path .. "|" .. manager.path .. "|" .. pcm.path ..
        "|" .. manager.camera .. "|" .. manager.pcm

    if ownershipKey ~= lastOwnershipKey then
        if lastOwnershipKey ~= nil then
            emit("OWNERSHIP_CHANGED previous=" .. lastOwnershipKey .. " current=" .. ownershipKey)
        else
            emit("OWNERSHIP_INITIAL " .. ownershipKey)
        end
        lastOwnershipKey = ownershipKey
    end

    if phase ~= lastPhase then
        emit("OBSERVED_PHASE " .. phase .. " reason=" .. reason ..
            " CameraFOV=" .. camera.fov .. " CameraAspect=" .. camera.aspect ..
            " CameraConstrain=" .. camera.constrain)
        lastPhase = phase
    end

    local key = table.concat({
        camera.fov, camera.firstPersonFov, camera.aspect, camera.axis,
        camera.constrain, camera.overrideAxis,
        pcm.current.fov, pcm.current.aspect, pcm.current.constrain,
        pcm.last.fov, pcm.last.aspect, pcm.last.constrain,
        pcm.viewTarget.fov, pcm.viewTarget.aspect, pcm.viewTarget.constrain,
    }, "|")

    if not force and key == lastStateKey then return end
    lastStateKey = key
    sequence = sequence + 1

    emit("SNAPSHOT seq=" .. tostring(sequence) .. " time=" .. now() .. " reason=" .. reason)
    emit("Camera path=" .. camera.path)
    emit("Camera FOV=" .. camera.fov .. " FirstPersonFOV=" .. camera.firstPersonFov ..
        " Aspect=" .. camera.aspect .. " Axis=" .. camera.axis ..
        " Constrain=" .. camera.constrain .. " OverrideAxis=" .. camera.overrideAxis)
    emit("CameraManager path=" .. manager.path)
    emit("CameraManager.Camera=" .. manager.camera)
    emit("CameraManager.PCM=" .. manager.pcm)
    emit("PCM path=" .. pcm.path .. " ViewTarget=" .. pcm.target)
    emit("PCM Current FOV=" .. pcm.current.fov .. " FirstPersonFOV=" .. pcm.current.firstPersonFov ..
        " Aspect=" .. pcm.current.aspect .. " Constrain=" .. pcm.current.constrain)
    emit("PCM LastFrame FOV=" .. pcm.last.fov .. " FirstPersonFOV=" .. pcm.last.firstPersonFov ..
        " Aspect=" .. pcm.last.aspect .. " Constrain=" .. pcm.last.constrain)
    emit("PCM ViewTarget.POV FOV=" .. pcm.viewTarget.fov .. " FirstPersonFOV=" .. pcm.viewTarget.firstPersonFov ..
        " Aspect=" .. pcm.viewTarget.aspect .. " Constrain=" .. pcm.viewTarget.constrain)
    emit("END SNAPSHOT seq=" .. tostring(sequence))
end

-- End is used as a low-conflict manual marker. Use the numeric Win32 virtual
-- key because this UE4SS build exposes RegisterKeyBind(integer) but does not
-- expose every key through the optional Key table. F5/F6 are save/load, Insert
-- is reserved by OptiScaler and Home is reserved by ReShade in this setup.
RegisterKeyBind(0x23, function()
    snapshot("manual", true)
end)

emit("loaded; read-only; End forces a snapshot; automatic change capture active")
-- These names come from the installed current-build UHT headers. Keep this
-- list intentionally small: two state-entry functions and one camera-view
-- evaluation function, with no property writes or guessed native addresses.
installReadOnlyHook("/Script/Engine.CameraComponent:SetAspectRatio", "CameraComponent.SetAspectRatio", 0)
installReadOnlyHook("/Script/Engine.CameraComponent:SetConstraintAspectRatio", "CameraComponent.SetConstraintAspectRatio", 0)
installReadOnlyHook("/Script/Engine.CameraComponent:GetCameraView", "CameraComponent.GetCameraView", 1)
LoopAsync(SAMPLE_INTERVAL_MS, function()
    snapshot("changed", false)
    return false
end)
