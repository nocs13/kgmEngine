gui_main = nil

function guis_init()
  local sw, sh = 1, 1
  local gw, gh = 1, 1
  local scw, sch = 1, 1

  gui_main = kgmGuiLoad('guis/start.ui')
  kgmLog('main gui is ' .. tostring(gui_main))

  --sw, sh = kgmScreenResolution()
  --kgmLog('screen resolution is ' .. tostring(sw) .. ' ' .. tostring(sh))
  --gw, gh = kgmGuiResolution(gui_main)
  --kgmLog('gui resolution is ' .. tostring(gw) .. ' ' .. tostring(gh))

  --scw = sw / gw
  --sch = sh / gh

  --kgmLog('gui scaling is ' .. tostring(scw) .. ' ' .. tostring(sch))

  --kgmGuiResize(gui_main, sw, sh)
  gui_quit = kgmGuiGetChild(gui_main, 'button2')

  if gui_quit ~= nil then
    kgmGuiCallback(gui_quit, 'gui_evt_quit')
  end

  kgmGuiSetVAlign(gui_main, 3);
  kgmGuiSetHAlign(gui_main, 3);
  kgmGuiShow(gui_main, 1)
  kgmLog('main gui 2 is ' .. tostring(gui_main))
end

function gui_evt_quit()
  kgmGameQuit()
end

