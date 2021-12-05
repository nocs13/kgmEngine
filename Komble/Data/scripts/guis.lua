VALIGN_LEFT   = 1
VALIGN_RIGHT  = 2
VALIGN_CENTER = 3

gui_main = nil
gui_pause = nil
gui_credits = nil

function guis_init()
  local sw, sh = 1, 1
  local gw, gh = 1, 1
  local scw, sch = 1, 1

  gui_main = kgmGuiLoad('guis/start.ui')
  kgm_log('main gui is ' .. tostring(gui_main))

  --sw, sh = kgmScreenResolution()
  --kgmLog('screen resolution is ' .. tostring(sw) .. ' ' .. tostring(sh))
  --gw, gh = kgmGuiResolution(gui_main)
  --kgmLog('gui resolution is ' .. tostring(gw) .. ' ' .. tostring(gh))

  --scw = sw / gw
  --sch = sh / gh

  --kgmLog('gui scaling is ' .. tostring(scw) .. ' ' .. tostring(sch))

  --kgmGuiResize(gui_main, sw, sh)
  --gui_quit = kgmGuiGetChild(gui_main, 'button2')

  kgmGuiSetVAlign(gui_main, 3);
  kgmGuiSetHAlign(gui_main, 3);
  kgmGuiShow(gui_main, 1)
  kgm_log('main gui 2 is ' .. tostring(gui_main))
end

function onGuiButtonQuit()
    kgmGameExit()
end

function onGuiButtonNew()
  kgm_log('onGuiButtonNew')

  state = main_load_new()

  if state ~= 0 then
    kgmGuiShow(gui_main, 0)
  end
end

function onGuiButtonLoad()
end

function onGuiButtonTest()
    kgm_log('onGuiButtonNew')

    state = main_load_test()

    if state ~= 0 then
        kgmGuiShow(gui_main, 0)
    end
end

function onGuiButtonCredits()
  kgm_log('onGuiButtonCredits')

  if gui_credits == nil then
    gui_credits = kgmGuiLoad('guis/credits.ui')
    kgmGuiSetVAlign(gui_credits, VALIGN_CENTER);
    kgmGuiSetHAlign(gui_credits, VALIGN_CENTER);
  end

  kgmGuiShow(gui_main, 0)
  kgmGuiShow(gui_credits, 1)
end

function onGuiCreditsBack()
  kgm_log('onGuiCreditsBack')

  kgmGuiShow(gui_credits, 0)
  kgmGuiShow(gui_main, 1)
end

function guiShowPause()
  if gui_pause == nil then
    gui_pause = kgmGuiLoad('guis/pause.ui')
    kgmGuiSetVAlign(gui_pause, VALIGN_CENTER);
    kgmGuiSetHAlign(gui_pause, VALIGN_CENTER);
  end

  kgmGuiShow(gui_pause, 1)
end

function onGuiButtonResume()
  stat = main_resume()

  if stas ~= 0 then
    kgmGuiShow(gui_pause, 0)
  end
end

function onGuiButtonMain()
  stat = main_unload()

  if stas ~= 0 then
    kgmGuiShow(gui_pause, 0)
    kgmGuiShow(gui_main, 1)
  end
end
