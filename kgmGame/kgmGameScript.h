#include "../kgmScript/kgmIScript.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmMap.h"
#include "../kgmSystem/kgmThread.h"

class kgmIGame;
class kgmUnit;
class kgmGui;

class kgmGameScript: public kgmObject
{
  KGM_OBJECT(kgmGameScript)

  kgmIScript* handler;
  kgmIGame*   game;

  bool status = false;

  kgmMap< kgmGui*, kgmString > slotters;
  kgmList< Slot<kgmGameScript>* > slots;

  static char* value;

  kgmThread::Mutex mutex;
  kgmThread::TID   mlocker;

  bool log_lock = true;
  s32  locks[2] = {0};

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

  void lock();
  void unlock();

  void onButton(s32 key, s32 btn, s32 down);

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

  bool locked();

  static s32 kgmLog(void*);

  static s32 kgmImport(void*);

  static s32 kgmGameExit(void*);
  static s32 kgmGameLoad(void*);
  static s32 kgmGameEdit(void*);
  static s32 kgmGamePlay(void*);
  static s32 kgmGamePause(void*);
  static s32 kgmGameState(void*);
  static s32 kgmGameUnits(void*);
  static s32 kgmGameUnload(void*);
  static s32 kgmGameCamera(void*);

  static s32 kgmCamMove(void*);
  static s32 kgmCamLook(void*);
  static s32 kgmCamPosition(void*);
  static s32 kgmCamDirection(void*);

  static s32 kgmUnitId(void*);
  static s32 kgmUnitName(void*);
  static s32 kgmUnitClass(void*);

  static s32 kgmGenRetention(void*);
  static s32 kgmDelRetention(void*);
  static s32 kgmGetRetention(void*);
  static s32 kgmSetRetention(void*);

  static s32 kgmGuiLoad(void*);
  static s32 kgmGuiShow(void*);
  static s32 kgmGuiResize(void*);
  static s32 kgmGuiResolution(void*);
  static s32 kgmGuiGetChild(void*);
  static s32 kgmGuiCallback(void*);
  static s32 kgmGuiCreate(void*);
  static s32 kgmGuiAdd(void*);
  static s32 kgmGuiSetText(void*);
  static s32 kgmGuiSetWindow(void*);
  static s32 kgmGuiSetCallback(void*);

  static s32 kgmScreenResolution(void*);

  static s32 kgmKeyState(void*);
  static s32 kgmPointState(void*);

  static s32 kgmRunProcess(void*);
};
