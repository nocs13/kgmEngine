#ifndef KEDITOR_H
#define KEDITOR_H

#include "../../kgmGame/kgmGameBase.h"
#include "../../kgmGraphics/kgmGuiMenu.h"

#include "kPivot.h"
#include "kGridline.h"
#include "kFileDialog.h"

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

private:
  bool ms_click[3];
  vec3 cam_pos;
  f32  cam_rot;

  kGridline* gridline;
  kPivot*    pivot;

  kMenu*     menu;

  kFileDialog* fdd;
public:
  kEditor();
  ~kEditor();

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
};

#endif // KEDITOR_H
