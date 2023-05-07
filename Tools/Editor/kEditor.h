#ifndef KEDITOR_H
#define KEDITOR_H

#include "../../kgmBase/kgmSettings.h"

#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGame/kgmGameGraphics.h"
#include "../../kgmSystem/kgmThread.h"
#include "../../kgmGraphics/kgmGuiMenu.h"
#include "../../kgmGraphics/kgmGuiFileDialog.h"

#include "kLine.h"
#include "kArrow.h"
#include "kPivot.h"
#include "kGridline.h"
#include "kViewObjects.h"
#include "kViewOptions.h"
#include "kResources.h"
#include "kWindow.h"

#include <gtk/gtk.h>

/*
 * Deprecated. Should move as independent script based engine editor.
*/

class kEditor: public kgmObject
{
private:

  enum
  {
    ViewPerspective,
    ViewFront,
    ViewLeft,
    ViewTop
  };

  bool ms_click[3];

  vec3 cam_pos;
  f32  cam_rot;

  vec3 cam_pos_bk;
  vec3 cam_dir_bk;

  kgmList<kgmUnit*> units;

  kGridline* gridline = null;
  //kPivot*    pivot    = null;
  kgmUnit*   pivot    = null;

  vec3  pv_pos;
  vec3  pv_rot;

  float pv_delta;

  kgmGuiMenu* menu = null;

  u32  oquered;
  u32  view_mode;

  kgmUnit* selected = null;
  kgmUnit* dragging = null;

  kgmMaterial* mtlLines = null;
  kgmMaterial* mtlPivot = null;

  kgmCamera* camera = null;
  bool  move_camera;

  bool mode_play;

  bool              m_isVisual;
  kgmThread::Thread m_thVisual;

  kgmList<kgmObject*> m_objects;

  Slot<kEditor, u32>               slotMenu;
  Slot<kEditor, kgmGuiFileDialog*> slotMapSave;
  Slot<kEditor, kgmGuiFileDialog*> slotMapOpen;
  Slot<kEditor, kgmGuiFileDialog*> slotOpenFile;
  Slot<kEditor, kgmString>         slotSelect;

  kgmIGC*        m_gc;
  kgmIGraphics*  m_graphics;
  kgmIResources* m_resources;
  kgmISettings*  m_settings;

  kWindow*       m_wnd;

public:
  kEditor(GtkWidget* w);
  ~kEditor();

  int            gQuit();
  int            gInit();
  int            gLoad(kgmString){}
  int            gUnload(){}
  int            gButton(game_button);
  u32            gState();
  int            gSwitch(u32);
  int            gAppend(kgmUnit*) {}
  void           gPause(bool);

  void setMsAbsolute(bool) {}
  kgmIGraphics* getRender() { return m_graphics; }

  void clear();
  void init();
  void loop() {}
  void close() {}

  void select(int x, int y);
  void select(kgmString name);

  s32 getKeyState(u8 key)  { return 0; }


  kgmRay3d<float> getPointRay(int x, int y);

  __stdcall bool fdMapSave(kgmGuiFileDialog*);
  __stdcall bool fdMapOpen(kgmGuiFileDialog*);
  __stdcall bool fdAddMesh(kgmGuiFileDialog*);
  __stdcall bool addUnit(kgmString);
  __stdcall bool addActor(kgmGuiFileDialog*);
  __stdcall bool addEffect(kgmString);
  __stdcall bool addSensor(kgmString);

  bool mapSave(kgmString);
  bool mapOpen(kgmString);
  bool addMesh(kgmString);

  void initResources() {}
  void initGraphycs() {}
  void initPhysics();
  void initSystem() {}
  void initLogic();
  void initAudio() {}

  u32  timeUpdate() { return 1; }
  kgmISettings* getSettings() { return m_settings; }

  void onIdle();
  void onEvent(kgmEvent::Event*);
  void onAction(kgmEvent*,int);
  void onKeyUp(int k);
  void onKeyDown(int k);
  void onMsMove(int k, int x, int y);
  void onMsWheel(int k, int x, int y, int z);
  void onMsLeftUp(int k, int x, int y);
  void onMsLeftDown(int k, int x, int y);
  void onMsRightUp(int k, int x, int y);
  void onMsRightDown(int k, int x, int y);

  kgmMaterial* getMaterial(kgmString id);

  kgmList<kgmObject*>& getObjects()
  {
    return m_objects;
  }

  void update();

  __stdcall void onQuit();
  __stdcall void onEditClone();
  __stdcall void onEditRemove();
  __stdcall void onEditOptions();
  __stdcall void onMapNew();
  __stdcall void onMapOpen();
  __stdcall void onMapSave();
  __stdcall void onAddText();
  __stdcall void onAddBox();
  __stdcall void onAddPlane();
  __stdcall void onAddSphere();
  __stdcall void onAddCylinder();
  __stdcall void onAddMesh();
  __stdcall void onAddUnit();
  __stdcall void onAddLight();
  __stdcall void onAddActor();
  __stdcall void onAddEffect();
  __stdcall void onAddSprite();
  __stdcall void onAddSensor();
  __stdcall void onAddTrigger();
  __stdcall void onAddObstacle();
  __stdcall void onAddMaterial();
  __stdcall void onAddParticles();
  __stdcall void onViewObjects();
  __stdcall void onViewMaterials();
  __stdcall void onViewPerspective();
  __stdcall void onViewFront();
  __stdcall void onViewLeft();
  __stdcall void onViewTop();
  __stdcall void onOptionsDatabase();
  __stdcall void onRunPlay();
  __stdcall void onRunStop();
  __stdcall void onSelectObject(kgmString);
  __stdcall void onSelectMaterial(kgmString);
  __stdcall void onCloseVop();
  void onMenu(u32 id);

private:
  void add(kgmUnit*);
  void remove(kgmUnit*);

  static int doVisUpdate(void*);
};

#endif // KEDITOR_H
