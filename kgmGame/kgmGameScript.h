#include "../kgmScript/kgmIScript.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmMap.h"
#include "../kgmBase/kgmList.h"

class kgmIGame;
class kgmGui;

class kgmGameScript: public kgmObject
{
  kgmIScript* handler;
  kgmIGame*   game;

  kgmMap<kgmGui*, kgmString> slotters;

 public:
  kgmGameScript(kgmIGame*);
  ~kgmGameScript();

  void init();
  void update();

  void setSlot(kgmGui*, kgmString);

  kgmIScript* getHandler() const
  {
    return handler;
  }

  __stdcall void onSlotGuiMenu(kgmGui*, u32);
  __stdcall void onSlotGuiList(kgmGui*, u32);
  __stdcall void onSlotGuiText(kgmGui*, kgmString);
  __stdcall void onSlotGuiCheck(kgmGui*, bool);
  __stdcall void onSlotGuiButton(kgmGui*, int);

 private:
  void free();

  static void kgmLog(void*);
  static void kgmImport(void*);
  static void kgmGameExit(void*);
  static void kgmGameLoad(void*);
  static void kgmGameEdit(void*);
  static void kgmGamePlay(void*);
  static void kgmGamePause(void*);
  static void kgmGameState(void*);
  static void kgmGuiLoad(void*);
  static void kgmGuiShow(void*);
};
