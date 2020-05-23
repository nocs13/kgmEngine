Camera = settag({}, newtag())

settagmethod(tag(Camera), "index", function(t, f) return %Camera[f] end)

angle = 0.0

function Camera:new(c)
  x, y, z = kgmCamPosition(c)
  
  local p = Vector3:new(x, y, z)

  x, y, z = kgmCamDirection(c)

  local d = Vector3:new(x, y, z)

  local obj = {cam = c, pos = p, dir = d}

  settag(obj, tag(Camera))

  return obj
end

function Camera:turn(d)
  local a = 0.01

  angle = angle + (d * a)

  local x = cos(angle)
  local y = sin(angle)

  self.dir:add(x, y, 0)

  local l = self.dir:length()

  self.dir:normalize()

  local n = self.dir

  kgmCamLook(self.cam, n.x, n.y, n.z)
end

function Camera:move(d)
  local l = 0.005 * d

  n = self.dir:nor()

  local x = n.x * l
  local y = n.y * l

  self.pos:add(x, y, 0.0)

  kgmCamMove(self.cam, self.pos.x, self.pos.y, self.pos.z)
end
