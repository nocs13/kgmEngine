#pragma once
#include "kgmGui.h"

class kgmGuiButton: public kgmGui
{
  KGM_OBJECT(kgmGuiButton);
public:

  enum State
  {
    StateNone,
    StateFocus,
    StateClick
  };

private:
  State  m_state;

  bool m_close;

public:
  Signal<int> sigClick;

public:
  kgmGuiButton();
  kgmGuiButton(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  virtual ~kgmGuiButton();

  State getState()
  {
    return m_state;
  }

  void sClose(bool s) {
    m_close = s;
  }

  bool sClose() {
    return m_close;
  }

private:
  void onMsLeftUp(int key, int x, int y);
  void onMsLeftDown(int key, int x, int y);
  void onMsInside();
  void onMsOutside();
};
