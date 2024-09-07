#include "../kgmIScript.h"
#include "../../kgmBase/kgmXml.h"

namespace kgmLibScript
{
  static kgmIScript* script = nullptr;

  s32 xmlInit(void*)
  {
    kgmXml* x = new kgmXml();

    script->resl("p", x);

    return 1;
  }

  s32 xmlFree(void*)
  {
    kgmXml* x = nullptr;

    script->args("p", &x);

    if (x)
      x->close();

    kgm_free(x);

    return 0;
  }

  s32 init(kgmIScript* is)
  {
    if (!is)
      return 0;

    script = is;

    is->set("kgmXmlInit",  xmlInit);
    is->set("kgmXmlFree",  xmlFree);

    return 1;
  }

};
