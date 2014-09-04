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

/*void kgmGuiButton::onPaint(kgmIGC* gc){
  u32 fwidth = m_rect.w / 15;
  u32 fheight = m_rect.h / 2; 
  u32 tlen = m_text.length();
  u32 fw = (tlen + 1) * fwidth;
  u32 fh = (fheight + 2);
  Rect tClip = m_rect;

  tClip.x = m_rect.x + m_rect.w / 2 - fw / 2;
   tClip.y = m_rect.y + m_rect.h/ 2 - fh / 2;
  tClip.w = m_rect.w / 2 + fw / 2;
   tClip.h = m_rect.h / 2 + fh / 2;

  tClip.x = (tClip.x < m_rect.x)?(m_rect.x):(tClip.x);
   tClip.y = (tClip.y < m_rect.y)?(m_rect.y):(tClip.y);
  tClip.w = (tClip.w > m_rect.w)?(m_rect.w):(tClip.w);
   tClip.h = (tClip.h > m_rect.h)?(m_rect.h):(tClip.h);

  switch(m_act){
  case ActFocus:
	   gcDrawRect(gc, m_rect, m_colors[1], 0);
	  break;
  case ActClick:
	   gcDrawRect(gc, m_rect, m_colors[2], 0);
	  break;
  case ActNone:
  default:
	   gcDrawRect(gc, m_rect, m_colors[0], 0);
  }
  gcDrawText(gc, getFont(), fwidth, fheight, m_colors[TEXT], tClip, m_text);
  }*/

void kgmGuiButton::onMsLeftUp(int key, int x, int y)
{
  //if(m_state == StateFocus)
  {
    if(callback.hasObject() && callback.hasFunction())
      callback(callback.getObject());

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

