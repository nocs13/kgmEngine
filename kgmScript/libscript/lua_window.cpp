#include "../../kgmScript/kgmLuaScript.h"
#include "../../kgmSystem/kgmWindow.h"
#include "../../kgmBase/kgmLog.h"

namespace kgmLibScript
{
  s32 kgmCreateWindow(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    char* name = null;
    s32 width = 1, height = 1;

    ls.args("sii", &name, &width, &height);

    if (name)
    {
      kgm_log() << "Creating: Window " << name << ".\n";

      kgmWindow* wnd = kgmWindow::createGLWindow(name, width, height);

      if (wnd)
      {
        wnd->show(true);

        ls.resl("p", wnd);

        return 1;
      }
    }

    return 0;
  }

  s32 kgmCloseWindow(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kgmWindow* w = null;

    ls.args("p", &w);

    if (w)
    {
      kgm_log() << "Closing: Window.\n";

      if (w)
      {
        w->close();
      }
    }

    return 0;
  }

  s32 kgmLoopWindow(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kgmWindow* w = null;

    ls.args("p", &w);

    if (w)
    {
      kgm_log() << "Looping: Window.\n";

      if (w)
      {
        w->loop();
      }
    }

    return 0;
  }
};