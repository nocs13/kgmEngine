#ifndef KGMGUIFRAME_H
#define KGMGUIFRAME_H

#include "kgmGui.h"

class kgmGuiLabel;
class kgmGuiButton;

class kgmGuiFrame : public kgmGui
{
  KGM_OBJECT(kgmGuiFrame);
private:

  kgmGui*        m_client;
  kgmGuiLabel*   m_title;
  kgmGuiButton*  m_close;

  int            m_prevx, m_prevy;

public:
  kgmGuiFrame(kgmString title = "Frame", int x = 50, int y = 50, int w = 200, int h = 300);

  void onMsMove(int k, int x, int y);
  void onResize(int w, int h);
  void onKeyDown(int k);
  void onKeyUp(int k);
};

#endif // KGMGUIFRAME_H
