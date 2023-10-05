--Player = {u=nil, c=nil}
--Player.__index = Player

--function Player:new(u)
--  local p = {}
--  setmetatable(p, Player)

--  p.u = u or nil
--  p.c = nil

--  return p
--end
Player = Unit:new()

function Player:new(u)
  local s = Unit:new(u)
  setmetatable(s, self)
  self.__index = self

  return s
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

function Player:onPlay()
  kgm_log('Player got on play message!')
  self:input()
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
  if (kgmKeyState(KEY_J) == 1) then
    self.c:jump(-1)
    kgm_log("jump down.");
  end
  if (kgmKeyState(KEY_L) == 1) then
    self.c:jump(1)
    kgm_log("jump up.");
  end
end

function player_generate()
  p = kgmUnitCreate('Player')

  if p == nil then
    kgm_log('Failed player unit create!')
    return nil
  end

  cam = Camera:new(kgmGameCamera())

  player = Player:new(nil)

  player:setUnit(p)
  player:setCamera(cam)

  return player
end
