gui_main = nil

function guis_init()
  local sw, sh = 1, 1
  local gw, gh = 1, 1
  local scw, sch = 1, 1

  gui_main = kgmGuiLoad('guis/main.gui')
  kgmLog('main gui is ' .. tostring(gui_main))

  sw, sh = kgmScreenResolution()
  kgmLog('screen resolution is ' .. tostring(sw) .. ' ' .. tostring(sh))
  gw, gh = kgmGuiResolution(gui_main)
  kgmLog('gui resolution is ' .. tostring(gw) .. ' ' .. tostring(gh))

  scw = sw / gw
  sch = sh / gh

  kgmLog('gui scaling is ' .. tostring(scw) .. ' ' .. tostring(sch))

  kgmGuiResize(gui_main, sw, sh)
  kgmGuiShow(gui_main, 0)
  kgmLog('main gui 2 is ' .. tostring(gui_main))
end

