/*
 * kgmGameLua.cpp
 *
 *  Created on: Nov 3, 2010
 *      Author: goga
 */

#include "kgmGameLua.h"
#include "kgmGameBase.h"

static kgmIGame*    g_game = 0;
static kgmGameLua*  g_glua = 0;

//luaGui
kgmGameLua::luaGui::luaGui(lua_State* lua, kgmGui* par, int x, int y, int w, int h)
:kgmGui(par, x, y, w, h)
{
    glua = 0;
    
    if(!par && lua)
    {
      glua = lua;
      //lua_getglobal(lua, "kgmEngine");
      //lua_gettop(lua);
      kgmGameBase* game = (kgmGameBase*)lua_touserdata(lua, -2);

      //lua_getfield(lua, LUA_GLOBALSINDEX, "kgmGame");
      //kgmGameBase* game = (kgmGameBase*)lua_touserdata(lua, -1);
      if(g_game != 0)
      {
        g_game->addGui(this);
      }
    }
    
    if(par)
      glua = ((luaGui*)par)->glua;
}

void kgmGameLua::luaGui::onAction(kgmGui* g, uint t)
{
  if (glua)
  {
    lua_getglobal(glua, "_gui_action");
    if (lua_isfunction(glua, -1))
    {
      lua_pushlightuserdata(glua, g);
      lua_pushnumber(glua, t);

      lua_pcall(glua, 2, 1, 0);
    }
  }
}

kgmGameLua::kgmGameLua(kgmIGame* g)
{
  game = g_game = g;
  g_glua = this;

  lua_newtable(m_lua);
  lua_pushstring(m_lua, "game");
  lua_pushlightuserdata(m_lua, game);
  lua_settable(m_lua, -3);
  lua_setglobal(m_lua, "kgmEngine");

  //set("kgmGame", this);
  set("_gui", (void*)_gui);
  set("_gui_button", (void*)_gui_button);
  set("_gui_list", (void*)_gui_list);

  set("_gui_show", (void*)_gui_show);
  set("_gui_delete", (void*)_gui_delete);

  set("_gui_list_add_item", (void*)_gui_list_add_item);
  set("_gui_list_rem_item", (void*)_gui_list_rem_item);

  set("_game_command", (void*)_game_command);
  set("_game_add_idle", (void*)_game_add_idle);
  set("_game_add_sensor", (void*)_game_add_sensor);
  set("_game_camera_move", (void*)_game_camera_move);
  set("_game_camera_rotate", (void*)_game_camera_rotate);

  eventMask = kgmIGame::InputEventListener::EventAll;
  if(g)
    g->addInputEventListener(this);
}

kgmGameLua::~kgmGameLua()
{
}

int kgmGameLua::_gui(lua_State* l)
{
  kgmGui* gpar = (kgmGui*)lua_topointer(l, 1);
  int x = lua_tointeger(l, 2);
  int y = lua_tointeger(l, 3);
  int w = lua_tointeger(l, 4);
  int h = lua_tointeger(l, 5);
  lua_pushlightuserdata(l, new luaGui(l, gpar, x, y, w, h));
  return 1;
}

int kgmGameLua::_gui_button(lua_State* l)
{
  kgmGui* gpar = (kgmGui*)lua_topointer(l, 1);
  if(!gpar){
    printf("not valid parent for gui button \n");
    return 0;
  }

  int x = lua_tointeger(l, 2);
  int y = lua_tointeger(l, 3);
  int w = lua_tointeger(l, 4);
  int h = lua_tointeger(l, 5);
  kgmString s = lua_tostring(l, 6);
  kgmGuiButton* b = new kgmGuiButton(gpar, x, y, w, h);
  b->setText(s);
  lua_pushlightuserdata(l, b);
  return 1;
}

int kgmGameLua::_gui_list(lua_State* l)
{
  kgmGui* gpar = (kgmGui*)lua_topointer(l, 1);
  int x = lua_tointeger(l, 2);
  int y = lua_tointeger(l, 3);
  int w = lua_tointeger(l, 4);
  int h = lua_tointeger(l, 5);
  kgmGuiList* ls = new kgmGuiList(gpar, x, y, w, h);
  for(int i = 6; lua_type(l, i) != LUA_TNONE; i++)
  {
    if(lua_isstring(l, i))
    {
      ls->addItem(lua_tostring(l, i));
    }
  }
  lua_pushlightuserdata(l, ls);
  return 1;
}

int kgmGameLua::_gui_progress(lua_State* l)
{
  return 0;
}


int kgmGameLua::_gui_show(lua_State* l)
{
  kgmGui* gui = (kgmGui*)lua_topointer(l, 1);
  int s = lua_tointeger(l, 2);
  if(gui)
  {
    if(s)
      gui->show();
    else
      gui->hide();
  }
  return 0;
}

int kgmGameLua::_gui_delete(lua_State* l)
{
  kgmGui* gui = (kgmGui*)lua_topointer(l, 0);
  int s = lua_tointeger(l, 1);
  if(gui){
    gui->release();
  }
  return 0;
}

int kgmGameLua::_gui_list_add_item(lua_State* l)
{
  kgmGuiList* gui = (kgmGuiList*) lua_topointer(l, 1);
  if (gui)
  {
    kgmString s;
    const char* a = lua_tostring(l, 2);
    s = a;
    gui->addItem(s);
  }
  return 0;
}

int kgmGameLua::_gui_list_rem_item(lua_State* l)
{
  kgmGuiList* gui = (kgmGuiList*) lua_topointer(l, 0);
  int s = lua_tointeger(l, 1);
  if (gui)
  {
    gui->remItem(s);
  }
  return 0;
}

int kgmGameLua::_game_command(lua_State* l)
{
  int res = 0;
  const char* str = lua_tostring(l, 1);

  if(!str)
  {
    lua_pushnumber(l, res);
    return res;
  }

  printf("%s\n", str);

  kgmList<kgmString> args;
  char* c = (char*)str;
  int spos = 0,
       epos = 0;

  while(true)
  {
    if((c[epos] == ' ') && (spos != epos))
    {
      args.add(kgmString(&str[spos], epos - spos));
      spos = epos;
    }
    else if((c[epos] == ' ') && (spos == epos))
    {
      spos = ++epos;
    }
    else if((c[epos] == '\0'))
    {
      if(spos != epos)
        args.add(kgmString(&str[spos], epos - spos));
      break;
    }
    else
    {
      epos++;
    }
  }

  if(args.length() < 1)
  {
    lua_pushnumber(l, res);
    return res;
  }

  if(args[0] == "load")
  {
    res = kgmIGame::getGame()->gLoad(args[1]);
  }
  else if(args[0] == "unload")
  {
    res = kgmIGame::getGame()->gUnload();
  }
  else if(args[0] == "quit")
  {
    res = kgmIGame::getGame()->gQuit();
  }

  lua_pushnumber(l, res);
  return 1;
}

int kgmGameLua::_game_camera_move(lua_State* l)
{
    int res = 0;
    double dist = lua_tonumber(l, 1);
    
    kgmCamera& cam = ((kgmGameBase*)g_game)->m_render->camera();
    cam.move((float)dist);
    
    return 0;
}

int kgmGameLua::_game_camera_rotate(lua_State* l)
{
    int res = 0;
    double yaw = lua_tonumber(l, 1);
    double pitch = lua_tonumber(l, 2);
    double roll = lua_tonumber(l, 3);
    
    kgmCamera& cam = ((kgmGameBase*)g_game)->m_render->camera();
    cam.rotate((float)yaw, (float)pitch);

    return 0;
}

int kgmGameLua::_game_add_idle(lua_State* l)
{
  int res = 0;
  const char* fn = lua_tostring(l, 1);

  luaGameIdle* lgi = new luaGameIdle(g_glua, fn);
  g_game->addGameIdle(lgi);

  res = 1;
  lua_pushnumber(l, res);
  return 1;
}

int kgmGameLua::_game_add_sensor(lua_State* l)
{
  int res = 0;
  const char* id = lua_tostring(l, 1);
  const char* fn = lua_tostring(l, 2);

  luaGameSensor* lgs = new luaGameSensor(g_glua, id, fn);
  g_game->addGameSensor(lgs);

  res = 1;
  lua_pushnumber(l, res);
  return 1;
}

void kgmGameLua::onEvent()
{
  if(eventType == kgmIGame::InputEventListener::EventInput){
      //printf("GameLua: _game_input, %i %i", eventType, eventArgument);
      call("_game_input", "ii", eventAction, eventArgument);
  }
}
