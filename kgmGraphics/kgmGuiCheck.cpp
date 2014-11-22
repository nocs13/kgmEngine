#include "kgmGuiCheck.h"
#include "../kgmBase/kgmLog.h"

KGMOBJECT_IMPLEMENT(kgmGuiCheck, kgmGuiLabel)

kgmGuiCheck::kgmGuiCheck()
:callback(null, null)
{
  m_state = StateNone;
  m_check = false;
}

kgmGuiCheck::kgmGuiCheck(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
:kgmGuiLabel(par, x, y, w, h), callback(null, null)
{
  m_state = StateNone;
  m_check = false;

  m_group_owner = null;
}

kgmGuiCheck::kgmGuiCheck(kgmGui *par, u32 x, u32 y, u32 w, u32 h, ClickEventCallback call)
:kgmGuiLabel(par, x, y, w, h), callback(null, null)
{
  m_state = StateNone;
  m_check = false;

  callback = call;
  m_group_owner = null;
}

kgmGuiCheck::~kgmGuiCheck()
{
  m_group.clear();
}

void kgmGuiCheck::onMsLeftUp(int key, int x, int y){
  //if(m_state == StateFocus)
  {
    //m_check = !m_check;
    setCheck(!m_check);

    if(callback.hasObject() && callback.hasFunction())
      callback(m_check);
  }
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

