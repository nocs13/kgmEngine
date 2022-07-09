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

function main_oninit()
end

function main_onfree()
end

function main_onupdate()
end

function main_onload()
end

function main_onunload()
end

function main_oninsert(u)
end

function main_onremove(u)
end

function main_onbutton(key, btn, down)
end
