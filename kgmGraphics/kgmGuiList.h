#pragma once
#include "kgmGui.h"
#include "kgmGuiScroll.h"

class kgmGuiList: public kgmGui{

 typedef kgmList<kgmString> Items;

 struct Item{
  kgmString text;
 };
public:
 Items     m_items;
 int       m_itemSel;
 uint      m_itemStart;
 uint      m_itemHeight;

 kgmGuiScroll *m_scroll;
 uint          m_position;
 uint          m_range;
 
public:
 kgmGuiList();
 kgmGuiList(kgmGui *par, int x, int y, int w, int h);
 ~kgmGuiList();
 void addItem(kgmString s);
 kgmString getItem(int i);
 void setSel(int sel);
 int getSel();
 char* getSelectedItem();
 void clear();

 virtual void onSelect(uint);
protected:
 void onPaint(kgmIGraphics* gc);
 void onMsLeftUp(int k, int x, int y);
 void onMsMove(int k, int x, int y);
 void onMsWheel(int k, int x, int y, int z);
 void onKeyDown(int k);
};
