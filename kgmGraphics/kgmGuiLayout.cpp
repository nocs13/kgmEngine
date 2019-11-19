#include "kgmGuiLayout.h"

kgmGuiLayout::kgmGuiLayout()
{

}

kgmGuiLayout::kgmGuiLayout(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_rcInit = Rect(x, y, w, h);

  m_scaler.x = 1.0;
  m_scaler.y = 1.0;

  if (par) {
    Rect prc;

    par->getRect(prc);

    m_scaler.x = (f64) w / (f64) prc.w;
    m_scaler.y = (f64) h / (f64) prc.h;
  }
}

void kgmGuiLayout::onResize(int w, int h)
{
  if (!visible())
    return;

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
