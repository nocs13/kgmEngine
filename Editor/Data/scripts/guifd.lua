GuiFD = {o = nil}

function GuiFD:new(n, f, s)
  local g = { o = nil }

  g.o = kgmGuiFileDialogCreate(n, f, s)

  setmetatable(g, self)
  self.__index = self

  return g
end

function GuiFD:gui()
  return self.o
end

function GuiFD:show()
  kgmGuiShow(self.o, 1)
end

function GuiFD:onClose()
  kgmLog('GuiFD Close.')
  main_guifd_close()
end

function GuiFD:onFail()
  kgmLog('GuiFD Fail.')
end

function GuiFD:onOpen(file)
  kgmLog('GuiFD Open: ' .. file)
end

function GuiFD:onSave(file)
  kgmLog('GuiFD Save: ' .. file)
end

