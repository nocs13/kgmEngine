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
  if (kgmKeyState(KEY_UP) == 1) or (kgmKeyState(KEY_W) == 1) then
    self.c:move(1)
    kgm_log("move forward.");
  end
  if (kgmKeyState(KEY_DOWN) == 1) or (kgmKeyState(KEY_S) == 1) then
    self.c:move(-1)
    kgm_log("move backward.");
  end
  if (kgmKeyState(KEY_LEFT) == 1)  or (kgmKeyState(KEY_A) == 1) then
    self.c:turn(1)
    kgm_log("turn left.");
  end
  if (kgmKeyState(KEY_RIGHT) == 1)  or (kgmKeyState(KEY_D) == 1) then
    self.c:turn(-1)
    kgm_log("turn right.");
  end
end
