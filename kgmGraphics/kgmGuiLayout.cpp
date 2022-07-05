#include "kgmGuiLayout.h"
#include "../kgmSystem/kgmSystem.h"

kgmGuiLayout::kgmGuiLayout()
{
  m_h_align = Align_Left;
  m_v_align = Align_Top;

  scalable(false);
}

kgmGuiLayout::kgmGuiLayout(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_rcInit = Rect(x, y, w, h);

  m_scaler.x = 1.0;
  m_scaler.y = 1.0;

  m_h_align = Align_Left;
  m_v_align = Align_Top;

  if (par) {
    Rect prc;

    par->getRect(prc);

    m_scaler.x = (f64) w / (f64) prc.w;
    m_scaler.y = (f64) h / (f64) prc.h;
  }

  scalable(false);
}

void kgmGuiLayout::onResize(int w, int h)
{
  if (!visible())
    return;

  m_rect = m_rcInit;

  realign();
}

void kgmGuiLayout::onShow()
{
  realign();
}

void kgmGuiLayout::onHide()
{

}

void kgmGuiLayout::onAddChild(kgmGui *c)
{
  Rect rc;

  c->getRect(rc);

  Gui gui = { c, rc };

  m_guis.add(gui);
}

void kgmGuiLayout::onDelChild(kgmGui *c)
{
  for (auto ci = m_guis.begin(); !ci.end(); ci.next())
  {
    if (c == (*ci).gui)
    {
      m_guis.erase(ci);

      break;
    }
  }
}

void kgmGuiLayout::realign()
{
  Rect rc_p, rc;

  getRect(rc);

  if (m_parent)
  {
    m_parent->getRect(rc_p);
  }
  else
  {
    u32 w, h;

    kgmSystem::getScreenResolution(w, h);

    rc_p = Rect(0, 0, w, h);
  }

  switch(m_h_align)
  {
  case Align_Left:
    rc.x = 1;
    break;
  case Align_Right:
    rc.x = rc_p.width() - rc.width() - 1;
    break;
  case Align_Center:
    rc.x = rc_p.width() / 2 - rc.width() / 2;
    break;
  }

  switch(m_v_align)
  {
  case Align_Top:
    rc.y = 1;
    break;
  case Align_Bottom:
    rc.y = rc_p.height() - rc.height() - 1;
    break;
  case Align_Center:
    rc.y = rc_p.height() / 2 - rc.height() / 2;
    break;
  }

  setRect(rc);
}

void kgmGuiLayout::rescale()
{
  for (auto ci = m_childs.begin(); !ci.end(); ci.next())
  {
    kgmGui* g = (*ci);

    Gui gui = find(g);

    if (!gui.gui)
      continue;

    Rect rc;

    g->getRect(rc);

    rc.x = (typeof rc.x) ( m_scaler.x * (f64) gui.rect.x);
    rc.y = (typeof rc.y) ( m_scaler.y * (f64) gui.rect.y);

    g->setRect(rc);
  }
}

void kgmGuiLayout::setVAlign(u32 a)
{
  if (a <= Align_Center)
    m_v_align = a;
}

void kgmGuiLayout::setHAlign(u32 a)
{
  if (a <= Align_Center)
    m_h_align = a;
}

kgmGuiLayout::Gui kgmGuiLayout::find(kgmGui *g)
{
  for (auto ci = m_guis.begin(); !ci.end(); ci.next())
  {
    if (g == (*ci).gui)
    {
      return (*ci);
    }
  }

  Gui gui = {0};

  return gui;
}
