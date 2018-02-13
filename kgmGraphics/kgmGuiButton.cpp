#include "kgmGuiButton.h"
#include "../kgmBase/kgmLog.h"

kgmGuiButton::kgmGuiButton()
{
  m_state = StateNone;
  m_close = false;
}

kgmGuiButton::kgmGuiButton(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
  :kgmGui(par, x, y, w, h)
{
  m_state = StateNone;
  m_close = false;
}

kgmGuiButton::~kgmGuiButton()
{
}

void kgmGuiButton::onMsLeftUp(int key, int x, int y)
{
  sigClick(0);

  m_state = StateFocus;
}

void kgmGuiButton::onMsLeftDown(int key, int x, int y)
{
  m_state = StateClick;
}

void kgmGuiButton::onMsInside()
{
  if(m_state == StateNone)
  {
    m_state = StateFocus;
  }
}

void kgmGuiButton::onMsOutside()
{
  if(m_state != StateNone)
  {
    m_state = StateNone;
  }
}

