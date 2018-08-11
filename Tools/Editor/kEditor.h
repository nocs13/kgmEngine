#ifndef KEDITOR_H
#define KEDITOR_H

#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGraphics/kgmGuiMenu.h"

#include "kNode.h"
#include "kPivot.h"
#include "kGridline.h"
#include "kFileDialog.h"
#include "kViewObjects.h"

class kEditor: public kgmGameBase
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

  kNode* selected;
  kgmList<kNode*> nodes;

public:
  kEditor();
  ~kEditor();

  void clear();

  kNode* select(int x, int y);

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

  static void callMapOpen(void*);
  static void callMapSave(void*);
  static void callAddMesh(void*);
  static void callAddLight(void*);
  static void callAddActor(void*);
  static void callAddMaterial(void*);
};

#endif // KEDITOR_H
