#pragma once
#include "kgmGui.h"
#include "../kgmBase/kgmCallback.h"


class kgmGuiButton: public kgmGui
{
  KGM_OBJECT(kgmGuiButton);

  enum Type
  {
    TypeNormal,
    TypeCheck,
    TypeRadio,
  };

  enum State
  {
    StateNone,
    StateFocus,
    StateClick
  };

  typedef kgmCallback<void(kgmObject*)> ClickEventCallback;

  Type   m_type;
  State  m_state;

  ClickEventCallback callback;

public:
  kgmGuiButton();
  kgmGuiButton(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  kgmGuiButton(kgmGui *par, u32 x, u32 y, u32 w, u32 h, ClickEventCallback call);
  virtual ~kgmGuiButton();
  
  virtual void onClick();

  State getState()
  {
    return m_state;
  }

  void  setClickCallback(ClickEventCallback call)
  {
    callback = call;
  }

private:
  void onMsLeftUp(int key, int x, int y);
  void onMsLeftDown(int key, int x, int y);
  void onMsInside();
  void onMsOutside();
};
