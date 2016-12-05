#include "kgmGuiCheck.h"
#include "../kgmBase/kgmLog.h"

kgmGuiCheck::kgmGuiCheck()
{
  m_state = StateNone;
  m_check = false;
}

kgmGuiCheck::kgmGuiCheck(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
  :kgmGuiLabel(par, x, y, w, h)
{
  m_state = StateNone;
  m_check = false;

  m_group_owner = null;
}

kgmGuiCheck::~kgmGuiCheck()
{
  m_group.clear();
}

void kgmGuiCheck::onMsLeftUp(int key, int x, int y)
{
  setCheck(!m_check);

  sigClick(m_check);
}

void kgmGuiCheck::onMsInside()
{
  if(m_state == StateNone)
  {
    m_state = StateFocus;
  }
}

void kgmGuiCheck::onMsOutside()
{
  if(m_state != StateNone)
  {
    m_state = StateNone;
  }
}

