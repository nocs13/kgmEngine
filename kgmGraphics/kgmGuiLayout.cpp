#include "kgmGuiLayout.h"

kgmGuiLayout::kgmGuiLayout()
{

}

kgmGuiLayout::kgmGuiLayout(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_rcInit = Rect(x, y, w, h);
}

void kgmGuiLayout::onResize(int w, int h)
{
  if (!visible())
    return;
}

void kgmGuiLayout::onShow()
{

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

void kgmGuiLayout::update()
{
  for (auto gi = m_childs.begin(); !gi.end(); gi.next())
  {
    bool already = false;

    for (auto ci = m_guis.begin(); !ci.end(); ci.next())
    {
      if ((*gi) == (*ci).gui)
      {
        already = true;

        break;
      }
    }

  }

  for (auto ci = m_guis.begin(); !ci.end(); ci.next())
  {
    kgmGui* g = (*ci).gui;

    bool already = false;

    for (auto gi = m_childs.begin(); !gi.end(); gi.next())
    {
      if ((*gi) == g)
      {
        already = true;

        break;
      }
    }
  }

}

void kgmGuiLayout::realign()
{

}
