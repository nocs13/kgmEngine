Camera = {}
Camera.__index = Camera

function Camera:new(c)
  local obj = {}
  setmetatable(obj, Camera)

  obj.cam = c

  x, y, z = kgmCamPosition(c)

  obj.pos = Vector3:new(x, y, z)

  x, y, z = kgmCamDirection(c)

  obj.dir = Vector3:new(x, y, z)

  return obj
end

function Camera:update(t)

end

function Camera:turn(d)
  a = 0.01

  x = d * mathf.cos(a)
  y = d * mathf.sin(a)

  self.dir:add(x, y, 0)
  self.dir = self.dir:nor()

  kgmCamLook(self.cam, self.dir.x, self.dir.y, self.dir.z)
end

function Camera:move()
  l = 0.5
  v = self.dir
  self.pos:add(v.x * l, v.y * l, v.z * l)

  kgmCamMove(self.cam, self.pos.x, self.pos.y, self.pos.z)
end
