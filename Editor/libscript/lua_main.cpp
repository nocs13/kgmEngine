#include "../../kgmScript/kgmLuaScript.h"
#include "lua_main.h"

namespace Editor
{
  s32 kEditorSetViewTop(void* lh);
  s32 kEditorSetViewLeft(void* lh);
  s32 kEditorSetViewFront(void* lh);
  s32 kEditorSetViewPerspective(void* lh);

  void lua_init(void* lh)
  {
    if (!lh)
      return;

    kgmLuaScript sc((lua_State*) lh);

    sc.set("kCreateEditor", kCreateEditor);
    sc.set("kCloseEditor",  kCloseEditor);

    sc.set("kEditorAddHandler",  kEditorAddHandler);
    sc.set("kEditorDelHandler",  kEditorDelHandler);

    sc.set("kEditorGuiAdd",  kEditorGuiAdd);

    sc.set("kEditorSetViewTop",  kEditorSetViewTop);
    sc.set("kEditorSetViewLeft",  kEditorSetViewLeft);
    sc.set("kEditorSetViewFront",  kEditorSetViewFront);
    sc.set("kEditorSetViewPerspective",  kEditorSetViewPerspective);
  }
};