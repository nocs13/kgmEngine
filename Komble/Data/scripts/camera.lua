Camera = {}
Camera.__index = Camera

function Camera:new(c)
  local c = {}
  setmetatable(c, Camera)

  c.cam = c

  return c
end

function Camera:update(t)

end
