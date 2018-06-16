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

  void update();

  kgmIScript* getHandler() const
  {
    return handler;
  }

private:
  static void kgmGameExit(void*);
  static void kgmGamePlay(void*);
  static void kgmGamePause(void*);
  static void kgmGameState(void*);
};
