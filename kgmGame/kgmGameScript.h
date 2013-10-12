#include "../kgmBase/kgmString.h"
#ifdef LUA
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//extern lua_State* lua_main;

bool kgmLuaOpen();
void kgmLuaClose();
bool kgmLuaLoad(kgmString str);
bool kgmLuaRun(kgmString str);
bool kgmLuaSet(kgmString name, void* fn);
bool kgmLuaCall(kgmString name, kgmString fmt, ...);
int kgmLuaToInteger(int num_arg);
double kgmLuaToDouble(int num_arg);
char* kgmLuaToString(int num_arg);
void* kgmLuaToPointer(int num_arg);
void kgmLuaPush(int arg);
void kgmLuaPush(double arg);
void kgmLuaPush(char* arg);
void kgmLuaPush(void* arg);

#endif
