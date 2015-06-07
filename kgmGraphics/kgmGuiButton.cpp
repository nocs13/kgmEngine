#include "kgmGuiButton.h"
#include "../kgmBase/kgmLog.h"

KGMOBJECT_IMPLEMENT(kgmGuiButton, kgmGui)

kgmGuiButton::kgmGuiButton()
:callback(null, null)
{
  m_state = StateNone;
}

kgmGuiButton::kgmGuiButton(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
:kgmGui(par, x, y, w, h), callback(null, null)
{
  m_state = StateNone;
}

kgmGuiButton::kgmGuiButton(kgmGui *par, u32 x, u32 y, u32 w, u32 h, ClickEventCallback call)
:kgmGui(par, x, y, w, h), callback(null, null)
{
  m_state = StateNone;

  callback = call;
}

kgmGuiButton::~kgmGuiButton()
{
}

void kgmGuiButton::onMsLeftUp(int key, int x, int y)
{
  //if(m_state == StateFocus)
  {
    if(callback.hasObject() && callback.hasFunction())
      callback();

    event(0);

    m_state = StateFocus;
  }
}

void kgmGuiButton::onMsLeftDown(int key, int x, int y){
  //if(m_state == StateFocus)
  {
    m_state = StateClick;
  }
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

