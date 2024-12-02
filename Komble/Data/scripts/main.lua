kgmImport('vector3')
kgmImport('camera')
kgmImport('gambit')
kgmImport('unit')
kgmImport('player')
kgmImport('guis')
kgmImport('keys')
kgmImport('light')
kgmImport('sky')

utypes = {}

retent = nil

mapId  = ''
maps   = {}

units  = {}
iunit  = 0


cam = nil
player = nil

state = 0

inputkey = 0

State_Idle  = 0
State_Quit  = 1
State_Play  = 2
State_Load  = 3
State_Clean = 4
State_Pause = 5
State_Edit  = 6

local function haveKey(table, key)
  return table[key] ~= nil
end

function kgm_log(s)
  kgmLog(s)
end

function main_quit()
  kgm_log('Quiting...')
  kgmGameExit()
end

function main_resume()
  kgm_log('Resuming...')
  state = kgmGameState()

  if state ~= State_Pause then
    kgm_log('Invalid state to resume.')
  end

  return kgmGamePlay()
end

function main_unload()
  kgm_log('Unloading...')
  iunit = 0
  return kgmGameUnload()
end

function main_load_new()
  kgm_log('Loading new...')
  kgmGameUnload()
  return kgmGameLoad(maps[1])
end

function main_load_test()
  kgm_log('Loading test...')

  kgmGameUnload()
  guiShowMain(false)

  p = player_generate()

  if p ~= nil then
    table.insert(units, player)
  end

  s = sky_generate()

  if s ~= nil then
    s:setCamera(player:getCamera())
    table.insert(units, s)
  end

  l = light_generate()

  if l ~= nil then
    table.insert(units, l)
  end

  kgmGcSetBgColor(0.5, 0.6, 0.7)

  return kgmGameSetState(State_Play)
end

function main_oninit()
  kgm_log('main init')

  guis_init()

  --maps[1] = 'map00.map'
  maps[1] = 'map_01.map'
  maps[2] = 'map01.map'
  maps[3] = 'map02.map'
  maps[4] = 'map03.map'
  maps[5] = 'map04.map'
  maps[6] = 'map05.map'

  retent = kgmGenRetention('.komble')

  mapId = kgmGetRetention(retent, 'mapCurrent')

  if mapId == '' then
    kgmSetRetention(retent, 'mapCurrent', 'map00')
  end

  utypes['Unit'] = Unit_assign
  utypes['Light'] = Light_assign
end

function main_onfree()
  kgmDelRetention(retent)
end

function main_onplay()
  --if player ~= nil then
  --  player:input()
  --end

  for _, u in ipairs(units) do
    u:onPlay()
  end

  kgm_log("lua on play.");
end

function main_onpause()
  guiShowPause()
  kgm_log("lua on pause.");
end

function main_onload()
  kgm_log('On game map load')

  cam = Camera:new(kgmGameCamera())

  player = Player:new(nil, nil)

  player:setCamera(cam)

  table.insert(units, player)

  s = nil --sky_generate()

  if s ~= nil then
    s:setCamera(player:getCamera())
    table.insert(units, s)
  end

  kgm_log('Switch to play state')

  state = State_Play
end

function main_onunload()
  kgm_log('On game map unload, clearing map level data')

  units  = {}
  player = nil
  cam    = nil

  iunit = 0

  kgm_log('Switch to idle state')
  state = State_Idle

  kgmGcSetBgColor(0.0, 0.0, 0.0)
end

function main_oninsert(u)
  kgm_log('Insert unit ...')
  if u ~= nil then
    iunit = iunit + 1

    nm = kgmUnitName(u)
    cl = kgmUnitType(u)

    units[iunit] = Unit_assign(u)

    kgm_log('Insert unit name: ' .. nm .. ' type: ' .. cl)
  end
end

function main_onremove(u)
  if u == nil then
    return
  end

  for i = 1, iunit do
    if units[i] == u then
      units[i] = nil
      table.remove(units, i)
    end
  end
end

