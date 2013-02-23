--set keys
Key_Escape = 1;
Key_Space = 50;

--build guis
sW, sH = screenSize()
 
gui  = Gui(0, 0, 0, sW, sH)
 btn1 = GuiButton(gui, 1,   1, 100, 30, 'Game')
 btn2 = GuiButton(gui, 100, 1, 100, 30, 'Video')
 btn3 = GuiButton(gui, 200, 1, 100, 30, 'Audio')
 btn4 = GuiButton(gui, 300, 1, 100, 30, 'Input')
 btn5 = GuiButton(gui, 400, 1, 100, 30, 'Net')
 btn6 = GuiButton(gui, 500, 1, 100, 30, 'Quit')

guiGame  = Gui(gui, 0, 31, sW, sH - 31)
 guiMapList = GuiList(guiGame, 50, 50, 200, 200)
  setGuiColor(guiMapList,  0, 0x545454)
  setGuiColor(guiMapList,  3, 0xFFFFFF)
  addGuiText(guiMapList, 'map001.kgm')
  addGuiText(guiMapList, 'map002.kgm')
  addGuiText(guiMapList, 'map005.kgm')
  
guiVideo = Gui(gui, 0, 31, sW, sH - 31)
guiAudio = Gui(gui, 0, 31, sW, sH - 31)
guiInput = Gui(gui, 0, 31, sW, sH - 31)
guiNet   = Gui(gui, 0, 31, sW, sH - 31)

showGui(guiGame,  1)
showGui(guiVideo, 1)
showGui(guiAudio, 1)
showGui(guiInput, 1)
showGui(guiNet,   1)

guiPause = Gui(0, 0, 0, sW, sH)
 btnMenu = GuiButton(guiPause, sW / 4,  sH / 5, sW / 2, sH / 5, 'Leave game')
 btnResume = GuiButton(guiPause, sW / 4, sH / 2, sW / 2, sH / 5, 'Continue')
  setGuiColor(btnMenu, 0, 0x99AABB)
   setGuiColor(btnMenu, 3, 0xFFFFFF)
  setGuiColor(btnResume, 0, 0x99AABB)
   setGuiColor(btnResume, 3, 0xFFFFFF)
 showGui(guiPause, 0)

function hideGuis()
 showGui(guiGame,  0)
 showGui(guiVideo, 0)
 showGui(guiAudio, 0)
 showGui(guiInput, 0)
 showGui(guiNet,   0)
end

gameState = 'none'
function onGuiAction(g,a)
 if g == btn1 then
  hideGuis()
  showGui(guiGame, 1)
 elseif g == btn2 then
  hideGuis()
  showGui(guiVideo, 1)
 elseif g == btn3 then
  hideGuis()
  showGui(guiAudio, 1)
 elseif g == btn4 then
  hideGuis()
  showGui(guiInput, 1)
 elseif g == btn5 then
  hideGuis()
  showGui(guiNet, 1)
 elseif g == btn6 then
  gameCommand('quit')
 elseif g == guiMapList then
  local res = 0 
  local map = getGuiText(guiMapList, a)
  print('Loading map: '..map..'...')
  res = gameCommand('load '..map)
  if res == 1 then
   showGui(gui, 0)
   gameState='play'
  end
 elseif g == btnResume then
  showGui(guiPause, 0)
  gameState = 'play'
 elseif g == btnMenu then
  showGui(guiPause, 0)
  showGui(gui, 1)
  gameState = 'none'
 end
end

function onResize(w, h)
end

function onKeyboard(k, s)
 if k == Key_Escape then
  if gameState == 'none' then
   gameCommand('quit'); 
  elseif gameState == 'play' then
   gameState = 'pause';
   showGui(guiPause, 1)
  end 
 end
end

function onSpawn(s)
 print('spawn actor: '..s);
 
 local a = nil;
 if s == 'RenTao' then
  a = Actor();
  setActorProperty(a, 'section', '');
  setActorProperty(a, 'mesh', 'RenTao.msh.kgm');
  setActorProperty(a, 'material', 'RenTao.mtl.kgm');
  setActorProperty(a, 'skeleton', 'RenTao.skl.kgm');
  setActorProperty(a, 'animation', 'RenTao.anm.kgm');
 elseif s == 'NejiHyuga' then
  a = Actor();
  setActorProperty(a, 'party', '');
  setActorProperty(a, 'mesh', 'NejiHyuga.kgm');
  setActorProperty(a, 'material', 'NejiHyuga.kgm');
  setActorProperty(a, 'skeleton', 'NejiHyuga.kgm');
  setActorProperty(a, 'animation', 'NejiHyuga.kgm');
 end
 return a
end
