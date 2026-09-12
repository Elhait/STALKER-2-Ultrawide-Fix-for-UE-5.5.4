-- CinematicLetterboxObserver
-- Read-only UE4SS Phase 1 observer. No hooks, writes or Live View calls.

local SAMPLE_INTERVAL_MS = 100
local DISCOVERY_INTERVAL_MS = 1000

local sequence = 0
local lastDiscoveryAt = 0
local lastState = nil
local lastOwnership = nil
local previousSnapshot = nil
local cached = { camera = nil, manager = nil, pcm = nil }

local function now()
    return os.date("%Y-%m-%d %H:%M:%S")
end

local function emit(line)
    print("[CinematicLetterboxObserver] " .. line .. "\n")
end

local function safeValue(fn)
    local ok, value = pcall(fn)
    if not ok or value == nil then return "<unavailable>" end
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

local function findCamera()
    local objects = FindAllOf("CameraComponent")
    if not objects then return nil end
    local fallback = nil
    for _, object in pairs(objects) do
        local name = fullName(object)
        if name:match("%.Camera$") then
            fallback = fallback or object
            if name:find("BP_Stalker2Character_C", 1, true) then return object end
        end
    end
    return fallback
end

local function findManager()
    local objects = FindAllOf("CameraManager")
    if not objects then return nil end
    for _, object in pairs(objects) do
        if fullName(object):match("^CameraManager%s") then return object end
    end
    return nil
end

local function discover()
    local tick = os.clock() * 1000
    if tick - lastDiscoveryAt < DISCOVERY_INTERVAL_MS and
       valid(cached.camera) and valid(cached.manager) and valid(cached.pcm) then return end
    lastDiscoveryAt = tick

    if not valid(cached.camera) then cached.camera = findCamera() end
    if not valid(cached.manager) then cached.manager = findManager() end
    if not valid(cached.pcm) then cached.pcm = FindFirstOf("PlayerCameraManager") end

    if valid(cached.manager) then
        local camera = safeObject(function() return cached.manager.CameraComponent end)
        if not valid(camera) then camera = safeObject(function() return cached.manager.Camera end) end
        if valid(camera) then cached.camera = camera end

        local pcm = safeObject(function() return cached.manager.PlayerCameraManager end)
        if not valid(pcm) then pcm = safeObject(function() return cached.manager.PCM end) end
        if valid(pcm) then cached.pcm = pcm end
    end
end

local function readCamera()
    return {
        path = fullName(cached.camera),
        fov = safeValue(function() return cached.camera.FieldOfView end),
        firstPersonFov = safeValue(function() return cached.camera.FirstPersonFieldOfView end),
        aspect = safeValue(function() return cached.camera.AspectRatio end),
        axis = safeValue(function() return cached.camera.AspectRatioAxisConstraint end),
        constrain = safeValue(function() return cached.camera.bConstrainAspectRatio end),
        overrideAxis = safeValue(function() return cached.camera.bOverrideAspectRatioAxisConstraint end),
    }
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

local function readPcm()
    if not valid(cached.pcm) then
        return { path = "<none>", target = "<none>", current = readPov(nil), last = readPov(nil), viewTarget = readPov(nil) }
    end
    return {
        path = fullName(cached.pcm),
        target = safeValue(function() return cached.pcm.ViewTarget.Target:GetFullName() end),
        current = readPov(safeObject(function() return cached.pcm.CameraCachePrivate.POV end)),
        last = readPov(safeObject(function() return cached.pcm.LastFrameCameraCachePrivate.POV end)),
        viewTarget = readPov(safeObject(function() return cached.pcm.ViewTarget.POV end)),
    }
end

local function readManager()
    if not valid(cached.manager) then return { path = "<none>", camera = "<none>", pcm = "<none>" } end
    return {
        path = fullName(cached.manager),
        camera = safeValue(function() return cached.manager.CameraComponent:GetFullName() end),
        pcm = safeValue(function() return cached.manager.PlayerCameraManager:GetFullName() end),
    }
end

local function stateKey(camera, manager, pcm)
    return table.concat({
        camera.path, manager.path, manager.camera, manager.pcm, pcm.path, pcm.target,
        camera.fov, camera.firstPersonFov, camera.aspect, camera.axis,
        camera.constrain, camera.overrideAxis,
        pcm.current.fov, pcm.current.firstPersonFov, pcm.current.aspect, pcm.current.constrain,
        pcm.last.fov, pcm.last.firstPersonFov, pcm.last.aspect, pcm.last.constrain,
        pcm.viewTarget.fov, pcm.viewTarget.firstPersonFov, pcm.viewTarget.aspect, pcm.viewTarget.constrain,
    }, "|")
end

local function ownershipKey(camera, manager, pcm)
    return table.concat({ camera.path, manager.path, manager.camera, manager.pcm, pcm.path }, "|")
end

local function emitDeltas(previous, current, source)
    if not previous then return end
    local fields = {
        { "CameraComponent.FieldOfView", previous.camera.fov, current.camera.fov },
        { "CameraComponent.FirstPersonFieldOfView", previous.camera.firstPersonFov, current.camera.firstPersonFov },
        { "CameraComponent.AspectRatio", previous.camera.aspect, current.camera.aspect },
        { "CameraComponent.AspectRatioAxisConstraint", previous.camera.axis, current.camera.axis },
        { "CameraComponent.bConstrainAspectRatio", previous.camera.constrain, current.camera.constrain },
        { "CameraComponent.bOverrideAspectRatioAxisConstraint", previous.camera.overrideAxis, current.camera.overrideAxis },
        { "PCM.Current.FOV", previous.pcm.current.fov, current.pcm.current.fov },
        { "PCM.Current.AspectRatio", previous.pcm.current.aspect, current.pcm.current.aspect },
        { "PCM.Current.Constrain", previous.pcm.current.constrain, current.pcm.current.constrain },
        { "PCM.LastFrame.FOV", previous.pcm.last.fov, current.pcm.last.fov },
        { "PCM.LastFrame.AspectRatio", previous.pcm.last.aspect, current.pcm.last.aspect },
        { "PCM.LastFrame.Constrain", previous.pcm.last.constrain, current.pcm.last.constrain },
        { "PCM.ViewTarget.FOV", previous.pcm.viewTarget.fov, current.pcm.viewTarget.fov },
        { "PCM.ViewTarget.AspectRatio", previous.pcm.viewTarget.aspect, current.pcm.viewTarget.aspect },
        { "PCM.ViewTarget.Constrain", previous.pcm.viewTarget.constrain, current.pcm.viewTarget.constrain },
    }
    for _, field in ipairs(fields) do
        if field[2] ~= field[3] then
            emit("DELTA seq=" .. tostring(sequence) .. " time=" .. now() ..
                " source=" .. source .. " field=" .. field[1] ..
                " from=" .. field[2] .. " to=" .. field[3])
        end
    end
end

local function snapshot(reason, force)
    discover()
    local camera = readCamera()
    local manager = readManager()
    local pcm = readPcm()
    local ownership = ownershipKey(camera, manager, pcm)
    if ownership ~= lastOwnership then
        emit("OWNERSHIP " .. (lastOwnership and "CHANGED" or "INITIAL") ..
            " time=" .. now() .. " value=" .. ownership)
        lastOwnership = ownership
        lastState = nil
    end

    local current = { camera = camera, manager = manager, pcm = pcm }
    local key = stateKey(camera, manager, pcm)
    if not force and key == lastState then return end
    sequence = sequence + 1
    emitDeltas(previousSnapshot, current, reason)
    emit("STATE seq=" .. tostring(sequence) .. " time=" .. now() .. " reason=" .. reason ..
        " Camera=" .. camera.fov .. "/" .. camera.aspect .. "/" .. camera.constrain ..
        " PCMCurrent=" .. pcm.current.fov .. "/" .. pcm.current.aspect .. "/" .. pcm.current.constrain ..
        " PCMLast=" .. pcm.last.fov .. "/" .. pcm.last.aspect .. "/" .. pcm.last.constrain)
    previousSnapshot = current
    lastState = key
end

RegisterKeyBind(0x23, function() snapshot("manual", true) end)
emit("loaded; read-only; End writes a marker only; delta observer active")
LoopAsync(SAMPLE_INTERVAL_MS, function()
    snapshot("sample", false)
    return false
end)
