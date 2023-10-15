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

  local obj = { cam = c, pos = p, dir = d, state = 0, angle = 0.0 }

  --settag(obj, tag(Camera))
  setmetatable(obj, self)
  self.__index = self

  return obj
end

function Camera:turn(d)
  local a = 0.1

  self.angle = self.angle + (d * a)

  local x = math.cos(self.angle)
  local y = math.sin(self.angle)

  dir = Vector3:new(x, y, 0)

  local l = dir:length()

  dir:normalize()

  local n = dir

  self.dir = dir

  kgmCamLook(self.cam, n.x, n.y, n.z)
end

function Camera:move(d)
  local l = 1 * d

  n = self.dir:nor()

  local x = n.x * l
  local y = n.y * l

  self.pos:add(x, y, 0.0)

  --p = Vector3:new(kgmCamPosition())
  --d = Vector3:new(kgmCamDirection())

  --d:mul(2)
  --p:vadd(d)

  kgmCamMove(self.cam, self.pos.x, self.pos.y, self.pos.z)
  --kgmCamMove(self.cam, p.x, p.y, p.z)
end

function Camera:jump(d)
  local l = 1 * d

  self.pos:add(0.0, 0.0, l)

  kgmCamMove(self.cam, self.pos.x, self.pos.y, self.pos.z)
end
