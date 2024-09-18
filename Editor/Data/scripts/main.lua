kgmImport('vector3')
kgmImport('camera')
kgmImport('player')
kgmImport('guis')
kgmImport('keys')
kgmImport('event')

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
   kgmGameExit()
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

function main()
  kgmLog('FUCK UUUUUUUUUUUUUUUUUUUU XXXXXXXXXXXX')
  window = kgmCreateWindow('kEditor', 800, 600)

  if window == nil then
    kgmLog('Window create failed.')
  else
    editor = kCreateEditor(window)

    if editor == nil then
      kgmLog('Editor create failed.')

      kgmCloseWindow(window)
    else
      ev = kgmNewEvent('eventHandler')

      if ev ~= nil then
        eventHandler = Event:new(ev)
        kEditorAddHandler(editor, ev)

        gbmenu = GuiMenu:new('gbmenu')
        gbmenu:init()
        gbmenu:show()
        kEditorGuiAdd(editor, gbmenu.o)
      end

      kgmLoopWindow(window)

      kCloseEditor(editor)

      kgmFreeWindow(window)
    end
  end
end
