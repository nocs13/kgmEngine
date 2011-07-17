/*
 * kgmGameLua.cpp
 *
 *  Created on: Nov 3, 2010
 *      Author: goga
 */

#include "kgmGameLua.h"
#include "kgmGameBase.h"

#include "../kgmMain/kgmTab.h"

kgmGameLua::kgmGameLua() {
}

kgmGameLua::~kgmGameLua() {
}
/*
void kgmGameLua::onEvent(kgmEvent::Event* e){
 if(!m_lua)
  return;
 lua_getglobal(m_lua, "onEvent");
 if(lua_isfunction(m_lua, -1)){
  push(e->event);
  lua_pcall(m_lua, 1, 0, 0);
 }
}

void kgmGameLua::onAction(kgmGui* g, int a){
 if(!m_lua)
  return;
 lua_getglobal(m_lua, "onAction");
 if(lua_isfunction(m_lua, -1)){
  push(g);
  push(a);
  lua_pcall(m_lua, 2, 0, 0);
 }
}*/
/////////////////
int kgmGameLua::command(lua_State* lua){
}

int kgmGameLua::gameStart(lua_State* lua){
 lua_pushlightuserdata(lua, kgmGameBase::getGame());
 return 1;
}

int kgmGameLua::gameLoop(lua_State*){
 return 0;
}

int kgmGameLua::gameQuit(lua_State*){
 return 0;
}

int kgmGameLua::gameLoad(lua_State*){
 return 0;
}

