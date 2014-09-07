#ifndef KEDITOR_H
#define KEDITOR_H

#include "../../kgmGraphics/kgmGuiMenu.h"

#include "kNode.h"
#include "kArrow.h"
#include "kPivot.h"
#include "kGridline.h"
#include "kFileDialog.h"
#include "kViewObjects.h"
#include "kViewOptions.h"

class kgmGameBase;

namespace kgmGameEditor
{

class kEditor: public kgmEvent
{
  class kMenu: public kgmGuiMenu
  {
    kEditor* editor;

  public:
    kMenu(kgmGui* parent, kEditor* e)
      :kgmGuiMenu(parent)
    {
      editor = e;
    }

    void onAction(kgmGui* gui, u32 arg)
    {
      if(editor)
      {
        editor->onAction(this, arg);
      }
    }
  };

private:

  kgmGameBase* game;

  bool ms_click[3];
  bool ctrl_left;

  vec3 cam_pos;
  f32  cam_rot;

  kGridline* gridline;

  kPivot*    pivot;
  vec3       pv_pos;
  vec3       pv_rot;

  kMenu*     menu;

  kFileDialog* fdd;

  u32        oquered;

  kViewObjects* vo;   //view objects
  kViewObjects* vs;   //view sensors

  kViewOptions* vop;


  kNode* selected;
  kgmList<kNode*> nodes;

  kgmMaterial* mtlLines;

  kgmVisual*     logView;

public:
  explicit kEditor(  kgmGameBase* game);
  ~kEditor();

  void clear();

  void select(int x, int y);
  void select(kgmString name);

  kgmRay3d<float> getPointRay(int x, int y);

  bool mapSave(kgmString);
  bool mapOpen(kgmString);
  bool addMesh(kgmString);
  bool addActor(kgmString);
  bool addSensor(kgmString);
  bool addObject(kgmString);

  void menuAddActor();
  void menuAddSensor();

  void initPhysics();
  void initLogic();

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

  void onQuit();
  void onEditRemove();
  void onEditDuplicate();
  void onEditOptions();
  void onMapOpen();
  void onMapSave();
  void onAddMesh();
  void onAddLight();
  void onAddActor();
  void onAddSensor();
  void onAddObject();
  void onAddTrigger();
  void onViewObjects();
  void onOptionsDatabase();
  void onRunRun();
  void onSelectObject();

  void onCloseVop();
};

}

#endif // KEDITOR_H
