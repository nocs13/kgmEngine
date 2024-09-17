#include "../../kgmScript/kgmLuaScript.h"
#include "../../kgmSystem/kgmWindow.h"
#include "../../kgmBase/kgmEvent.h"
#include "../kEditor.h"

namespace Editor
{
  class kgmLuaEvent : public kgmEvent
  {
    KGM_OBJECT(kgmLuaEvent)
    lua_State *lua;
    s8* name;
  public:
    kgmLuaEvent(lua_State* l, const s8* n) { lua = l; name = (s8*) n; }
    ~kgmLuaEvent() {}

    void onEvent(kgmEvent::Event *e)
    {
      kgmLuaScript ls(lua);

      if (!name)
        return;

      switch (e->event)
      {
      case evtCreate:
        ls.ccall(name, "onCreate", "");
        break;
      case evtClose:
        ls.ccall(name, "onClose", "");
        break;
      case evtPaint:
        ls.ccall(name, "onPaint", "");
        break;
      case evtResize:
        ls.ccall(name, "onResize", "ii", e->width, e->height);
        break;
      case evtMsWheel:
        ls.ccall(name, "onMsWheel", "iii", e->keyMask, e->msx, e->msy, e->msz);
        break;
      case evtMsMove:
        ls.ccall(name, "onMsMove", "iii", e->keyMask, e->msx, e->msy);
        break;
      case evtMsLeftDown:
        ls.ccall(name, "onMsDown", "iiii", 1, e->keyMask, e->msx, e->msy);
        break;
      case evtMsLeftUp:
        ls.ccall(name, "onMsUp", "iiii", 1, e->keyMask, e->msx, e->msy);
        break;
      case evtMsRightDown:
        ls.ccall(name, "onMsDown", "iiii", 2, e->keyMask, e->msx, e->msy);
        break;
      case evtMsRightUp:
        ls.ccall(name, "onMsUp", "iiii", 2, e->keyMask, e->msx, e->msy);
        break;
      case evtKeyDown:
        ls.ccall(name, "onKeyDown", "i", e->key);
        break;
      case evtKeyUp:
        ls.ccall(name, "onKeyUp", "i", e->key);
        break;
      }
    }
  };

  s32 kgmNewEvent(void *lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State *)lh);

    const s8 *n = null;

    ls.args("s", &n);

    if (n)
    {
      kgmLuaEvent* le = new kgmLuaEvent((lua_State *) lh, n);

      ls.resl("p", le);

      return 1;
    }

    return 0;
  }

  s32 kgmEventDelete(void *lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State *)lh);

    kgmLuaEvent *le = null;

    ls.args("s", &le);

    if (le)
    {
      le->release();

      ls.resl("i", 1);

      return 1;
    }

    return 0;
  }

};