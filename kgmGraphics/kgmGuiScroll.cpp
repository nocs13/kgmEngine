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
}

kgmGuiScroll::~kgmGuiScroll(){
}

void kgmGuiScroll::setRange(u32 r){
  m_range = r;
  m_position = 0;
}

/*void kgmGuiScroll::onPaint(kgmIGC* gc){
  if(!m_view)
   return;
  Rect rect = Rect(m_rect.x, m_rect.y + m_position * m_dimension.y,
            m_rect.w, (m_position + 1) * m_dimension.y);
  gcDrawRect(gc, m_rect, m_colors[0], 0);
  gcDrawRect(gc, rect, m_colors[3], 0);
  }*/

void kgmGuiScroll::onMsMove(int key, u32 x, u32 y){
  u32 pos;

  if(!m_view  ||
     !m_focus ||
     !key == KEY_MSBLEFT ||
     !m_rect.inside(x, y))
    return;

  if(m_orientation == ORIENT_VERTICAL)
    pos = (y - m_rect.y) / m_range;
  else
    pos = (x - m_rect.x) / m_range;

  if(pos != m_position)
  {
    onChange(pos);

    m_position = pos;

    if(m_parent)
      m_parent->onAction(this, 0);
  }
}

void kgmGuiScroll::onChange(u32 pos)
{

}
