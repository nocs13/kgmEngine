#pragma once
#include "kgmGui.h"
#include "kgmGuiScroll.h"

class kgmGuiList: public kgmGui{
  KGM_OBJECT(kgmGuiList)

 typedef kgmList<kgmString> Items;

 struct Item
 {
   u32       icon;
   kgmString text;
 };

public:
 Items     m_items;
 int       m_itemSel;
 u32       m_itemStart;
 u32       m_itemHeight;

 kgmGuiScroll *m_scroll;
 u32           m_position;
 u32           m_range;

 kgmList<Icon> icons;

public:
 kgmGuiList();
 kgmGuiList(kgmGui *par, int x, int y, int w, int h);
 ~kgmGuiList();
 void addItem(kgmString s, u32 icon = -1);
 void remItem(int s);

 kgmString getItem(int i);
 Rect      getItemRect(int i);
 Icon      getItemIcon(int i);
 void      setSel(int sel);
 int       getSel();
 int       getFirstVisibleItem();
 int       getVisibleItemsCount();
 char*     getSelectedItem();
 void      clear();
 void      sort();

 virtual void onSelect(u32);

protected:
 void onMsLeftDown(int k, int x, int y);
 void onMsLeftUp(int k, int x, int y);
 void onMsMove(int k, int x, int y);
 void onMsWheel(int k, int x, int y, int z);
 void onKeyDown(int k);
 void onAction(kgmGui*, u32);
};
