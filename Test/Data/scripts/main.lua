kgmImport('guis')

function main_init()
  guis_init()
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
