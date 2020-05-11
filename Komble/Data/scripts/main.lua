kgmImport('guis')

retent = nil
mapId  = ''
maps   = {}

function main_init()
  kgmLog('main init')

  guis_init()

  maps[1] = 'map00'
  maps[2] = 'map01'
  maps[3] = 'map02'
  maps[4] = 'map03'
  maps[5] = 'map04'

  retent = kgmGenRetention('.komble')

  mapId = kgmGetRetention(retent, 'mapCurrent')

  if mapId == '' then
    kgmSetRetention(retent, 'mapCurrent', 'map00')
  end
end

function main_free()
  kgmDelRetention(retent)
end

function main_update()
  --kgmLog('Log from lua update')
end

function main_log()
  --kgmLog('hello from lua')
end

function main_quit()
   kgmGameExit()
end

function main_button(key, btn, down)
  kgmLog('got input event')
end
