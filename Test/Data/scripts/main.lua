gui_main = nil

function main_init()
  gui_main = kgmGuiLoad('main.gui')
end

function main_free()
end

function main_update()
end

function main_log()
   kgmLog('hello from lua')
end

function main_quit()
   kgmGameExit()
end
