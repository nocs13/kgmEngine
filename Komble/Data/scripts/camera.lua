Camera = settag({}, newtag())

settagmethod(tag(Camera), "index", function(t, f) return %Camera[f] end)

function Camera:new(c)
  x, y, z = kgmCamPosition(c)
  
  p = Vector3:new(x, y, z)

  x, y, z = kgmCamDirection(c)

  d = Vector3:new(x, y, z)

  local obj = {cam = c, pos = p, dir = d}

  settag(obj, tag(Camera))

  kgm_log('Camera start position is ' .. tostring(p.x) .. ' ' .. tostring(p.y) .. ' ' .. tostring(p.z))
  kgm_log('Camera start direction is ' .. tostring(d.x) .. ' ' .. tostring(d.y) .. ' ' .. tostring(d.z))

  return obj
end

function Camera:turn(d)
  local a = 0.001

  local x = d * cos(a)
  local y = d * sin(a)

  kgm_log('Turn vector is ' .. tostring(x) .. ' ' .. tostring(y))

  self.dir:add(x, y, 0)
    
  kgm_log('Turn add is ' .. tostring(self.dir.x) .. ' ' .. tostring(self.dir.y) .. ' ' .. tostring(self.dir.z))

  local n = self.dir:nor()
  
  kgm_log('Camera direction is ' .. tostring(self.dir.x) .. ' ' .. tostring(self.dir.y) .. ' ' .. tostring(self.dir.z))

  kgmCamLook(self.cam, n.x, n.y, n.z)
end

function Camera:move(d)
  kgm_log('Camera d ' .. tostring(d))

  local l = 0.005 * d

  kgm_log('Camera move length ' .. tostring(l))

  n = self.dir:nor()

  kgm_log('Camera move normal ' .. tostring(n.x) .. ' ' .. tostring(n.y) .. ' ' .. tostring(n.z))

  local x = n.x * l
  local y = n.y * l

  self.pos:add(x, y, 0.0)

  kgm_log('Camera position is ' .. tostring(self.pos.x) .. ' ' .. tostring(self.pos.y) .. ' ' .. tostring(self.pos.z))

  kgmCamMove(self.cam, self.pos.x, self.pos.y, self.pos.z)
end
