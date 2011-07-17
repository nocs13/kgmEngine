Object={}

function extends(base)
 local new_class = {}
 local class_mt = { __index = new_class }

 function new_class:create()
  local newinst = {}
  setmetatable( newinst, class_mt )
  return newinst
 end

 if base then
  setmetatable( new_class, { __index = base } )
 end
 return new_class
end

Rect = {x=0, y=0, w=0, h=0}
Vector = {x=0, y=0, z=0}





