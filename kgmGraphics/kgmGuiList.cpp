#include "kgmGuiList.h"

KGMOBJECT_IMPLEMENT(kgmGuiList, kgmGui)

kgmGuiList::kgmGuiList()
 :kgmGui(){
 m_scroll = new kgmGuiScroll(this, 0, 0, 0, 0);
 m_scroll->show();
 m_scroll->setRange(1);
 m_itemSel = -1;
 m_itemHeight = 20;
 m_position = 0;
 m_range    = 0;
}

kgmGuiList::kgmGuiList(kgmGui *par, int x, int y, int w, int h)
:kgmGui(par, x, y, w, h){
 m_scroll = new kgmGuiScroll(this, 0, 0, 0, 0);
 m_scroll->show();
 m_scroll->setRange(1);
 m_itemSel = -1;
 m_itemHeight = 20;
 m_position = 0;
 m_range    = 0;
}


kgmGuiList::~kgmGuiList(){
  clear();
  delete m_scroll;
}

void kgmGuiList::addItem(kgmString s){
  if(!s.length())
   return;

  int vCnt = m_rect.h / m_itemHeight;
  int sRange = m_items.size() - vCnt;
  m_items.push_back(s);
  if(m_range > 1)
   m_scroll->setRange(m_range + 2);
}

void kgmGuiList::remItem(int s){
  if(s == -1)
   return;

  m_items.erase(s);
}

kgmString kgmGuiList::getItem(int i){
  if(i < 0 || i > m_itemSel)
	  return kgmString();
  return m_items[i];
}

void kgmGuiList::setSel(int sel){
  m_itemSel = sel;
}

int kgmGuiList::getSel(){
  return m_itemSel;
}

char* kgmGuiList::getSelectedItem(){
  if(m_itemSel < 0)
	  return 0;
  return getItem(m_itemSel);
}

void kgmGuiList::clear(){
 for(int i = 0; i < m_items.length(); i++)
   m_items[i].clear();
 m_items.clear();
}

void kgmGuiList::onSelect(u32 sel){
 m_parent->onAction(this, sel);
}

void kgmGuiList::onMsLeftUp(int k, int x, int y){
  Rect rect = Rect(0, 0, m_rect.w, m_rect.h);
  m_itemSel = m_position + (y - rect.y) / m_itemHeight;
  if(m_parent && (m_itemSel < m_items.size()))
   onSelect(m_itemSel);
}

void kgmGuiList::onMsMove(int k, int x, int y){
  Rect rect = Rect(0, 0, m_rect.w, m_rect.h);

  if(y / m_itemHeight < m_items.size())
   m_itemSel = y / m_itemHeight;
}

void kgmGuiList::onMsWheel(int k, int x, int y, int z){
 if(z > 0)
     onKeyDown(KEY_UP);
 else
     onKeyDown(KEY_DOWN);
}

void kgmGuiList::onKeyDown(int k){
  switch(k){
  case KEY_UP:
	  if(m_itemSel > 0)
		  m_itemSel--;
	  break;
  case KEY_DOWN:
	  if(m_itemSel < (m_items.size() - 1))
		  m_itemSel++;
	  break;
  case KEY_ENTER:
	  if(m_itemSel >= 0 && m_itemSel < m_items.size())
	    onSelect(m_itemSel);
	  break;
  }
}
 
 
