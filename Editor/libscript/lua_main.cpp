#include "../../kgmScript/kgmLuaScript.h"
#include "lua_main.h"

namespace Editor
{
  void kgm_lua_init(void* lh)
  {
    if (!lh)
      return;

    kgmLuaScript sc((lua_State*) lh);

    sc.set("kCreateEditor", kCreateEditor);
    sc.set("kCloseEditor",  kCloseEditor);
  }
};