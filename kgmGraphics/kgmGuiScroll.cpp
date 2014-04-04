#include "kgmGuiScroll.h"

KGMOBJECT_IMPLEMENT(kgmGuiScroll, kgmGui)

kgmGuiScroll::kgmGuiScroll()
{
  m_drag = false;
  m_range = 1;
  m_position  = 0;
  m_orientation = ORIENT_VERTICAL;
}

kgmGuiScroll::kgmGuiScroll(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_drag = false;
  m_range = 1;
  m_position  = 0;
  m_orientation = ORIENT_VERTICAL;
}

kgmGuiScroll::~kgmGuiScroll()
{
}

kgmGui::Rect kgmGuiScroll::getScrollerRect()
{
  int s = m_rect.height() / m_range;
  int k = m_rect.height() / m_range;

  s = (s > 10) ? (s) : (10);
  k = (k >  1) ? (k) : ( 1);

  Rect rect = Rect(m_rect.x, m_rect.y + m_position * k,
                   m_rect.w, s);

  return rect;
}

void kgmGuiScroll::setRange(u32 r)
{
  if(r > 0)
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

void kgmGuiScroll::onMsMove(int key, u32 x, u32 y)
{
  u32 pos;

  if(!m_view  ||
     !m_drag ||
     !m_rect.inside(x, y))
    return;

  float d;
  float h = (float)m_rect.height() / (float)m_range;

  if(m_orientation == ORIENT_VERTICAL)
    d = (float)(y - m_rect.y) / (float)h;
  else
    d = (float)(x - m_rect.x) / (float)h;

  pos = d;

  if(pos != m_position)
  {
    m_position = pos;

    onChange(pos);
  }
}

void kgmGuiScroll::onMsLeftDown(int key, u32 x, u32 y)
{
  m_drag = true;
  m_dx = x;
  m_dy = y;
}

void kgmGuiScroll::onMsLeftUp(int key, u32 x, u32 y)
{
  m_drag = false;
}

void kgmGuiScroll::onChange(u32 pos)
{
  if(m_parent)
    m_parent->onAction(this, pos);
}
