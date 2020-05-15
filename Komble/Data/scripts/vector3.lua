Vector3 = {}
Vector3.__index = Vector3

function Vector3:new(x, y, z)
  local v = {}
  setmetatable(v, Vector3)
  v.x = x
  v.y = y
  v.z = z

  return v
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

function Vector3:len(a)
  a = self.x * self.x
  b = self.y * self.y
  c = self.z * self.z

  return math.sqrt(a + b + c)
end

function Vector3:nor()
  l = self:len()

  if l == 0.0 then
    return Vector3:new(0, 0, 0)
  end

  a = self.x / l
  b = self.y / l
  c = self.z / l

  return Vector3:new(a, b, c)
end
