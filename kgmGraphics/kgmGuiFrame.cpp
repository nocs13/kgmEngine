#include "kgmGuiFrame.h"
#include "kgmGuiLabel.h"
#include "kgmGuiButton.h"

kgmGuiFrame::kgmGuiFrame(kgmString title, int x, int y, int w, int h)
:kgmGui(null, x, y, w, h)
{
  m_title = new kgmGuiLabel(this, 0, 0, w - 16, 15);
  m_title->setText(title);
  m_close = new kgmGuiButton(this, w - 16, 0, 15, 15);
  m_close->setText("X");
  m_client = new kgmGui(this, 0, 16, w, h - 15);
}

void kgmGuiFrame::onMsMove(int k, int x, int y)
{
  Rect rc(0, 0, m_rect.width(), 16);

  if(rc.inside(x, y))
  {
    int dx = x - m_prevx;
    int dy = y - m_prevy;

    m_rect.x += dx;
    m_rect.y += dy;

    m_prevx = x;
    m_prevy = y;
  }

  kgmGui::onMsMove(k, x, y);
}

void kgmGuiFrame::onResize(int w, int h)
{
  kgmGui::onResize(w, h);
}

void kgmGuiFrame::onKeyDown(int k)
{

  kgmGui::onKeyDown(k);
}

void kgmGuiFrame::onKeyUp(int k)
{

  kgmGui::onKeyUp(k);
}
