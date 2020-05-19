--Camera = {}
Camera = settag({}, newtag())

settagmethod(tag(Camera), "index", function(t, f) return %Camera[f] end)

function Camera:new(c)
  kgm_log('Inpit camera is ' .. tostring(c))
  
  x, y, z = kgmCamPosition(c)
  
  kgm_log('Camera start position is ' .. tostring(x) .. ' ' .. tostring(y) .. ' ' .. tostring(z))

  p = Vector3:new(x, y, z)

  x, y, z = kgmCamDirection(c)

  kgm_log('Camera start direction is ' .. tostring(x) .. ' ' .. tostring(y) .. ' ' .. tostring(z))

  d = Vector3:new(x, y, z)

  local obj = {cam = c, pos = p, dir = d}

  settag(obj, tag(Camera))

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
