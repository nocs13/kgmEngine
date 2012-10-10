#include "kgmGuiScroll.h"

KGMOBJECT_IMPLEMENT(kgmGuiScroll, kgmGui)

kgmGuiScroll::kgmGuiScroll(){
  m_range = 1;
  m_position  = 0;
}

kgmGuiScroll::kgmGuiScroll(kgmGui *par, int x, int y, int w, int h)
:kgmGui(par, x, y, w, h){
  m_range = 1;
  m_position  = 0;
  m_dimension = uint2(w, h / m_range);
}

kgmGuiScroll::~kgmGuiScroll(){
}

void kgmGuiScroll::setRange(uint r){
  m_range = r;
  m_dimension = uint2(m_rect.w, m_rect.h / m_range);
}

/*void kgmGuiScroll::onPaint(kgmIGraphics* gc){
  if(!m_view)
   return;
  Rect rect = Rect(m_rect.x, m_rect.y + m_position * m_dimension.y,
	           m_rect.w, (m_position + 1) * m_dimension.y);
  gcDrawRect(gc, m_rect, m_colors[0], 0);
  gcDrawRect(gc, rect, m_colors[3], 0);
  }*/

void kgmGuiScroll::onMsMove(int key, uint x, uint y){
  if(!(m_view) ||
	 !m_focus ||
	 !m_rect.inside(x, y))
   return;
  m_position = (y - m_rect.y) / m_dimension.y;
  if(m_parent)
   m_parent->onAction(this, 0);
}

