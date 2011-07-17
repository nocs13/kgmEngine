#pragma once
#include "kgmGui.h"
#include "kgmGuiList.h"

class kgmGuiCombo: public kgmGui{

 kgmGuiList *m_list;
 int         m_selected;
public:
 kgmGuiCombo();
 kgmGuiCombo(kgmGui *par, int x, int y, int w, int h);
 ~kgmGuiCombo();

 void addItem(kgmString& s);
 void onPaint(kgmIGraphics* gc);
 void onMsLeftUp(int k, uint x, uint y);
 void onAction(kgmEvent* evt);
};
