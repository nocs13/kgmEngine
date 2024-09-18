#include "kgmLuaScript.h"
#include "../kgmBase/kgmIResources.h"
#include "../kgmBase/kgmLog.h"
#include <stdarg.h>

extern "C"
{
  #include <lua/lua.h>
  #include <lua/lualib.h>
  #include <lua/lauxlib.h>
}

kgmLuaScript::kgmLuaScript()
{
  need_close = false;

  init();
}

kgmLuaScript::kgmLuaScript(lua_State *h)
{
  need_close = false;

  if (h)
  {
    handler = h;
  }
  else
  {
    init();
  }
}

kgmLuaScript::~kgmLuaScript()
{
  if(handler && need_close)
    lua_close(handler);
}

void kgmLuaScript::setX(void* x)
{
  X = x;
}

void* kgmLuaScript::getX()
{
  return X;
}

/*
bool kgmLuaScript::load(kgmString s)
{
  if (!resources)
    return false;

  kgmString ls = "scripts/";

  ls += (s + ".lua");

  kgmString script;

  kgmArray<u8> content;

  if (!resources->getFile(ls, content))
    return false;

  script = kgmString((const char*) content.data(), content.length());

  s32 res = 0;

  #if defined(LUA_VERSION_NUM) && (LUA_VERSION_NUM >= 501)
    res = luaL_dostring(handler, script.data());
  #else
    res = lua_dostring(handler, script.data());
  #endif

  if(res)
  {
    const char* err = lua_tostring(handler, -1);

    kgm_log() << "Lua dostring error: " << err << "\n";

    lua_pop(handler, 1);

    kgm_log() << "Lua loading script: " << s.data() << " failed.\n";

    return false;
  }

  return true;
}
*/

bool kgmLuaScript::load(kgmString sc)
{
  s32 res = 0;

  #if defined(LUA_VERSION_NUM) && (LUA_VERSION_NUM >= 501)
    res = luaL_dostring(handler, sc.data());
  #else
    res = lua_dostring(handler, sc.data());
  #endif

  if(res)
  {
    const char* err = lua_tostring(handler, -1);

    //kgm_log() << "run Lua dostring error: " << err << "\n";
    fprintf(stderr, "run Lua dostring error: %s\n", err);

    lua_pop(handler, 1);

    return false;
  }

  return true;
}

bool kgmLuaScript::set(kgmString name, FN fn)
{
  lua_register(handler, name.data(), (int (*)(lua_State*)) fn);

  return true;
}

bool kgmLuaScript::args(kgmString fmt, ...)
{
  int args = 0;
  char* f = fmt.data();
  va_list vl;
  va_start(vl, fmt);

  s32 largs = lua_gettop(handler);

  if(largs < 2)
    m_carg = -1;
  else
    m_carg = 1;

  while((*f != '\0') && (args < 128))
  {
    switch(*f)
    {
    case 's':
      pop((char**)va_arg(vl, char**));
      break;
    case 'i':
      pop((int*)va_arg(vl, int*));
      break;
    case 'd':
      pop((double*)va_arg(vl, double*));
      break;
    case 'p':
      pop((void**)va_arg(vl, void**));
      break;
    }

    f++;
    args++;

    if(largs > 1)
      m_carg = args + 1;
  }

  va_end(vl);

  return true;
}

bool kgmLuaScript::resl(kgmString fmt, ...)
{
  int args = 0;
  char* f = fmt.data();
  va_list vl;
  va_start(vl, fmt);

  while((f) && (*f != '\0') && (args < 128))
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
    case 'p':
      push((void*)va_arg(vl, void*));
      break;
    }

    f++;
    args++;
  }

  va_end(vl);

  return true;
}

bool kgmLuaScript::reslarr(kgmString fmt, void *a, s32 c)
{
  if (fmt.length() < 1 || !a || !c)
    return false;

  char* f = fmt.data();
  s32 size = 0;

  switch(*f)
  {
  case 's':
    size = sizeof(s8*);
    break;
  case 'i':
    size = sizeof(s32);
    break;
  case 'd':
    size = sizeof(double);
    break;
  case 'p':
    size = sizeof(void*);
    break;
  default:
    return false;
  }

  char* p = (char*)a;
  s32 i = 0;

  char*   sdata;
  s32   idata;
  f64   fdata;
  void* pdata;


  lua_newtable(handler);
  lua_settable(handler, 1);


  switch(*f)
  {
  case 's':
    for (i = 0; i < c; i++)
    {
      memcpy(&sdata, (p + size * i), sizeof(sdata));
      lua_pushnumber(handler, i + 1);
      lua_pushstring(handler, sdata);
      lua_rawset(handler, -3);
    }
    break;
  case 'i':
    for (i = 0; i < c; i++)
    {
      memcpy(&idata, (p + size * i), sizeof(idata));
      lua_pushnumber(handler, i + 1);
      lua_pushnumber(handler, idata);
      lua_rawset(handler, -3);
    }
    break;
  case 'd':
    for (i = 0; i < c; i++)
    {
      memcpy(&fdata, (p + size * i), sizeof(fdata));
      lua_pushnumber(handler, i + 1);
      lua_pushnumber(handler, fdata);
      lua_rawset(handler, -3);
    }
    break;
  case 'p':
    for (i = 0; i < c; i++)
    {
      memcpy(&pdata, (p + size * i), sizeof(pdata));
      lua_pushnumber(handler, i + 1);
      #if defined(LUA_VERSION_NUM) && (LUA_VERSION_NUM >= 501)
        lua_pushlightuserdata(handler, pdata);
      #else
        lua_pushuserdata(handler, pdata);
      #endif
      lua_rawset(handler, -3);
    }
    break;
  }

  return true;
}

void* kgmLuaScript::call(const s8* name, const s8* fmt, ...)
{
  s32 ssize = lua_gettop(handler);

  lua_getglobal(handler, name);

  if( !lua_isfunction(handler, -1) )
    return (void *) -1;

  int args = 0;
  int nres = 0;
  char* f = (char*) fmt;
  va_list vl;
  va_start(vl, fmt);

  while((f) && (*f != '\0') && (args < 128))
  {
    switch(*f)
    {
    case 's':
      push((char*) va_arg(vl, char*));
      break;
    case 'i':
      push((int)va_arg(vl, int));
      break;
    case 'd':
      push((double)va_arg(vl, double));
      break;
    case 'p':
      push((void*)va_arg(vl, void*));
      break;
    }

    f++;
    args++;
  }

  va_end(vl);

  lua_call(handler, args, nres);

  s32 rnum = lua_gettop(handler) - ssize;

  if (rnum < 1)
    return (void *) -1;

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

void* kgmLuaScript::ccall(const s8* name, const s8* method, const s8* fmt, ...)
{
  s32 ssize = lua_gettop(handler);

  lua_getglobal(handler, name);

  lua_getfield(handler, -1, method);
  lua_pushvalue(handler, -2);

  int args = 0;
  int nres = 0;
  char* f = (char*) fmt;
  va_list vl;
  va_start(vl, fmt);

  while((f) && (*f != '\0') && (args < 128))
  {
    switch(*f)
    {
    case 's':
      push((char*) va_arg(vl, char*));
      break;
    case 'i':
      push((int)va_arg(vl, int));
      break;
    case 'd':
      push((double)va_arg(vl, double));
      break;
    case 'p':
      push((void*)va_arg(vl, void*));
      break;
    }

    f++;
    args++;
  }

  va_end(vl);

  lua_call(handler, args + 1, nres);

  s32 rnum = lua_gettop(handler) - ssize;

  if (rnum < 1)
    return (void *) -1;

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
#if defined(LUA_VERSION_NUM) && (LUA_VERSION_NUM >= 501)
  lua_pushlightuserdata(handler, arg);
#else
  lua_pushuserdata(handler, arg);
#endif
}

void kgmLuaScript::pop(int* arg)
{
  double dd = 0.0;

  if(lua_isnumber(handler, m_carg))
    dd = lua_tonumber(handler, m_carg);

  *arg = (int) dd;
}

void kgmLuaScript::pop(double* arg)
{
  if(lua_isnumber(handler, m_carg))
    *arg = lua_tonumber(handler, m_carg);
}

void kgmLuaScript::pop(char** arg)
{
  if(lua_isstring(handler, m_carg))
    *arg = (char*) lua_tostring(handler, m_carg);
}

void kgmLuaScript::pop(void** arg)
{
  void* pp = null;

  if(lua_isuserdata(handler, m_carg))
//    pp = (void*) lua_touserdata(handler, m_carg);
    pp = (void*) lua_topointer(handler, m_carg);

  *arg = pp;
}

void kgmLuaScript::init()
{
#if defined(LUA_VERSION_NUM) && (LUA_VERSION_NUM >= 501)
  handler = luaL_newstate();
#else
  handler = lua_open(0);
#endif

  if(!handler)
    return;

  need_close = true;

#if defined(LUA_VERSION_NUM) && (LUA_VERSION_NUM >= 501)
  luaL_openlibs(handler);
#else
  lua_baselibopen (handler);
  lua_iolibopen (handler);
  lua_strlibopen (handler);
  lua_mathlibopen (handler);
  lua_dblibopen (handler);
#endif
}
