#include "kgmLuaScript.h"
#include "../kgmBase/kgmIResources.h"
#include <lua/lua.h>
#include <lua/lualib.h>
#include <stdarg.h>

kgmLuaScript::kgmLuaScript(kgmIResources *r)
{
  resources = r;

  handler = lua_open(0);

  if(!handler)
    return;


  //lua_baselibopen (handler);
  //lua_iolibopen (handler);
  //lua_strlibopen (handler);
  //lua_mathlibopen (handler);
  //lua_dblibopen (handler);
}

kgmLuaScript::~kgmLuaScript()
{
  if(handler)
    lua_close(handler);
}

bool kgmLuaScript::load(kgmString s)
{
  if (!resources)
    return false;

  kgmString ls = s + ".lua";

  kgmString script;

  kgmMemory<u8> content;

  if (!resources->getFile(ls, content))
    return false;

  script = (const s8*) content.data();

  if(lua_dostring(handler, script.data()))
    return false;

  return true;
}

bool kgmLuaScript::set(kgmString name, void* fn)
{
  lua_register(handler, name.data(), (int (*)(lua_State*))fn);

  return true;
}

void* kgmLuaScript::call(kgmString name, kgmString fmt, ...)
{
  lua_getglobal(handler, name.data());

  if( !lua_isfunction(handler, -1) )
    return (void *) -1;

  int args = 0;
  int nres = 0;
  char* f = fmt.data();
  va_list vl;
  va_start(vl, fmt);

  while((*f != '\0') && (args < 128))
  {
    switch(*f)
    {
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

  if(lua_call(handler, args, nres))
    return (void *) -1;

  f64 r_number;

  r_call = null;

  if(lua_isnumber(handler, -1))
  {
    r_number = (f64) lua_tonumber(handler, -1);

    r_call = &r_number;
  }
  else if(lua_isstring(handler, -1))
  {
    r_call = (char *) lua_tostring(handler, -1);
  }
  else if(lua_isuserdata(handler, -1))
  {
    r_call = (void *) lua_topointer(handler, -1);
  }

  return r_call;
}

void kgmLuaScript::push(int arg)
{
  lua_pushnumber(handler, arg);
}

void kgmLuaScript::push(double arg)
{
  lua_pushnumber(handler, arg);
}

void kgmLuaScript::push(char* arg)
{
  lua_pushstring(handler, arg);
}

void kgmLuaScript::push(void* arg)
{
  lua_pushuserdata(handler, arg);
}
