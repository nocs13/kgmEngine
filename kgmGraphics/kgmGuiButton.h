#pragma once
#include "kgmGui.h"


class kgmGuiButton: public kgmGui
{
  KGM_OBJECT(kgmGuiButton)
    enum Type{
    TypeNormal,
    TypeCheck,
    TypeRadio,
  };

  enum State{
    StateNone,
    StateFocus,
    StateClick
  };

  Type 	 m_type;
  State  m_state;
public:
  kgmGuiButton();
  kgmGuiButton(kgmGui *par, uint x, uint y, uint w, uint h);
  virtual ~kgmGuiButton();
  
  virtual void onClick();
  State getState(){ return m_state; }

private:
  void onMsLeftUp(int key, int x, int y);
  void onMsLeftDown(int key, int x, int y);
  void onMsInside();
  void onMsOutside();
};
