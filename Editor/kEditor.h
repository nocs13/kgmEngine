#ifndef KEDITOR_H
#define KEDITOR_H

#include "../kgmBase/kgmSettings.h"

#include "../kgmGame/kgmGameBase.h"
#include "../kgmGame/kgmGameGraphics.h"
#include "../kgmSystem/kgmOGL.h"
#include "../kgmSystem/kgmThread.h"
#include "../kgmSystem/kgmWindow.h"
#include "../kgmSystem/kgmResources.h"
#include "../kgmGraphics/kgmGuiMenu.h"
#include "../kgmGraphics/kgmGuiFileDialog.h"

#include "kLine.h"
#include "kArrow.h"
#include "kPivot.h"
#include "kGridline.h"
#include "kViewObjects.h"
#include "kViewOptions.h"
#include "kResources.h"

/*
 * Deprecated. Should move as independent script based engine editor.
 */

class kEditor : public kgmEvent
{
public:
enum MENUEVENT
{
  ME_QUIT,
  ME_MAP_NEW,
  ME_MAP_OPEN,
  ME_MAP_SAVE,
  ME_EDIT_CLONE,
  ME_EDIT_REMOVE,
  ME_EDIT_OPTIONS,
  ME_ADD_BOX,
  ME_ADD_PLANE,
  ME_ADD_SPHERE,
  ME_ADD_CYLINDER,
  ME_ADD_MESH,
  ME_ADD_UNIT,
  ME_ADD_LIGHT,
  ME_ADD_ACTOR,
  ME_ADD_EFFECT,
  ME_ADD_SENSOR,
  ME_ADD_TRIGGER,
  ME_ADD_PARTICLES,
  ME_ADD_MATERIAL,
  ME_RUN_PLAY,
  ME_RUN_STOP,
  ME_VIEW_OBJECTS,
  ME_VIEW_MATERIALS,
  ME_VIEW_PERSPECTIVE,
  ME_VIEW_FRONT,
  ME_VIEW_BACK,
  ME_VIEW_TOP,
  ME_OPTIONS_DATABASE,
  ME_HELP_ABOUT
};

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
  f32 cam_rot;

  vec3 cam_pos_bk;
  vec3 cam_dir_bk;

  kgmList<kgmUnit *> units;

  kGridline *gridline = null;
  kPivot *pivot = null;

  vec3 pv_pos;
  vec3 pv_rot;

  float pv_delta;

  kgmGuiMenu *menu = null;

  u32 oquered;
  u32 view_mode;

  kgmUnit *selected = null;
  kgmUnit *dragging = null;

  kgmMaterial *mtlLines = null;
  kgmMaterial *mtlPivot = null;

  kgmCamera *camera = null;
  bool move_camera;

  bool mode_play;

  bool m_isVisual;
  kgmThread::Thread m_thVisual;

  kgmList<kgmObject *> m_objects;

  Slot<kEditor, u32> slotMenu;
  Slot<kEditor, kgmGuiFileDialog *> slotMapSave;
  Slot<kEditor, kgmGuiFileDialog *> slotMapOpen;
  Slot<kEditor, kgmGuiFileDialog *> slotOpenFile;
  Slot<kEditor, kgmString> slotSelect;

  kgmIGC       *m_gc;

  kgmResources *m_resources;
  kgmSettings  *m_settings;
  kgmGraphics  *m_graphics;

  kgmWindow *m_wnd;

public:
  kEditor(kgmWindow *w);
  ~kEditor();

  int quit();
  int gInit();
  int gLoad(kgmString) { return 0; }
  int gUnload() { return 0; }
  int gButton(game_button);
  u32 gState();
  int gSwitch(u32);
  void gPause(bool);

  void setMsAbsolute(bool) {}
  kgmIGraphics *getRender() { return m_graphics; }

  void clear();
  void init();
  void loop() {}
  void prepare();
  void resize(float w, float h);

  kgmUnit *unit(kgmString name);
  void append(kgmUnit *);

  void select(int x, int y);
  void select(kgmString name);

  s32 getKeyState(u8 key) { return 0; }

  kgmRay3d<float> getPointRay(int x, int y);

  __stdcall bool fdMapSave(kgmGuiFileDialog *);
  __stdcall bool fdMapOpen(kgmGuiFileDialog *);
  __stdcall bool fdAddMesh(kgmGuiFileDialog *);
  __stdcall bool addUnit(kgmString);
  __stdcall bool addActor(kgmGuiFileDialog *);
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

  u32 timeUpdate() { return 1; }
  kgmISettings *getSettings() { return m_settings; }

  void onIdle();
  void onClose();
  void onEvent(kgmEvent::Event *);
  void onResize(int w, int h);
  void onAction(kgmEvent *, int);
  void onKeyUp(int k);
  void onKeyDown(int k);
  void onMsMove(int k, int x, int y);
  void onMsWheel(int k, int x, int y, int z);
  void onMsLeftUp(int k, int x, int y);
  void onMsLeftDown(int k, int x, int y);
  void onMsRightUp(int k, int x, int y);
  void onMsRightDown(int k, int x, int y);

  kgmMaterial *getMaterial(kgmString id);

  kgmList<kgmObject *> &getObjects()
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
  void add(kgmUnit *);
  void remove(kgmUnit *);

  static int doVisUpdate(void *);
};

#endif // KEDITOR_H
