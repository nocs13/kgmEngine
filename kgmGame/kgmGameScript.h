#include "../kgmScript/kgmIScript.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"

class kgmIGame;

class kgmGameScript: public kgmObject
{
  kgmIScript* handler;
  kgmIGame*   game;

 public:
  kgmGameScript(kgmIGame*);
  ~kgmGameScript();
};
