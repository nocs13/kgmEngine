#include "../../kgmScript/kgmLuaScript.h"
#include "lua_main.h"

namespace Editor
{
  void lua_init(void* lh)
  {
    if (!lh)
      return;

    kgmLuaScript sc((lua_State*) lh);

    sc.set("kCreateEditor", kCreateEditor);
    sc.set("kCloseEditor",  kCloseEditor);

    sc.set("kEditorAddHandler",  kEditorAddHandler);
    sc.set("kEditorDelHandler",  kEditorDelHandler);
  }
};