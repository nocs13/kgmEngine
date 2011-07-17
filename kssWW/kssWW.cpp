#include "kssWW.h"
#include "kGame.h"

#pragma comment(lib, "../kgmGame/kgmGame.lib")
#pragma comment(lib, "../kgmMain/kgmMain.lib")
#pragma comment(lib, "../kgmAudio/kgmAudio.lib")
#pragma comment(lib, "../kgmGraphics/kgmGraphics.lib")
#pragma comment(lib, "../kgmPhysics/kgmPhysics.lib")
#pragma comment(lib, "../kgmGraphics/kgmGraphics.lib")
#pragma comment(lib, "../kgmExtention/kgmExtention.lib")

#pragma comment(lib, "../kgmGame/kgmGame.lib")
#pragma comment(lib, "../kgmDevice/kgmDevice.lib")
#pragma comment(lib, "../kgmExtention/kgmExtention.lib")
#pragma comment(lib, "../kgmGraphics/kgmGraphics.lib")
#pragma comment(lib, "../kgmLua/lua.lib")
#pragma comment(lib, "../kgmOpenAL/OpenAL32.lib")

#pragma comment(lib, "../kgmLua/lua.lib")

#include "../kgmMain/kgmOGL.h"

#include "../kgmScript/inc/lua.hpp"
#include "../kgmScript/kgmLua.h"
#include "../kgmGame/kgmGameLua.h"
/*
class kssLua{
public:
 static const char class_name[];
 static const kgmLuaBind<kssLua>::Reg Register[];
 kssLua(lua_State*){
  printf("new\n");
 }
 ~kssLua(){
  printf("delete \n");
 }
 int print(lua_State* l){
  printf(lua_tostring(l, 2));
  printf("tt");
  return 0;
 }
};

const char kssLua::class_name[] = "kLua";
const kgmLuaBind<kssLua>::Reg kssLua::Register[] = {
 {"print", &kssLua::print},
 {0, 0}
};
*/
kssWW::kssWW(){
}

kssWW::~kssWW(){
}

void kssWW::main(){
 //kGame*	game = new kGame();
 //game->fullscreen(true);
 //game->loop();
 //kgmWindow 	wnd;
 //wnd.create(0, "w", 0, 0, 500, 500, 24, true);
 //wnd.fullscreen(true);
 //kgmOGL ogl(&wnd);
 //wnd.loop();
 //game->release();
 //kgmGameLua* lg = new kgmGameLua();
 //kgmLuaBind<kssLua>::init(lg->m_lua);
 //lg->fnRegister("print", (void*)kprint);
 //luaopen_io(lg->m_lua);
 //if(!lg->load("main.lua"))
  //printf("lua error: %s\n", lua_tostring(lg->m_lua, -1));
 //if(!lg->call("main", 0, 0))
  //printf("lua error: %s\n", lua_tostring(lg->m_lua, -1));
 //delete lg;
 kGame g;
}

kssWW app;
