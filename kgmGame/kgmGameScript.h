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

  void init();
  void update();

  kgmIScript* getHandler() const
  {
    return handler;
  }

private:
  void free();

  static void kgmLog(void*);
  static void kgmGameExit(void*);
  static void kgmGameLoad(void*);
  static void kgmGameEdit(void*);
  static void kgmGamePlay(void*);
  static void kgmGamePause(void*);
  static void kgmGameState(void*);
  static void kgmGuiLoad(void*);
  static void kgmGuiShow(void*);
};
