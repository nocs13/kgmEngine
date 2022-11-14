Player = {u=nil, c=nil}
Player.__index = Player

function Player:new(u)
  local p = {}
  setmetatable(p, Player)

  p.u = u or nil
  p.c = nil

  return p
end

function Player:state(s)
  self.s = s
end

function Player:setUnit(u)
  self.u = u
end

function Player:getUnit()
  return self.u
end

function Player:setCamera(c)
  self.c = c
end

function Player:getCamera(c)
  return self.c
end

function Player:input()
  if kgmKeyState(KEY_UP) == 1 then
    self.c:move(1)
  elseif kgmKeyState(KEY_DOWN) == 1 then
    self.c:move(-1)
  elseif kgmKeyState(KEY_LEFT) == 1 then
    self.c:turn(1)
  elseif kgmKeyState(KEY_RIGHT) == 1 then
    self.c:turn(-1)
  end
end
