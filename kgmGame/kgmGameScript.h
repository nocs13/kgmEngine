#pragma once

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
  bool play   = false;

  kgmMap< kgmGui*, kgmString > slotters;
  kgmList< Slot<kgmGameScript>* > slots;

  static char* value;

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

  void setPlay(bool p) { play = p; }
  bool getPlay()       { return play; }

  __stdcall void onQuit();
  __stdcall void onLoad();
  __stdcall void onPlay();
  __stdcall void onPause();
  __stdcall void onUnload();
  __stdcall void onInsert(kgmUnit*);
  __stdcall void onRemove(kgmUnit*);

  __stdcall void onSlotGuiMenu(kgmGui*, u32);
  __stdcall void onSlotGuiList(kgmGui*, u32);
  __stdcall void onSlotGuiText(kgmGui*, kgmString);
  __stdcall void onSlotGuiCheck(kgmGui*, bool);
  __stdcall void onSlotGuiButton(kgmGui*, int);

 private:
  void clear();

  static s32 kgmLog(void*);

  static s32 kgmImport(void*);

  static s32 kgmGameExit(void*);
  static s32 kgmGameLoad(void*);
  static s32 kgmGameEdit(void*);
  static s32 kgmGamePlay(void*);
  static s32 kgmGamePause(void*);
  static s32 kgmGameState(void*);
  static s32 kgmGameUnload(void*);
  static s32 kgmGameCamera(void*);
  static s32 kgmGameSetState(void*);

  static s32 kgmCamMove(void*);
  static s32 kgmCamLook(void*);
  static s32 kgmCamPosition(void*);
  static s32 kgmCamDirection(void*);

  static s32 kgmMtlGet(void*);
  static s32 kgmMtlSetType(void*);
  static s32 kgmMtlSetMaps(void*);
  static s32 kgmMtlSetAlpha(void*);
  static s32 kgmMtlSetColor(void*);

  static s32 kgmLightGet(void*);
  static s32 kgmLightSetType(void*);
  static s32 kgmLightSetForce(void*);
  static s32 kgmLightSetColor(void*);

  static s32 kgmMeshGet(void*);

  static s32 kgmUnitId(void*);
  static s32 kgmUnitName(void*);
  static s32 kgmUnitClass(void*);
  static s32 kgmUnitCreate(void*);
  static s32 kgmUnitRemove(void*);
  static s32 kgmUnitSetState(void*);
  static s32 kgmUnitGetState(void*);
  static s32 kgmUnitSetValue(void*);
  static s32 kgmUnitGetValue(void*);
  static s32 kgmUnitSetBody(void*);
  static s32 kgmUnitSetMesh(void*);
  static s32 kgmUnitAddMesh(void*);
  static s32 kgmUnitRemMesh(void*);
  static s32 kgmUnitSetLight(void*);
  static s32 kgmUnitSetPosition(void*);
  static s32 kgmUnitGetPosition(void*);
  static s32 kgmUnitSetRotation(void*);
  static s32 kgmUnitGetRotation(void*);

  static s32 kgmUnitIterator(void*);
  static s32 kgmUnitIterNext(void*);
  static s32 kgmUnitIterFree(void*);

  static s32 kgmGenRetention(void*);
  static s32 kgmDelRetention(void*);
  static s32 kgmGetRetention(void*);
  static s32 kgmSetRetention(void*);

  static s32 kgmGcSetBgColor(void*);

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
