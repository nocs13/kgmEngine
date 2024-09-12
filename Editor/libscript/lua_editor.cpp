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
};