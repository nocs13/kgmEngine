#include "../../kgmScript/kgmLuaScript.h"
#include "../../kgmSystem/kgmWindow.h"
#include "../../kgmBase/kgmLog.h"
#include "../kEditor.h"

namespace Editor
{
  s32 kCreateEditor(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kgmWindow* wnd = null;

    ls.args("p", &wnd);

    if (wnd)
    {
      kgm_log() << "Creating: Editor.\n";

      kEditor* e = new ::kEditor(wnd);

      if (wnd)
      {
        ls.resl("p", e);

        return 1;
      }
    }

    return 0;
  }

  s32 kCloseEditor(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kEditor* e = null;

    ls.args("p", &e);

    if (e)
    {
      kgm_log() << "Closing: Editor.\n";

      e->release();
    }

    return 0;
  }

  s32 kEditorAddHandler(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kEditor* e = null;
    kgmEvent* ev = null;

    ls.args("pp", &e, &ev);

    if (e && ev)
    {
      e->addHandler(ev);
    }

    return 0;
  }

  s32 kEditorDelHandler(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kEditor* e = null;
    kgmEvent* ev = null;

    ls.args("pp", &e, &ev);

    if (e && ev)
    {
      e->remHandler(ev);
    }

    return 0;
  }

  s32 kEditorGuiAdd(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kEditor* e = null;
    kgmGui* g = null;

    ls.args("pp", &e, &g);

    if (e && g)
    {
      if (e->addGui(g))
      {
        ls.resl("i", 1);

        return 1;

      }
    }

    return 0;
  }

  s32 kEditorSetViewTop(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kEditor* e = null;

    ls.args("p", &e);

    if (e)
    {
      e->onViewTop();
    }

    return 0;
  }

  s32 kEditorSetViewLeft(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kEditor* e = null;

    ls.args("p", &e);

    if (e)
    {
      e->onViewLeft();
    }

    return 0;
  }

  s32 kEditorSetViewFront(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kEditor* e = null;

    ls.args("p", &e);

    if (e)
    {
      e->onViewFront();
    }

    return 0;
  }

  s32 kEditorSetViewPerspective(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kEditor* e = null;

    ls.args("p", &e);

    if (e)
    {
      e->onViewPerspective();
    }

    return 0;
  }

};