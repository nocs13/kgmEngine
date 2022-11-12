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

function Player:input()
  if kgmKeyState(KEY_UP) == 1 then
  elseif kgmKeyState(KEY_DOWN) == 1 then
  elseif kgmKeyState(KEY_LEFT) == 1 then
  elseif kgmKeyState(KEY_RIGHT) == 1 then
  end
end
