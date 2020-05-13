kgmImport('guis')

retent = nil
mapId  = ''
maps   = {}

KEY_ESCAPE = 1
KEY_0 = 2
KEY_1 = 3
KEY_2 = 4
KEY_3 = 5

State_Idle  = 0
State_Quit  = 1
State_Play  = 2
State_Load  = 3
State_Clean = 4
State_Pause = 5
State_Edit  = 6


function main_init()
  kgmLog('main init')

  guis_init()

  maps[1] = 'map00.map'
  maps[2] = 'map01.map'
  maps[3] = 'map02.map'
  maps[4] = 'map03.map'
  maps[5] = 'map04.map'

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

function main_unload()
end

function main_insert(u)
end

function main_remove(u)
end

function main_load_new()
  return kgmGameLoad(maps[1])
end

function main_button(key, btn, down)
  kgmLog('got input event')

  if key == KEY_ESCAPE and down == 0 then
    state = kgmGameState()
    kgmLog('Game state: ' .. tostring(state))

    if state == State_Play then
      kgmLog('state is play')

      kgmGamePause()

      state = kgmGameState()

      if state == State_Pause then
        kgmLog('gui show pause')
        guiShowPause()
      end
    end
  end
end
