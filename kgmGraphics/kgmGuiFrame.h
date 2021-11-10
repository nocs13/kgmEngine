#ifndef KGMGUIFRAME_H
#define KGMGUIFRAME_H

#include "kgmGuiWidget.h"

class kgmGuiLabel;
class kgmGuiButton;

class kgmGuiFrame : public kgmGuiWidget
{
  KGM_OBJECT(kgmGuiFrame);
private:

  kgmGui*        m_client;
  kgmGuiLabel*   m_title;
  kgmGuiButton*  m_close;

  int            m_prevx, m_prevy;

  Slot<kgmGuiFrame, int> slotClose;

public:
  kgmGuiFrame(kgmString title = "Frame", int x = 50, int y = 50, int w = 200, int h = 300);

  void onMsMove(int k, int x, int y);
  void onMsLeftDown(int k, int x, int y);
  void onResize(int w, int h);
  void onClose();

  __stdcall void onFrameClose();

  kgmGui* getClient()
  {
    return m_client;
  }
};

#endif // KGMGUIFRAME_H
