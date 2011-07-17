#include "kgmGuiButton.h"

kgmGuiButton::kgmGuiButton(){
  m_type = TypeNormal;
  m_act = ActNone;
}

kgmGuiButton::kgmGuiButton(kgmGui *par, uint x, uint y, uint w, uint h)
:kgmGui(par, x, y, w, h){
  m_type = TypeNormal;
  m_act = ActNone;
}

kgmGuiButton::~kgmGuiButton(){
}

void kgmGuiButton::onClick(){
 if(m_parent)
  m_parent->onAction(this, 0);
}

void kgmGuiButton::onActivated(){
}

void kgmGuiButton::onDeactivated(){
}

void kgmGuiButton::onPaint(kgmIGraphics* gc){
  uint fwidth = m_rect.w / 15;
  uint fheight = m_rect.h / 2; 
  uint tlen = m_text.length();
  uint fw = (tlen + 1) * fwidth;
  uint fh = (fheight + 2);
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
}

void kgmGuiButton::onMsLeftUp(int key, int x, int y){
 if(m_act == ActFocus){
  onClick();
 }
}

void kgmGuiButton::onMsLeftDown(int key, int x, int y){
 if(m_act == ActFocus){
  m_act == ActClick;
 }
}

void kgmGuiButton::onMsMove(int key, int x, int y){
  if(!m_view)
   return;
  if(m_rect.inside(x, y) && (m_act == ActNone)){
   m_act = ActFocus;
   onActivated();
  }else if(!m_rect.inside(x, y) && (m_act != ActNone)){
    m_act = ActNone;
    onDeactivated();
  }
}

