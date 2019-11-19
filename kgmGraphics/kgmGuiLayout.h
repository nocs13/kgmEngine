#ifndef KGMGUILAYOUT_H
#define KGMGUILAYOUT_H

#include "kgmGui.h"

class kgmGuiLayout : public kgmGui
{
  KGM_OBJECT(kgmGuiLayout);

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

public:
  kgmGuiLayout();
  kgmGuiLayout(kgmGui *par, int x, int y, int w, int h);

  void onResize(int w, int h);
  void onShow();
  void onHide();
  void onAddChild(kgmGui *c);
  void onDelChild(kgmGui *c);

protected:
  void realign();

private:
  Gui find(kgmGui*);
};

#endif // KGMGUILAYOUT_H
