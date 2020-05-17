kgmImport('vector3')
kgmImport('camera')
kgmImport('player')
kgmImport('guis')

retent = nil

mapId  = ''
maps   = {}

units  = {}
iunit  = 0

player = nil

cam = nil

state = 0

inputkey = 0

KEY_ESCAPE = 1
KEY_0 = 2
KEY_1 = 3
KEY_2 = 4
KEY_3 = 5
KEY_UP    = 76
KEY_LEFT  = 77
KEY_RIGHT = 78
KEY_DOWN  = 79

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

function main_onupdate()
  kgm_log('Update state is ' .. tostring(state))
  if state == State_Play then
    kgm_log('Input key is ' .. tostring(inputkey))
    if inputkey == Key_Left and cam ~= nil then
      cam:turn(-1)
	  kgm_log('Camera direction ' .. tostring(cam.pos.x) .. ' ' .. tostring(cam.pos.y) .. ' ' .. tostring(cam.pos.z))
    elseif inputkey == Key_Right and cam ~= nil then
      cam:turn(1)
    elseif inputkey == Key_Up and cam ~= nil then
      cam:move()
    end
  end
end

function main_onload()
  c = kgmGameCamera()

  if c ~= nil then
    cam = Camera:new(c)
  else
    kgm_log('No main camera found')
  end

  state = State_Play
end

function main_onunload()
  units = {}
  player = nil

  cam = nil

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
  kgm_log('got input event')

  if key == KEY_ESCAPE and down == 0 then
    state = kgmGameState()
    kgm_log('Game state: ' .. tostring(state))

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
    kgm_log('Set key ' .. tostring(key))
    inputkey = key
  elseif down == 0 then
    inputkey = 0
  end
end
