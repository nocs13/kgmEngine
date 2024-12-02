#include "lua_main.h"
#include "../kgmLuaScript.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmBase/kgmFile.h"
#include "../../kgmSystem/kgmApp.h"
#include "../../kgmSystem/kgmResources.h"

namespace kgmLibScript
{
  void _lua_gui_init(void* lh);

  s32 xmlFree(void*);
  s32 xmlInit(void*);

  s32 kgmNewEvent(void *lh);
  s32 kgmEventDelete(void *lh);

  s32 kgmGuiShow(void* lh);

  s32 kgmGuiMenuCreate(void* lh);
  s32 kgmGuiMenuItemCreate(void* lh);
  s32 kgmGuiMenuItemAdd(void* lh);

  s32 kgmCreateWindow(void* lh);
  s32 kgmCloseWindow(void* lh);
  s32 kgmFreeWindow(void* lh);
  s32 kgmLoopWindow(void* lh);

  s32 kgmImport(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kgmIResources* res = kgmApp::application()->getResources();

    char* id = null;

    ls.args("s", &id);

    if (id)
    {
      kgm_log() << "Importing: " << id << " script.\n";

      kgmCString path = "scripts/";

      path += id;
      path += ".lua";

      kgmArray<u8> m;

      if (res->getFile(path, m))
      {
        kgmCString sc((char*) m.data(), m.length());

        if (!ls.load(sc))
        {
          kgm_log() << "Error: unable load " << id << " script.\n";
        }
      }
    }

    return 0;
  }

  s32 kgmLog(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    s8* log = null;

    ls.args("s", &log);

    kgm_log() << log << "\n";

    return 0;
  }


  void kgm_lua_init(void* lh)
  {
    if (!lh)
      return;

    _lua_gui_init(lh);

    kgmLuaScript sc((lua_State*) lh);

    sc.set("kgmLog",  kgmLog);
    sc.set("kgmImport",  kgmImport);

    sc.set("kgmXmlInit",  xmlInit);
    sc.set("kgmXmlFree",  xmlFree);

    sc.set("kgmCreateWindow", kgmCreateWindow);
    sc.set("kgmCloseWindow",  kgmCloseWindow);
    sc.set("kgmFreeWindow",   kgmFreeWindow);
    sc.set("kgmLoopWindow",   kgmLoopWindow);

    sc.set("kgmNewEvent",     kgmNewEvent);
    sc.set("kgmEventDelete",  kgmEventDelete);

    sc.set("kgmGuiShow",      kgmGuiShow);

    sc.set("kgmGuiMenuCreate",     kgmGuiMenuCreate);
    sc.set("kgmGuiMenuItemCreate", kgmGuiMenuItemCreate);
    sc.set("kgmGuiMenuItemAdd",    kgmGuiMenuItemAdd);

  }
};