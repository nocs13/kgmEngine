Gui = {}
Gui.__index = Gui

function Gui:new(u)
  local g = {}
  setmetatable(g, Gui)

  o = kgmGuiContainer(400, 400)
  g.o = o

  return g
end

function Gui:align(s)
end

function Gui:addList(w, handler)
end

function Gui:addText(w, handler)
end

function Gui:addLabel(w, text)
end

function Gui:addCheck(w, text, status, handler)
end

function Gui:addButton(w, text, handler)
end

function Gui:addProgress(w, text, handler)
end
