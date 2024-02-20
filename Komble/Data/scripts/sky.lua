--Sky = {u=nil, c=nil}
--Sky.__index = Sky

--function Sky:new(u, c)
--  local s = {}
--  setmetatable(s, Sky)

--  s.u = u or nil
--  s.c = c or nil

--  return s
--end

--function Sky:setUnit(u)
--  self.u = u
--end

Sky = Unit:new()

function Sky:new(u)
  local s = Unit:new(u)
  setmetatable(s, self)
  self.__index = self

  return s
end

function Sky:setCamera(c)
  self.c = c
end

function Sky:onPlay()
  if self.c ~= nil then
    local x, y, z = kgmCamPosition()
    kgmUnitSetPosition(u, x, y, z)
  end
  kgm_log('Sky got on play message!')
end

function sky_generate()
  u = kgmUnitCreate('Sky')

  if u == nil then
    kgm_log('Failed sky unit create!')
    return nil
  end

  mh = kgmMeshGet('sky.msh')

  if mh == nil then
    kgm_log('Failed sky mesh request!')
    kgmUnitRemove(u)
    return nil
  end

  ml = kgmMtlGet('sky')

  if ml == nil then
    kgm_log('Failed generate material!')
    kgmUnitRemove(u)
    return nil
  end

  kgmMtlSetType(ml, 0)
  kgmMtlSetCull(ml, 0)

  kgmMtlSetColor(ml, 1.0, 1.0, 1.0)

  kgmMtlSetMaps(ml, 'Sky_c.tga', '', '')

  kgmUnitSetMesh(u, mh, ml)
  kgmUnitSetBound(u, 2.0, 2.0, 2.0)
  kgmUnitSetScale(u, 100.0, 100.0, 100.0)
  kgmUnitSetPosition(u, 0.0, 0.0, 0.0)

  s = Sky:new(u)

  return s
end
