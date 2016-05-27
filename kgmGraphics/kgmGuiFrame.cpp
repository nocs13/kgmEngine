#include "kgmGuiFrame.h"
#include "kgmGuiLabel.h"
#include "kgmGuiButton.h"

kgmGuiFrame::kgmGuiFrame(kgmString title, int x, int y, int w, int h)
:kgmGui(null, x, y, w, h)
{
  m_title = new kgmGuiLabel(this, 0, 0, w - 20, 20);
  m_title->setText(title);
  m_close = new kgmGuiButton(this, w - 20, 0, 20, 20);
  m_close->setText("X");
  m_close->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kgmGuiFrame::onFrameClose));

  m_client = new kgmGui(this, 0, 20, w, h - 20);
}

void kgmGuiFrame::onMsMove(int k, int x, int y)
{
  Rect rc(m_rect.x, m_rect.y, m_rect.w, 20);

  if((k == KEY_MSBLEFT) && rc.inside(x, y))
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

void kgmGuiFrame::onMsLeftDown(int k, int x, int y)
{
  Rect rc(m_rect.x, m_rect.y, m_rect.w, 20);

  if(rc.inside(x, y))
  {
    m_prevx = x;
    m_prevy = y;
  }

  kgmGui::onMsLeftDown(k, x, y);
}

void kgmGuiFrame::onResize(int w, int h)
{
  m_close->m_rect.x = w - 20;

  kgmGui::onResize(w, h);
}

void kgmGuiFrame::onFrameClose()
{
  close();
}

void kgmGuiFrame::onClose()
{
}
