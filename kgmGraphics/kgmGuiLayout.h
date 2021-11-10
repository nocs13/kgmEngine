#ifndef KGMGUILAYOUT_H
#define KGMGUILAYOUT_H

#include "kgmGui.h"

class kgmGuiLayout : public kgmObject
{
  KGM_OBJECT(kgmGuiLayout);

public:
  enum {
    Align_None,
    Align_Left,
    Align_Right,
    Align_Center,
    Align_Top = 1,
    Align_Bottom,
  } Align;

private:
  struct Gui {
    kgmGui* gui;
    kgmGui::Rect    rect;
  };

  struct Scale {
    f64 x;
    f64 y;
  };

  kgmGui::Rect m_rcInit;

  Scale m_scaler;

  u32 m_h_align;
  u32 m_v_align;

  kgmGui *m_gui = null;

public:
  kgmGuiLayout(kgmGui*);

  void onResize(int w, int h);
  void onShow();
  void onHide();

  void setVAlign(u32 a);
  void setHAlign(u32 a);

protected:
  void realign();
  void rescale();

private:
  Gui find(kgmGui*);
};

#endif // KGMGUILAYOUT_H
