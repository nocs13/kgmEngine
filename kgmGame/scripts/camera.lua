--Camera = settag({}, newtag())
--settagmethod(tag(Camera), "index", function(t, f) return %Camera[f] end)
Camera = {c=nil}

angle = 0.0

State_TurnLeft     = 1
State_TurnRight    = 2
State_MoveForward  = 3
State_MoveBack     = 4

function Camera:new(c)
  x, y, z = kgmCamPosition(c)

  local p = Vector3:new(x, y, z)

  x, y, z = kgmCamDirection(c)

  local d = Vector3:new(x, y, z)

  local obj = {cam = c, pos = p, dir = d, state = 0}

  --settag(obj, tag(Camera))
  setmetatable(obj, self)
  self.__index = self

  return obj
end

function Camera:turn(d)
  local a = 0.5

  angle = angle + (d * a)

  local x = math.cos(angle)
  local y = math.sin(angle)

  self.dir:add(x, y, 0)

  local l = self.dir:length()

  self.dir:normalize()

  local n = self.dir

  kgmCamLook(self.cam, n.x, n.y, n.z)
end

function Camera:move(d)
  local l = 0.05 * d

  n = self.dir:nor()

  local x = n.x * l
  local y = n.y * l

  self.pos:add(x, y, 0.0)

  kgmCamMove(self.cam, self.pos.x, self.pos.y, self.pos.z)
end

function Camera:jump(d)
  local l = 0.05 * d

  self.pos:add(0.0, 0.0, l)

  kgmCamMove(self.cam, self.pos.x, self.pos.y, self.pos.z)
end