#include "../../kgmScript/kgmLuaScript.h"

namespace kEditor
{

  void kgm_lua_init(void* lh)
  {
    if (!lh)
      return;

    kgmLuaScript sc((lua_State*) lh);

    sc.set("kgmLog",  kgmLog);
    sc.set("kgmImport",  kgmImport);

    sc.set("kgmXmlInit",  xmlInit);
    sc.set("kgmXmlFree",  xmlFree);
  }
};