Unit = {u=nil, c=nil}
Unit.__index = Unit

function Unit:new(u)
  local s = {}
  setmetatable(s, Unit)

  s.u = u or nil

  return s
end

function Unit:setUnit(u)
  self.u = u
end

function Unit:onPlay()
  kgm_log('Unit got on play message!')
end

function Unit_assign(u)
  if u == nile then
    return nil
  end

  o = Unit:new(u)

  return o
end
