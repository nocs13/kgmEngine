--Light = {u=nil, c=nil}
--Light.__index = Light

--function Light:new(u)
--  local s = {}
--  setmetatable(s, Light)

--  s.u = u or nil

--  return s
--end

--function Light:setUnit(u)
--  self.u = u
--end

Light = Unit:new()

function Light:new(u)
  local s = Unit:new(u)
  setmetatable(s, self)
  self.__index = self

  return s
end

function Light:onPlay()
  kgm_log('Light got on play message!')
end


function Light_assign(u)
  if u == nil then
    return nil
  end

  o = Light:new(u)

  return o
end

function light_generate()
  u = kgmUnitCreate('Light')

  if u == nil then
    kgm_log('Failed light unit create!')
    return nil
  end

  l = kgmLightGet(1)

  if l == nil then
    kgm_log('Failed light request!')
    kgmUnitRemove(u)
    return nil
  end

  kgmLightSetForce(1.0);

  kgmUnitSetLight(u, l)

  o = Light:new(u)

  return o
end
