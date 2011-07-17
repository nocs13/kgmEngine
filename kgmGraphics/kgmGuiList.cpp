#include "kgmGuiList.h"

kgmGuiList::kgmGuiList()
 :kgmGui(){
 m_scroll = new kgmGuiScroll(this, 0, 0, 0, 0);
 m_scroll->show();
 m_scroll->setRange(1);
 m_itemSel = -1;
 m_itemHeight = 20;
 m_position = 0;
}

kgmGuiList::kgmGuiList(kgmGui *par, int x, int y, int w, int h)
:kgmGui(par, x, y, w, h){
 m_scroll = new kgmGuiScroll(this, 0, 0, 0, 0);
 m_scroll->show();
 m_scroll->setRange(1);
 m_itemSel = -1;
 m_itemHeight = 20;
 m_position = 0;
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

void kgmGuiList::onSelect(uint sel){
 m_parent->onAction(this, sel);
}

void kgmGuiList::onPaint(kgmIGraphics* gc){
  uint m_fontHeight = m_itemHeight - 2;
  uint item_cnt = m_items.size();
  uint item_view = m_itemHeight;
  Rect frect;
 
  //Draw Main Rect
  gcDrawRect(gc, m_rect, m_colors[kgmGui::BACKGROUND], 0);
  //Draw Focused Rect
  if((m_itemSel >= 0) && (m_itemSel < m_items.size()))
   gcDrawRect(gc, Rect(m_rect.x, m_rect.y + m_itemSel * m_itemHeight,
                       m_rect.w, m_itemHeight), m_colors[kgmGui::FOREGROUND], 0);
  //Draw Items Rects
  for(int i = m_position; i < (m_position + item_view); i++){
   if(i >= item_cnt) continue;
   kgmString item; item = m_items[i];
   uint a = (i - m_position);
   Rect clip(m_rect.x, m_rect.y + m_itemHeight * a,
	     m_rect.w, m_itemHeight);

   clip.h ++;
   //gcDrawText(gc, getFont(), 10, m_fontHeight, m_colors[kgmGui::TEXT], clip, item);
   gcDrawText(gc, getFont(), 10, clip.height(), m_colors[kgmGui::TEXT], clip, item);
  }
} 

void kgmGuiList::onMsLeftUp(int k, int x, int y){
  m_itemSel = m_position + (y - m_rect.y) / m_itemHeight;
  if(m_parent && (m_itemSel < m_items.size()))
   onSelect(m_itemSel);
}

void kgmGuiList::onMsMove(int k, int x, int y){
  Rect rc = m_rect;
  rc.h --;
  if(!rc.inside(x, y))
   return;
  int dy = y - m_rect.y;
  if(dy/m_itemHeight < m_items.size())
   m_itemSel = dy/m_itemHeight;
}

void kgmGuiList::onMsWheel(int k, int x, int y, int z){
  Rect rc = m_rect;
  rc.h --;
  if(!rc.inside(x, y))
   return;
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
 
 
