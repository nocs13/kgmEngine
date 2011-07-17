#include "kgmGuiCombo.h"

kgmGuiCombo::kgmGuiCombo(){

}

kgmGuiCombo::kgmGuiCombo(kgmGui *par, int x, int y, int w, int h)
:kgmGui(par, x, y, w, h){
  m_selected = -1;
  m_list = new kgmGuiList(this, x, y, w, h + 200);
}

kgmGuiCombo::~kgmGuiCombo(){
  delete m_list;
}

void kgmGuiCombo::addItem(kgmString& s){
  if(!m_list)
   return;
  m_list->addItem(s);
}

void kgmGuiCombo::onPaint(kgmIGraphics* gc){
  gcDrawRect(gc, m_rect, m_colors[0], 0);
  Rect clip(m_rect.x, m_rect.y, m_rect.w, m_rect.h);
  if(m_selected >= 0){
   kgmString str;
   str = m_list->m_items[m_selected];
   gcDrawText(gc, getFont(), 10, 10, m_colors[4], m_rect, str);
  }else if(m_text.length())
    gcDrawText(gc, getFont(), 10, 10, m_colors[4], m_rect, m_text);
}

void kgmGuiCombo::onMsLeftUp(int k, uint x, uint y){
  if(!m_view || !m_rect.inside(x,y) || m_list->m_rect.inside(x, y)){
    //m_list->onMsLeftUp(k, x, y);
   m_list->hide();
   return;
  }
  if(m_rect.inside(x,y)){
   m_list->show();
  }
}

void kgmGuiCombo::onAction(kgmEvent* evt){
  if(!evt)
   return;
}


