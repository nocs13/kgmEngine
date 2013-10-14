#include "kgmGameScript.h"
#include "kgmGameBase.h"
#include "kgmGameObject.h"
#include "kgmActor.h"
#include <stdarg.h>

#ifdef LUA
typedef int (*lua_CFn) (lua_State *lua);

static int kgmLuaListObjects(lua_State *lua);
static int kgmLuaListActors(lua_State *lua);
static int kgmLuaRemoveObject(lua_State *lua);
static int kgmLuaGetObjectType(lua_State *lua);
static int kgmLuaGetActorState(lua_State *lua);
static int kgmLuaSetActorState(lua_State *lua);

lua_State* lua_main = null;

bool kgmLuaOpen()
{
  if(lua_main)
    return false;

  lua_main = lua_open(0);

  if(!lua_main)
    return false;


  lua_baselibopen (lua_main);
  lua_iolibopen (lua_main);
  lua_strlibopen (lua_main);
  lua_mathlibopen (lua_main);
  lua_dblibopen (lua_main);

  return true;
}

void kgmLuaClose()
{
  if(lua_main)
    lua_close(lua_main);

  lua_main = null;
}

bool kgmLuaRun(kgmString str){
  if(lua_dostring(lua_main, str.data()))
    return false;

  return true;
}

bool kgmLuaSet(kgmString name, void* fn){
  lua_register(lua_main, name.data(), (int (*)(lua_State*))fn);

  return true;
}

bool kgmLuaCall(kgmString name, kgmString fmt, ...){
  lua_getglobal(lua_main, name.data());

  if( !lua_isfunction(lua_main,-1) )
    return false;

  int args = 0;
  int nres = 0;
  char* f = fmt.data();
  va_list vl;
  va_start(vl, f);

  while((*f != '\0') && (args < 128)){
    switch(*f){
    case 's':
      kgmLuaPush((char*)va_arg(vl, char*));
      break;
    case 'i':
      kgmLuaPush((int)va_arg(vl, int));
      break;
    case 'd':
      kgmLuaPush((double)va_arg(vl, double));
      break;
    case 'v':
      kgmLuaPush((void*)va_arg(vl, void*));
      break;
    }
    f++;
    args++;
  }

  if(lua_call(lua_main, args, nres))
    return false;

  return true;
}

int kgmLuaToInteger(int num_arg){
  return (int)lua_tonumber(lua_main, num_arg);
}

double kgmLuaToDouble(int num_arg){
  return lua_tonumber(lua_main, num_arg);
}

char* kgmLuaToString(int num_arg){
  return (char*)lua_tostring(lua_main, num_arg);
}

void* kgmLuaToPointer(int num_arg){
  return (void*)lua_topointer(lua_main, num_arg);
}

void kgmLuaPush(int arg){
  lua_pushnumber(lua_main, arg);
}

void kgmLuaPush(double arg){
  lua_pushnumber(lua_main, arg);
}

void kgmLuaPush(char* arg){
  lua_pushstring(lua_main, arg);
}

void kgmLuaPush(void* arg){
  lua_pushuserdata(lua_main, arg);
}

//
int kgmLuaListObjects(lua_State *lua)
{
  int n = lua_gettop(lua);

  if(n < 1)
    return 0;

  const char* fn_name = lua_tostring(lua, 1);

  kgmIGame* game = kgmIGame::getGame();

  if(!game || !game->getLogic())
    return;

  kgmList<kgmGameObject*> objects;
  game->getLogic()->getObjects(objects);

  for(int i = 0; i < objects.length(); i++)
    kgmLuaCall(fn_name, "v", objects[i]);

  return 0;
}

int kgmLuaListActors(lua_State *lua)
{

}

int kgmLuaRemoveObject(lua_State *lua)
{

}

int kgmLuaGetObjectType(lua_State *lua)
{

}

int kgmLuaGetActorState(lua_State *lua)
{

}

int kgmLuaSetActorState(lua_State *lua)
{

}

#endif
