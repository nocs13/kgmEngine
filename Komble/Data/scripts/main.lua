kgmImport('guis')

retent = nil
mapId  = ''
maps   = {}

KEY_ESCAPE = 1
KEY_0 = 2
KEY_1 = 3
KEY_2 = 4
KEY_3 = 5

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
end

function main_log()
  --kgmLog('hello from lua')
end

function main_quit()
   kgmGameExit()
end

function main_load()
end

function main_load_new()
  return kgmGameLoad(maps[1])
end

function main_button(key, btn, down)
  kgmLog('got input event')

  if (key == KEY_ESCAPE and down == 0) then
    kgmGameExit()
  end
end
