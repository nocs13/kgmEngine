Gambit = {}
Gambit.__index = Gambit

function Gambit:new(u)
  local g = {}
  setmetatable(g, Gambit)

  g.u = u or nil
  g.id = ''
  g.switchto = ''
  g.switchfrom = ''

  g.priopity = 0
  g.timeout = 0
  g.stime = 0

  g.sound = nil
  g.animation = nil
  g.fstart = 0
  g.fend = 0

  return g
end