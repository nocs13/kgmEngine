kgmImport('vector3')
kgmImport('camera')
kgmImport('player')
kgmImport('guis')
kgmImport('keys')

retent = nil

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

function main_resume()
  state = kgmGameState()

  if state ~= State_Pause then
    kgm_log('Invalid state to resume.')
  end

  return kgmGamePlay()
end

function main_unload()
  state = kgmGameState()

  if state ~= State_Play and state ~= State_Pause then
    kgm_log('Invalid state to unload.')
  end

  if state == State_Play then
    kgm_log('Pause game before unload.')

    kgmGamePause()
  end

  return kgmGameUnload()
end

function main_load_new()
  state = kgmGameState()

  if state ~= State_Idle then
    kgm_log('Invalid state to load map.')

    return 0
  end

  kgm_log('Loading first map.')

  state = kgmGameLoad(maps[1])

  if state ~= State_Play then
    state = 0
  end

  return state
end

function main_load_test()
  kgm_log('Loading test mode.')

  kgmGameUnload()
  kgmGameLoad('EMPTY')

  state = kgmGameState()

  return state
end

function main_oninit()
  kgm_log('main init')

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

function main_onfree()
  kgmDelRetention(retent)
end

function main_onplay()
  if cam ~= nil then
    cam:update()
  end

  kgm_log("lua on play.");
end

function main_onload()
  kgm_log('On game map load')

  c = kgmGameCamera()

  if c ~= nil then
    kgm_log('Map have camera')
    cam = Camera:new(c)
  end

  it = kgmUnitIterator()

  if it ~= nil then
    while 1 do
      u = nil
      u = kgmUnitIterNext(it)

      if not u then
        break
      end

      table.insert(units, u)
    end
  end

  kgmUnitIterFree(it);

  it = nil

  kgm_log('Switch to play state')
  state = State_Play
end

function main_onunload()
  kgm_log('On game map unload, clearing map level data')

  units  = {}
  player = nil
  cam    = nil

  kgm_log('Switch to idle state')
  state = State_Idle
end

function main_oninsert(u)
  if u ~= nil then
    iunit = iunit + 1
    units[iunit] = u

    nm = kgmUnitName(u)
    cl = kgmUnitClass(u)

    kgm_log('Insert unit name: ' .. nm .. ' class: ' .. cl)
  end
end

function main_onremove(u)
  if u == nil then
    return
  end

  for i = 1, iunit do
    if units[i] == u then
      units[i] = nil
    end
  end
end

function main_onbutton(key, btn, down)
  state = kgmGameState()

  kgm_log('got input event')
  kgm_log('Game state is ' .. tostring(state))

  if key == KEY_ESCAPE and down == 0 then
    if state == State_Play then
      kgm_log('State is play')

      kgmGamePause()

      state = kgmGameState()

      if state == State_Pause then
        kgm_log('gui show pause')
        guiShowPause()
      end
    end
  end

  if down ~= 0 then
    inputkey = key
  elseif down == 0 then
    inputkey = 0
  end

  kgm_log('Setting camera state depend of input.')

  if cam ~= nil and state == State_Play then
    kgm_log('Have camera.')
    cam:button(key, down)
  end
end
