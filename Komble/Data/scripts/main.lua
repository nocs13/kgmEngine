kgmImport('guis')

retent = nil
mapId = ''

function main_init()
  --kgmLog('Init interface')
  guis_init()

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
   --kgmGameExit()
end
