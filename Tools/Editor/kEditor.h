#ifndef KEDITOR_H
#define KEDITOR_H

#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGraphics/kgmGuiMenu.h"

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

  class Node
  {
  public:
    enum Type
    {
      NONE,
      MESH,
      LIGHT,
      ACTOR,
      MATERIAL
    };

    union
    {
      kgmMesh*      msh;
      kgmLight*     lgt;
      kgmActor*     act;
      kgmMaterial*  mtl;
    };

    Type typ;
    vec3 pos;
    vec3 rot;
    box3 bnd;
    kgmString nam;

    Node()
    {
      typ = NONE;
      msh = null;
    }

    Node(kgmMesh* m)
    {
      typ = MESH;
      msh = m;
    }

    Node(kgmLight* l)
    {
      typ = LIGHT;
      lgt = l;
    }

    Node(kgmActor* a)
    {
      typ = ACTOR;
      act = a;
    }

    Node(kgmMaterial* m)
    {
      typ = MATERIAL;
      mtl = m;
    }

    ~Node()
    {
      kgmObject* o = (kgmObject*)msh;

      if(o)
        o->release();
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
  u32        fddMode;

  u32        oquered;

  kViewObjects* vo;

  Node* selected;
  kgmList<Node*> nodes;

public:
  kEditor();
  ~kEditor();

  Node* select(int x, int y);

  bool mapSave(kgmString);
  bool mapOpen(kgmString);

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
  void onMapOpen();
  void onMapSave();
  void onAddMesh();
  void onViewObjects();
};

#endif // KEDITOR_H
