kgmImport('gui')
kgmImport('guifd')
kgmImport('guimenu')

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

  gui_main = Gui:new(300, 240)
  gui_main:addEmpty(1)
  gui_main:addButton('idNew', 3, 'New', 'onGuiButtonNew')
  gui_main:addEmpty(1)

  gui_main:addEmpty(1)
  gui_main:addButton('idLoad', 3, 'Load', 'onGuiButtonLoad')
  gui_main:addEmpty(1)

  gui_main:addEmpty(1)
  gui_main:addButton('idTest', 3, 'Test', 'onGuiButtonTest')
  gui_main:addEmpty(1)

  gui_main:addEmpty(1)
  gui_main:addButton('idQuit', 3, 'Quit', 'onGuiButtonQuit')
  gui_main:addEmpty(1)

  gui_main:addEmpty(5)

  gui_main:addEmpty(1)
  gui_main:addButton('idCredits', 3, 'Credits', 'onGuiButtonCredits')
  gui_main:addEmpty(1)

  gui_pause = Gui:new(100, 120)
  gui_pause:addButton('idBtnResume', 5, 'Resume', 'onGuiButtonResume')
  gui_pause:addButton('idBtnMain', 5, 'Main', 'onGuiButtonMain')
  gui_pause:addButton('idBtnQuit', 5, 'Quit', 'onGuiButtonQuit')
  gui_pause:hide()

  gui_credits = Gui:new(200, 280)
  gui_credits:addLabel('idCredCredits', 2, 'Credits')
  gui_credits:addEmpty(3)

  gui_credits:addLabel('idCredDeveloper', 2, 'Developer')
  gui_credits:addLabel('idCredDeveloperAuth', 3 , 'Karakal_13')
  gui_credits:addLabel('idCredDesigner', 2, 'Designer')
  gui_credits:addLabel('idCredDesignerAuth', 3 , 'Karakal_13')
  gui_credits:addLabel('idCredVideo', 2, 'Video')
  gui_credits:addLabel('idCredVideoAuth', 3 , 'Karakal_13')
  gui_credits:addLabel('idCredAudio', 2, 'Audio')
  gui_credits:addLabel('idCredAudioAuth', 3, 'Karakal_13')
  gui_credits:addEmpty(5)
  gui_credits:addEmpty(2)
  gui_credits:addButton('idBtnBack', 1, 'Back', 'onGuiCreditsBack')
  gui_credits:addEmpty(2)
  gui_credits:hide()
end

function onGuiButtonQuit()
    kgmGameExit()
end

function onGuiButtonNew()
  kgm_log('onGuiButtonNew')

  state = main_load_new()

  if state ~= 0 then
    gui_main:hide()
  end
end

function onGuiButtonLoad()
end

function onGuiButtonTest()
    kgm_log('onGuiButtonNew')

    state = main_load_test()

    if state ~= 0 then
        gui_main:hide()
    end
end

function onGuiButtonCredits()
  kgm_log('onGuiButtonCredits')

  gui_main:hide()
  gui_credits:show()
end

function onGuiCreditsBack()
  kgm_log('onGuiCreditsBack')

  gui_credits:hide()
  gui_main:show()
end

function guiShowPause()
  gui_pause:show()
end

function onGuiButtonResume()
  stat = main_resume()

  if stas ~= 0 then
    gui_pause:hide()
  end
end

function onGuiButtonMain()
  stat = main_unload()

  if stas ~= 0 then
    gui_pause:hide()
    gui_main:show()
  end
end
