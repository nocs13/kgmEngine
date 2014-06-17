#ifndef KEDITOR_H
#define KEDITOR_H

#include "../../kgmGraphics/kgmGuiMenu.h"

#include "kNode.h"
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

  class kFDD: public kFileDialog
  {
    kEditor* editor;

  public:
    kFDD(kEditor* e)
    {
      editor = e;
    }

    void onOpen()
    {
      editor->onAction(this, 1);
    }

    void onSave()
    {
      editor->onAction(this, 2);
    }

    void onFail()
    {
      editor->onAction(this, 0);
    }
  };

private:

  kgmGameBase* game;

  bool ms_click[3];
  vec3 cam_pos;
  f32  cam_rot;

  kGridline* gridline;

  kPivot*    pivot;
  vec3       pv_pos;
  vec3       pv_rot;

  kMenu*     menu;

  kFDD*      fdd;

  u32        oquered;

  kViewObjects* vo;

  kViewOptions* vop;


  kNode* selected;
  kgmList<kNode*> nodes;

public:
  kEditor(  kgmGameBase* game);
  ~kEditor();

  void clear();

  void select(int x, int y);
  void select(kgmString name);

  kgmRay3d<float> getPointRay(int x, int y);

  bool mapSave(kgmString);
  bool mapOpen(kgmString);
  bool addMesh(kgmString);
  bool addActor(kgmString);
  bool addMaterial(kgmString);

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
  void onAddTrigger();
  void onAddMaterial();
  void onViewObjects();
  void onOptionsDatabase();
  void onRunRun();

  void onCloseVop();
};

}

#endif // KEDITOR_H
