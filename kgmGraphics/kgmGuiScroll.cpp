#include "kgmGuiScroll.h"

kgmGuiScroll::kgmGuiScroll()
  :kgmGui()
{
  m_ppp = 1;
  m_drag = false;
  m_range = 1;
  m_position = 1;
  m_orientation = ORIENT_VERTICAL;

  m_msp = 0;
}

kgmGuiScroll::kgmGuiScroll(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_ppp = 1;
  m_drag = false;
  m_range = 1;
  m_position = 1;
  m_orientation = ORIENT_VERTICAL;

  m_msp = 0;
}

kgmGuiScroll::~kgmGuiScroll()
{
}

kgmGui::Rect kgmGuiScroll::getScrollerRect()
{
  int t = (m_orientation == ORIENT_VERTICAL) ? (m_rect.height()) : (m_rect.width());
  int s = t / m_range;
  int k = t / m_range;

  s = (s > 10) ? (s) : (10);
  k = (k >  1) ? (k) : ( 1);

  Rect rect;

  if(m_orientation == ORIENT_VERTICAL)
    if((m_position * m_ppp + s) < m_rect.height())
      rect = Rect(m_rect.x, m_rect.y + m_position * m_ppp, m_rect.w, s);
    else
      rect = Rect(m_rect.x, m_rect.y + m_rect.height() - s, m_rect.w, s);
  else
    if((m_position * m_ppp + s) < m_rect.width())
      rect = Rect(m_rect.x + m_position * m_ppp, m_rect.y, s, m_rect.h);
    else
      rect = Rect(m_rect.x + m_rect.width() - s, m_rect.y, s, m_rect.h);

  return rect;
}

void kgmGuiScroll::setRange(u32 r)
{
  if(r < 1)
    return;

  m_range    = r;
  m_position = 1;

  int t = (m_orientation == ORIENT_VERTICAL) ? (m_rect.height()) :(m_rect.width());

  m_ppp = (float)t / (float)r;
}

void kgmGuiScroll::setPosition(u32 p)
{
  if(p >= 1 && p <= m_range)
    m_position = p;
}

void kgmGuiScroll::setOrientation(ORIENT o)
{
  m_orientation = o;
}

void kgmGuiScroll::onMsMove(int key, int x, int y)
{
  u32 pos = 0;

  if(!m_drag)
    return;

  float d;
  float h;

  if(m_orientation == ORIENT_VERTICAL)
  {
    h = (float)m_rect.height() / (float)m_range;
    d = (float)(y) / (float)h;
  }
  else
  {
    h = (float)m_rect.width() / (float)m_range;
    d = (float)(x) / (float)h;
  }

  pos = d;

  if(m_orientation == ORIENT_VERTICAL)
    pos = (y) / m_ppp;
  else
    pos = (x) / m_ppp;

  if(pos <= m_range && pos != m_position)
  {
    m_position = pos;

    sigChange(pos);
  }
}

void kgmGuiScroll::onMsLeftDown(int key, int x, int y)
{
  if(getScrollerRect().inside(m_rect.x + x, m_rect.y + y))
  {
    m_drag = true;
    m_dx = x;
    m_dy = y;
  }
}

void kgmGuiScroll::onMsLeftUp(int key, int x, int y)
{
  m_drag = false;
}

void kgmGuiScroll::onMsOutside()
{
  m_drag = false;
}
