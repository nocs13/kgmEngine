local class_mt = {}

function class_mt: __index(key)
    return self.__baseclass[key]
end

class = setmetatable({ __baseclass = {} }, class_mt)

function class:new(...)
    local c = {}
    c.__baseclass = self
    setmetatable(c, getmetatable(self))
    if c.init then
        c.init(...)
    end
end