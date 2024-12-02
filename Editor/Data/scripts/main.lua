kgmImport('vector3')
kgmImport('camera')
kgmImport('player')
kgmImport('guis')
kgmImport('keys')
kgmImport('event')
kgmImport('editor')

retent = nil
gbmenu = nil
editor = nil
window = nil
eventHandler = nil

mapId  = ''
maps   = {}

units  = {}
iunit  = 0

player = nil

cam = nil

state = 0

inputkey = 0

State_Idle  = 0
State_Quit  = 1
State_Play  = 2
State_Load  = 3
State_Clean = 4
State_Pause = 5
State_Edit  = 6

function kgm_log(s)
  kgmLog(s)
end

function main_quit()
  main_close_window()
end

function main_oninit()
end

function main_onfree()
end

function main_onupdate()
end

function main_onload()
end

function main_onunload()
end

function main_oninsert(u)
end

function main_onremove(u)
end

function main_onbutton(key, btn, down)
end

function main_close_window()
  if window ~= nil then
    kgmCloseWindow(window)
  end
end

function main_open_request()
  guifd = GuiFD:new('guifd', '', 0)

  if guifd:gui() ~= nil then
    editor:guiAdd(guifd:gui())
  end
end

function main_save_request()
  guifd = GuiFD:new('guifd', '', 1)

  if guifd:gui() ~= nil then
    editor:guiAdd(guifd.o)
  end
end

function main_guifd_close()
  guifd = nil
end

function main()
  kgmLog('FUCK UUUUUUUUUUUUUUUUUUUU XXXXXXXXXXXX')
  window = kgmCreateWindow('kEditor', 800, 600)

  if window == nil then
    kgmLog('Window create failed.')
  else
    e = kCreateEditor(window)

    if e == nil then
      kgmLog('Editor create failed.')

      kgmCloseWindow(window)
    else
      editor = Editor:new(e)

      ev = kgmNewEvent('eventHandler')

      if ev ~= nil then
        eventHandler = Event:new(ev)

        editor:addHandler(ev)

        gbmenu = GuiMenu:new('gbmenu')
        gbmenu:init()
        gbmenu:show()

        editor:guiAdd(gbmenu.o)
      end

      kgmLoopWindow(window)

      editor:close()

      kgmFreeWindow(window)
    end
  end
end
