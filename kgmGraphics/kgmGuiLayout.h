#ifndef KGMGUILAYOUT_H
#define KGMGUILAYOUT_H

#include "kgmGui.h"

class kgmGuiLayout : public kgmGui
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
    Rect    rect;
  };

  struct Scale {
    f64 x;
    f64 y;
  };

  Rect m_rcInit;

  Scale m_scaler;

  kgmList<Gui> m_guis;

  u32 m_h_align;
  u32 m_v_align;

public:
  kgmGuiLayout();
  kgmGuiLayout(kgmGui *par, int x, int y, int w, int h);

  void onResize(int w, int h);
  void onShow();
  void onHide();
  void onAddChild(kgmGui *c);
  void onDelChild(kgmGui *c);

  void setVAlign(u32 a);
  void setHAlign(u32 a);

protected:
  void realign();
  void rescale();

private:
  Gui find(kgmGui*);
};

#endif // KGMGUILAYOUT_H
