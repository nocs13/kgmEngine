/*
 * kgmGameLua.h
 *
 *  Created on: Nov 3, 2010
 *      Author: goga
 */

/*
 *  Class for lua integration in kgm game
 */
#pragma once
#include "../kgmSystem/kgmLua.h"
#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiButton.h"

#include "kgmIGame.h"

class kgmGameLua: public kgmLua, public kgmIGame::InputEventListener{

  class luaGui: public kgmGui
  {
    lua_State* glua;
  public:
    luaGui(lua_State* lua, kgmGui* par, int x, int y, int w, int h);
    void onAction(kgmGui* g, u32 t);
  };

  //  class luaGameAction: public kgmIGame::GameSensor::Action{
  //  public:
  //    void action(kgmIGame::GameSensor*);
  //  };

  class luaGameSensor: public kgmIGame::GameSensor{
    kgmLua *lua;
    kgmString id, fnSensor;
  public:
    luaGameSensor(kgmLua* l, kgmString id, kgmString fn){
      lua = l;
      fnSensor = fn;
    }

    void sense(){
      if(lua){
        lua->call(fnSensor, "");
      }
    }
  };

  class luaGameIdle: public kgmIGame::Idle{
    kgmLua *lua;
    kgmString id, fnIdle;
  public:
    luaGameIdle(kgmLua* l, kgmString fn){
      lua = l;
      fnIdle = fn;
    }

    void idle(){
      if(lua){
        lua->call(fnIdle, "");
      }
    }
  };

  kgmIGame*	game;
public:
  kgmGameLua(kgmIGame* g = 0);
  virtual ~kgmGameLua();

  static int _gui(lua_State*);
  static int _gui_menu(lua_State*);
  static int _gui_list(lua_State*);
  static int _gui_button(lua_State*);
  static int _gui_progress(lua_State*);

  static int _gui_show(lua_State*);
  static int _gui_delete(lua_State*);
  static int _gui_resize(lua_State*);

  static int _gui_list_add_item(lua_State*);
  static int _gui_list_rem_item(lua_State*);

  static int _game_command(lua_State*);
  static int _game_camera_move(lua_State*);
  static int _game_camera_rotate(lua_State*);

  static int _game_add_idle(lua_State*);
  static int _game_add_sensor(lua_State*);

  kgmIGame* getGame() const
  {
    return game;
  }

  void onEvent();
};
