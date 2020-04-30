#include "../kgmScript/kgmIScript.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmMap.h"
#include "../kgmBase/kgmList.h"

class kgmIGame;
class kgmUnit;
class kgmGui;

class kgmGameScript: public kgmObject
{
  kgmIScript* handler;
  kgmIGame*   game;

  bool status = false;

  kgmMap<kgmGui*, kgmString> slotters;
  kgmList< Slot<kgmGameScript>* > slots;

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

  bool getStatus() const
  {
    return status;
  }

  __stdcall void onQuit();
  __stdcall void onLoad();
  __stdcall void onUnload();
  __stdcall void onInsert(kgmUnit*);
  __stdcall void onRemove(kgmUnit*);

  __stdcall void onSlotGuiMenu(kgmGui*, u32);
  __stdcall void onSlotGuiList(kgmGui*, u32);
  __stdcall void onSlotGuiText(kgmGui*, kgmString);
  __stdcall void onSlotGuiCheck(kgmGui*, bool);
  __stdcall void onSlotGuiButton(kgmGui*, int);

 private:
  void free();

  static s32 kgmLog(void*);

  static s32 kgmImport(void*);

  static s32 kgmGameExit(void*);
  static s32 kgmGameLoad(void*);
  static s32 kgmGameEdit(void*);
  static s32 kgmGamePlay(void*);
  static s32 kgmGamePause(void*);
  static s32 kgmGameState(void*);

  static int kgmGuiLoad(void*);
  static s32 kgmGuiShow(void*);
  static s32 kgmGuiResize(void*);
  static s32 kgmGuiResolution(void*);
  static s32 kgmGuiSetVAlign(void*);
  static s32 kgmGuiSetHAlign(void*);

  static s32 kgmScreenResolution(void*);

  static s32 kgmGoFind(void*);
  static s32 kgmGoCamera(void*);
};
