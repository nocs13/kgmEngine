Gui = settag({}, newtag())

settagmethod(tag(Gui), "index", function(t, f) return %Gui[f] end)

function Gui:new(w, h)
  local g = { o = nil }

  settag(g, tag(Gui))

  g.o = kgmGuiCreate(w, h)
  kgmGuiSetWindow(g.o)

  return g
end

function Gui:align(s)
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
