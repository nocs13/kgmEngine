#include "../kgmSystem/kgmApp.h"
#include "../kgmGame/kgmGameBase.h"
#include "../kgmGame/kgmGameLogic.h"

#include "../kgmBase/kgmXml.h"
#include "kLua.h"

class kActMotion: public kgmGameLogic::Actuator{
  float length;
  vec3  rotate;
public:
  kActMotion(float l, vec3 r){
    length = l;
    rotate = r;
  }

  bool action(kgmObject* o, int a){
    if(!o)
      return false;

    if(o->instanceof(kgmCamera::Class)){
      kgmCamera::cast(o)->move(length);
      kgmCamera::cast(o)->rotate(rotate.z, rotate.y);
    }
    return true;
  }
};

class kActRotation: public kgmGameLogic::Actuator{
  vec3  rotate;
public:
  kActRotation(vec3 r){
    rotate = r;
  }

  bool action(kgmObject* o, int a){
    if(!o)
      return false;

    if(o->instanceof(kgmCamera::Class)){
      kgmCamera::cast(o)->rotate(a * rotate.z, a * rotate.y);
    }
    return true;
  }
};

class kSnsFly: public kgmGameLogic::Sensor{
public:
  kSnsFly(kgmObject* o)
  :kgmGameLogic::Sensor(o){
    kgmGameLogic::Controller* ctl;
    type = kgmGameLogic::SensorInput;
    ctl = new kgmGameLogic::Controller(kgmGameLogic::ControllerAnd);
    ctl->add(new kActMotion(0.0001, vec3(0, 0, 0)));
    ctls.add(ctl);
    ctl = new kgmGameLogic::Controller(kgmGameLogic::ControllerAnd);
    ctl->add(new kActMotion(-0.0001, vec3(0, 0, 0)));
    ctls.add(ctl);  
    ctl = new kgmGameLogic::Controller(kgmGameLogic::ControllerAnd);
    ctl->add(new kActMotion(0.0, vec3(0, 0, 0.001)));
    ctls.add(ctl);  
    ctl = new kgmGameLogic::Controller(kgmGameLogic::ControllerAnd);
    ctl->add(new kActMotion(0.0, vec3(0, 0, -0.001)));
    ctls.add(ctl);  
    ctl = new kgmGameLogic::Controller(kgmGameLogic::ControllerAnd);
    ctl->add(new kActRotation(vec3(0,  0, 0.001)));
    ctls.add(ctl);
    ctl = new kgmGameLogic::Controller(kgmGameLogic::ControllerAnd);
    ctl->add(new kActRotation(vec3(0.0, 0.1, 0.0)));
    ctls.add(ctl);
  }

  bool sense(void* p){
    ctls[0]->control(((char*)p)[gbtn_up], obj);
    ctls[1]->control(((char*)p)[gbtn_down], obj);
    ctls[2]->control(((char*)p)[gbtn_left], obj);
    ctls[3]->control(((char*)p)[gbtn_right], obj);
    //    ctls[4]->control(((char*)p)[grot_x], obj);
    //    ctls[5]->control(((char*)p)[grot_y], obj);
    return true;
  }
};

class kLogic: public kgmGameLogic{
public:
  kLogic(){
  }
  void addFlyMotion(kgmCamera* o){
    add( new kSnsFly(o) );
  }
};

class kGame: public kgmGameBase{
  class kGui: public kgmGui{
  public:
   kGui(int x, int y, int w, int h)
   :kgmGui(0, x, y, w, h){
   }
   void onAction(kgmGui* g, u32 a){
    ((kGame*)kgmIGame::getGame())->guiAction(g, a);
   }
  };
public:
  kGame(){
    if(m_logic) delete m_logic;
    m_logic = new kLogic();
    //kgmLuaInit(m_script.m_lua.m_lua);
    luaLib(m_script.m_lua.m_lua);
    if(!luaLoad("gui.lua")) fprintf(stderr, "\nLua error");
    if(!luaRun("main.lua")) fprintf(stderr, "\nLua error");
  }

  ~kGame(){
  }

  int gLoad(kgmString s){
    m_logic->clear();
    int r = kgmGameBase::gLoad(s);
    if(m_scenes.size()){
      for(int i = 0; i < m_scenes[0]->objects.size(); i++){
        if(m_scenes[0]->objects[i]->instanceof(kgmCamera::Class)){
          ((kLogic*)m_logic)->addFlyMotion((kgmCamera*)m_scenes[0]->objects[i]);
        }
      }
    }
    return r;
  }

  int gCommand(kgmString s){
   char* str = s.data();
   if(s == "quit"){
	gQuit();
   }else if(s == "pause"){
     m_state = State_Pause;
   }else   if(s == "resume"){
     m_state = State_Play;
   }else if(!strncmp(str, "load ", 5)){
    gLoad(&str[5]);
    return gState();
   }else if(!strncmp(str, "add ", 4)){
    gLoad(&str[4]);
    return gState();
   }

   return 1;
  }

  void onKeyDown(int k){
    kgmGameBase::onKeyDown(k);
  }

  void onKeyUp(int k){
    kgmGameBase::onKeyUp(k);
    keyAction(k, false);
  }

  kgmActor* gSpawn(kgmString a){
   kgmActor* actor = 0;
   lua_State* lua = m_script.m_lua.m_lua;
   lua_getfield(lua, LUA_GLOBALSINDEX, "onSpawn");
   if( lua_isfunction(lua,-1) ){
    lua_pushstring(lua, a.data());
    lua_pcall(lua, 1, 1, 0);
    actor = (kgmActor*)lua_topointer(lua, -1);
   }
   if(!actor)
    actor = kgmGameBase::gSpawn(a);
   fprintf(stderr, "\nSpwning actor: %s %i", a.data(), (int)actor);
   return actor;
  }

  bool luaLoad(kgmString script)
  {
    kgmMemory<char> m;
    m_resources->getFile(script, m);

    if(m.length() > 0){
      kgmString s(m.data(), m.length() + 1);
      m_script.add(kgmGameScript::Lua, s);
      return true;
    }
    return false;
  }

  bool luaRun(kgmString script)
  {
    kgmMemory<char> m;
    m_resources->getFile(script, m);

    if(m.length() > 0){
      kgmString s(m.data(), m.length() + 1);
      m_script.run(kgmGameScript::Lua, s);
      return true;
    }
    return false;
  }

  //Lua
  void luaLib(lua_State* lua){
   lua_register(lua, "screenSize",  luaScreenSize);
   lua_register(lua, "gameCommand", luaGameCommand);
   lua_register(lua, "Gui",         luaGui);
   lua_register(lua, "GuiList",     luaGuiList);
   lua_register(lua, "GuiButton",   luaGuiButton);
   lua_register(lua, "showGui",     luaShowGui);
   lua_register(lua, "setGuiColor", luaSetGuiColor);
   lua_register(lua, "setGuiRect",  luaSetGuiRect);
   lua_register(lua, "addGuiText",  luaAddGuiText);
   lua_register(lua, "getGuiText",  luaGetGuiText);

   lua_register(lua, "Actor", luaActor);
   lua_register(lua, "setActorProperty", luaSetActorProperty);
  }

  void keyAction(u32 k, bool s){
   lua_State* lua = m_script.m_lua.m_lua;
   lua_getfield(lua, LUA_GLOBALSINDEX, "onKeyboard");
   if( !lua_isfunction(lua,-1) )
    return;
   lua_pushinteger(lua, k);
   lua_pushinteger(lua, s);
   lua_pcall(lua, 2, 0, 0);
  }

  void guiAction(kgmGui* g, u32 a){
   lua_State* lua = m_script.m_lua.m_lua;
   lua_getfield(lua, LUA_GLOBALSINDEX, "onGuiAction");
   if( !lua_isfunction(lua,-1) )
    return;
   lua_pushlightuserdata(lua, g);
   lua_pushinteger(lua, a);
   lua_pcall(lua, 2, 0, 0);
  }

  void addGui(kgmGui* g){
    kgmGameBase::add(g);
  }

  //
  static int luaScreenSize(lua_State* lua){
   kGame* g = (kGame*)kgmIGame::getGame();
   int r[4];
   g->getRect(r[0], r[1], r[2], r[3]);
   lua_pushnumber(lua, r[2]);
   lua_pushnumber(lua, r[3]);
   printf("Size: %i %i", r[2], r[3]);
   return 2;
  }

  static int luaGameCommand(lua_State* lua){
   kgmString s = lua_tostring(lua, 1);
   int res = ((kGame*)kgmIGame::getGame())->gCommand(s);
   lua_pushinteger(lua, res);
   return 1;
  }

  static int luaGui(lua_State* lua){
   int n = lua_gettop(lua);
   kgmGui* gui = 0;
   if(n < 5){
    lua_pushlightuserdata(lua, gui);
    return 1;
   }
   if(!lua_topointer(lua, 1)){
    gui = new kGui(lua_tointeger(lua, 2),
                   lua_tointeger(lua, 3),
                   lua_tointeger(lua, 4),
                   lua_tointeger(lua, 5));
    ((kGame*)kgmGameBase::getGame())->addGui(gui);
   }else{
    gui = new kgmGui((kgmGui*)lua_topointer(lua, 1),
    			 lua_tointeger(lua, 2),
	                 lua_tointeger(lua, 3),
	                 lua_tointeger(lua, 4),
	                 lua_tointeger(lua, 5));
   }
   lua_pushlightuserdata(lua, gui);
   return 1;
  }

  static int luaGuiList(lua_State* lua){
   int n = lua_gettop(lua);
   if(n < 5){
     lua_pushlightuserdata(lua, 0);
  	 return 1;
   }

   kgmGui* gui = new kgmGuiList((kgmGui*)lua_topointer(lua, 1),
  		                   	    lua_tointeger(lua, 2),
  		                   	    lua_tointeger(lua, 3),
  		                   	    lua_tointeger(lua, 4),
  		                   	    lua_tointeger(lua, 5));
   lua_pushlightuserdata(lua, gui);
   return 1;
  }

  static int luaGuiButton(lua_State* lua){
   int n = lua_gettop(lua);
   if(n < 5){
     lua_pushlightuserdata(lua, 0);
  	 return 1;
   }

   kgmGui* gui = new kgmGuiButton((kgmGui*)lua_topointer(lua, 1),
  		                   	    lua_tointeger(lua, 2),
  		                   	    lua_tointeger(lua, 3),
  		                   	    lua_tointeger(lua, 4),
  		                   	    lua_tointeger(lua, 5));
   kgmString s = lua_tostring(lua, 6);
   gui->setText(s);
   fprintf(stderr, "\nt: %s", s.data());
   lua_pushlightuserdata(lua, gui);
   return 1;
  }

  static int luaShowGui(lua_State* lua){
   kgmGui* gui = (kgmGui*)lua_topointer(lua, 1);
   if(lua_tointeger(lua, 2))
	gui->show();
   else
	gui->hide();
   return 0;
  }

  static int luaSetGuiColor(lua_State* lua){
   kgmGui* gui = (kgmGui*)lua_topointer(lua, 1);
   int     cid = lua_tointeger(lua, 2);
   return 0;
  }

  static int luaSetGuiRect(lua_State* lua){
   kgmGui* gui = (kgmGui*)lua_topointer(lua, 1);
   int     x = lua_tointeger(lua, 2);
   int     y = lua_tointeger(lua, 3);
   int     w = lua_tointeger(lua, 4);
   int     h = lua_tointeger(lua, 5);
   gui->setRect(x, y, w, h);
   return 0;
  }

  static int luaAddGuiText(lua_State* lua){
   kgmGuiList* gui = (kgmGuiList*)lua_topointer(lua, 1);
   kgmString s = lua_tostring(lua, 2);
   gui->addItem(s);
   return 0;
  }

  static int luaGetGuiText(lua_State* lua){
   kgmGui* gui = (kgmGuiList*)lua_topointer(lua, 1);
   int     i   = lua_tointeger(lua, 2);
   kgmString s;
   if(i < 0){
	  s = gui->m_text;
   }else{
	 s = ((kgmGuiList*)gui)->getItem(i);
   }
   lua_pushstring(lua, s.data());
   return 1;
  }

  static int luaActor(lua_State* lua){
   kgmActor* a = new kgmActor();
   lua_pushlightuserdata(lua, a);
   return 1;
  }

  static int luaSetActorProperty(lua_State* lua){
   kgmActor* a = (kgmActor*)lua_topointer(lua, 1);
   char*     c = (char*)lua_tostring(lua, 2);
   char*     s = (char*)lua_tostring(lua, 3);

   if(!a)
     return 0;

   if(!strncmp(c, "section", 7)){
     a->addSection();
   }else if(!strncmp(c, "mesh", 4)){
     a->add(0, getGame()->getResources()->getMesh(s));
   }else if(!strncmp(c, "material", 8)){
     a->set(0, getGame()->getResources()->getMaterial(s));
   }else if(!strncmp(c, "skeleton", 8)){
     a->set(0, getGame()->getResources()->getSkeleton(s));
   }else if(!strncmp(c, "animation", 9)){
     a->set(0, getGame()->getResources()->getAnimation(s));
   }
   return 0;
  }
  //
};


class kApp: public kgmApp{
 kGame*	m_game;	 
public:
 kApp(){
 }
 
 ~kApp(){
 }

 void print_xml(kgmXml::Node* n){
  if(!n)
   return;
  kgmString s, d;
  n->id(s);
  n->data(d);
  printf("\n id=%s", (char*)s);
  printf("\n data=%s", (char*)d);
  
  for(int i = 0; i < n->attributes(); i++){
    kgmString s, d;
    n->attribute(i, s, d);
    printf("\n   attr: %s=%s", (char*)s, (char*)d);
  }
  
  for(int i = 0; i < n->nodes(); i++)
   print_xml(n->node(i));
 }
 
  void printf_xml(char* path){
   kgmString s(path, strlen(path));
   kgmFile f;
   f.open(s, kgmFile::Read);
   kgmMemory<char> m;
   m.alloc(f.length());
   f.read(m.data(), f.length());
   f.close();
   kgmXml xml(m);
   print_xml(xml.m_node);
  }

 void main(){
  //printf_xml("map001.kgm");
  m_game = new kGame();
  m_game->loop();
  delete m_game;
  //kgmOGLWindow* w = new kgmOGLWindow(0, "", 0, 0, 100, 100, 16, false); 
  //w->loop();
  //delete w;
   printf("hello\n");
 } 
};

//main object
kApp theApp;
//////////////
