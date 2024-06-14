Player = {}
Player.__index = Player

function Player:new(u)
  local p = {}
  setmetatable(p, Player)

  p.u = u

  return p
end

function Player:state(s)
  self.s = s
end
