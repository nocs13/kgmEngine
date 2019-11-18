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

  Rect m_rcInit;

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
  void update();
  void realign();
};

#endif // KGMGUILAYOUT_H
