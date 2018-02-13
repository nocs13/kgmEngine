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

  struct Theme
  {
    Color bground;
    Color fground;
  };

  State  m_state;

  Theme m_theme;

  bool m_notheme;

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

private:
  void onMsLeftUp(int key, int x, int y);
  void onMsLeftDown(int key, int x, int y);
  void onMsInside();
  void onMsOutside();
};
