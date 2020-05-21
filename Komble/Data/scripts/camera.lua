Camera = settag({}, newtag())

settagmethod(tag(Camera), "index", function(t, f) return %Camera[f] end)

function Camera:new(c)
  x, y, z = kgmCamPosition(c)
  
  p = Vector3:new(x, y, z)

  x, y, z = kgmCamDirection(c)

  d = Vector3:new(x, y, z)

  local obj = {cam = c, pos = p, dir = d}

  settag(obj, tag(Camera))

  return obj
end

function Camera:update(t)

end

function Camera:turn(d)
  a = 0.01

  x = d * cos(a)
  y = d * sin(a)

  kgm_log('Turn vector is ' .. tostring(x) .. ' ' .. tostring(y))

  self.dir:add(x, y, 0)
    
  kgm_log('Turn add is ' .. tostring(self.dir.x) .. ' ' .. tostring(self.dir.y) .. ' ' .. tostring(self.dir.z))

  self.dir = self.dir:nor()
  
  kgm_log('Turn nor is ' .. tostring(self.dir.x) .. ' ' .. tostring(self.dir.y) .. ' ' .. tostring(self.dir.z))

  kgmCamLook(self.cam, self.dir.x, self.dir.y, self.dir.z)
end

function Camera:move()
  l = 0.5
  v = self.dir
  self.pos:add(v.x * l, v.y * l, v.z * l)

  kgmCamMove(self.cam, self.pos.x, self.pos.y, self.pos.z)
end
