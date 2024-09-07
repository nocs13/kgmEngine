--Gui = settag({}, newtag())
--settagmethod(tag(Gui), "index", function(t, f) return %Gui[f] end)
Gui = {o=nil}

function Gui:new(w, h)
  local g = { o = nil }

  --settag(g, tag(Gui))

  g.o = kgmGuiCreate(w, h)
  kgmGuiSetWindow(g.o)

  setmetatable(g, self)
  self.__index = self

  return g
end

function Gui:show()
  kgmGuiShow(self.o, 1)
end

function Gui:hide()
  kgmGuiShow(self.o, 0)
end

function Gui:addEmpty(w)
  kgmGuiAdd(self.o, '', '', w)
end

function Gui:addList(id, w, handler)
end

function Gui:addText(id, w, handler)
end

function Gui:addLabel(id, w, text)
  kgmGuiAdd(self.o, 'Label', id, w)
  kgmGuiSetText(self.o, id, text)
end

function Gui:addCheck(id, w, text, status, handler)
  kgmGuiAdd(self.o, 'Check', id, w)
  kgmGuiSetText(self.o, id, text)
end

function Gui:addButton(id, w, text, handler)
  c = kgmGuiAdd(self.o, 'Button', id, w)
  kgmGuiSetText(self.o, id, text)

  if c ~= nil then
    kgmGuiCallback(c, handler)
  end
end

function Gui:addProgress(id, w, handler)
  kgmGuiAdd(self.o, 'Progress', id, w)
end