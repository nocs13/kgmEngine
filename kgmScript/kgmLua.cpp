/*
 * kgmLua.cpp
 *
 *  Created on: Nov 10, 2010
 *      Author: goga
 */

#include "kgmLua.h"

kgmLua::kgmLua() {
 m_lua = lua_open();
 if(m_lua){
  luaL_openlibs(m_lua);
 }
}

kgmLua::~kgmLua() {
 if(m_lua)
  lua_close(m_lua);
}

bool kgmLua::load(kgmString str){
 if( luaL_loadstring(m_lua, str.data()) )
  return false;
 return true;
}

bool kgmLua::run(kgmString str){
 if( luaL_dostring(m_lua, str.data()) )
  return false;
 return true;
}

bool kgmLua::set(kgmString name, void* fn){
 lua_register(m_lua, name.data(), (int (*)(lua_State*))fn);
 return true;
}

bool kgmLua::call(kgmString name, kgmString fmt, ...){
 lua_getfield(m_lua, LUA_GLOBALSINDEX, name.data());
 if( !lua_isfunction(m_lua,-1) )
  return false;

 int args = 0;
 int nres = 0;
 char* f = fmt.data();
 va_list vl;
 va_start(vl, f);
 while((*f != '\0') && (args < 128)){
  switch(*f){
  case 's':
   push((char*)va_arg(vl, char*));
   break;
  case 'i':
   push((int)va_arg(vl, int));
   break;
  case 'd':
   push((double)va_arg(vl, double));
   break;
  case 'v':
   push((void*)va_arg(vl, void*));
   break;
  }
  f++;
  args++;
 }

 if( lua_pcall(m_lua, args, nres, 0) )
  return false;
 return true;
}

int kgmLua::toInteger(int num_arg){
 return lua_tointeger(m_lua, num_arg);
}

double kgmLua::toDouble(int num_arg){
 return lua_tonumber(m_lua, num_arg);
}

char* kgmLua::toString(int num_arg){
 return (char*)lua_tostring(m_lua, num_arg);
}

void* kgmLua::toPointer(int num_arg){
 return (void*)lua_topointer(m_lua, num_arg);
}

void kgmLua::push(int arg){
 lua_pushinteger(m_lua, arg);
}

void kgmLua::push(double arg){
 lua_pushnumber(m_lua, arg);
}

void kgmLua::push(char* arg){
 lua_pushstring(m_lua, arg);
}

void kgmLua::push(void* arg){
 lua_pushlightuserdata(m_lua, arg);
}
