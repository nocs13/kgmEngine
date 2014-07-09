#include "kgmGuiScroll.h"

KGMOBJECT_IMPLEMENT(kgmGuiScroll, kgmGui)

kgmGuiScroll::kgmGuiScroll()
  :kgmGui(), callback(null, null)
{
  m_ppp = 1;
  m_drag = false;
  m_range = 1;
  m_position  = 0;
  m_orientation = ORIENT_VERTICAL;

  m_msp = 0;
}

kgmGuiScroll::kgmGuiScroll(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h), callback(null, null)
{
  m_ppp = 1;
  m_drag = false;
  m_range = 1;
  m_position  = 0;
  m_orientation = ORIENT_VERTICAL;

  m_msp = 0;
}

kgmGuiScroll::~kgmGuiScroll()
{
}

kgmGui::Rect kgmGuiScroll::getScrollerRect()
{
  int t = (m_orientation == ORIENT_VERTICAL) ? (m_rect.height()) :(m_rect.width());
  int s = t / m_range;
  int k = t / m_range;

  s = (s > 10) ? (s) : (10);
  k = (k >  1) ? (k) : ( 1);

  Rect rect;

  if(m_orientation == ORIENT_VERTICAL)
    rect = Rect(m_rect.x, m_rect.y + m_position * m_ppp, m_rect.w, s);
  else
    rect = Rect(m_rect.x + m_position * m_ppp, m_rect.y, s, m_rect.h);

  return rect;
}

void kgmGuiScroll::setRange(u32 r)
{
  if(r > 0)
    m_range = r;

  m_position = 0;

  int t = (m_orientation == ORIENT_VERTICAL) ? (m_rect.height()) :(m_rect.width());

  m_ppp = (float)t / (float)r;
}

void kgmGuiScroll::setPosition(u32 p)
{
  m_position = p;
}

void kgmGuiScroll::setOrientation(ORIENT o)
{
  m_orientation = o;
}

/*void kgmGuiScroll::onPaint(kgmIGC* gc){
  if(!m_view)
   return;
  Rect rect = Rect(m_rect.x, m_rect.y + m_position * m_dimension.y,
            m_rect.w, (m_position + 1) * m_dimension.y);
  gcDrawRect(gc, m_rect, m_colors[0], 0);
  gcDrawRect(gc, rect, m_colors[3], 0);
  }*/

void kgmGuiScroll::onMsMove(int key, int x, int y)
{
  u32 pos;

  //if(!m_view  ||
     //!m_drag ||
     //!m_rect.inside(x, y))
    //return;

  float d;
  float h;

  if(m_orientation == ORIENT_VERTICAL)
  {
    h = (float)m_rect.height() / (float)m_range;
    d = (float)(y - m_rect.y) / (float)h;
  }
  else
  {
    h = (float)m_rect.width() / (float)m_range;
    d = (float)(x - m_rect.x) / (float)h;
  }

  pos = d;

  if(m_orientation == ORIENT_VERTICAL)
    pos = (y - m_rect.y) / m_ppp;
  else
    pos = (x - m_rect.x) / m_ppp;

  if(pos != m_position)
  {
    m_position = pos;
    //m_msp = y;
    if(callback.hasObject() && callback.hasFunction())
      callback(callback.getObject(), pos);
  }
}

void kgmGuiScroll::onMsLeftDown(int key, int x, int y)
{
  m_drag = true;
  m_dx = x;
  m_dy = y;
}

void kgmGuiScroll::onMsLeftUp(int key, int x, int y)
{
  m_drag = false;
}
