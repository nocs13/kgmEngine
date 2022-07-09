--Vector3 = settag({}, newtag())
--settagmethod(tag(Vector3), "index", function(t, f) return %Vector3[f] end)

Vector3 = {x=0, y=0, z=0}

function Vector3:new(x, y, z)
  local obj = {x=x, y=y, z=z}
  
  --settag(obj, tag(Vector3))
  setmetatable(obj, self)
  self.__index = self
  return obj
end

function Vector3:add(x, y, z)
  self.x = self.x + x
  self.y = self.z + y
  self.z = self.z + z
end

function Vector3:mul(a)
  self.x = self.x * a
  self.y = self.y * a
  self.z = self.z * a
end

function Vector3:length()
  a = self.x * self.x
  b = self.y * self.y
  c = self.z * self.z

  return math.sqrt(a + b + c)
end

function Vector3:nor()
  l = self:length()

  if l == 0.0 then
    return Vector3:new(0, 0, 0)
  end

  a = self.x / l
  b = self.y / l
  c = self.z / l

  return Vector3:new(a, b, c)
end

function Vector3:normalize()
  l = self:length()

  if l < 0.0000000001 then
    return
  end

  self.x = self.x / l
  self.y = self.y / l
  self.z = self.z / l
end
