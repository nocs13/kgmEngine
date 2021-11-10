#include "kgmGuiLayout.h"
#include "../kgmSystem/kgmSystem.h"

kgmGuiLayout::kgmGuiLayout(kgmGui* gui)
{
  m_h_align = Align_Left;
  m_v_align = Align_Top;

  m_gui = gui;
}

void kgmGuiLayout::onResize(int w, int h)
{
  realign();
}

void kgmGuiLayout::onShow()
{
  realign();
}

void kgmGuiLayout::onHide()
{

}

void kgmGuiLayout::realign()
{
  kgmGui::Rect rc_p, rc;

  m_gui->getRect(rc);

  if (m_gui->getParent())
  {
    m_gui->getParent()->getRect(rc_p);
  }
  else
  {
    u32 w, h;

    kgmSystem::getScreenResolution(w, h);

    rc_p = kgmGui::Rect(0, 0, w, h);
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

  m_gui->setRect(rc);
}

void kgmGuiLayout::rescale()
{
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
