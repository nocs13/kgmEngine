#pragma once
#include "kgmGui.h"
#include "../kgmBase/kgmCallback.h"


class kgmGuiCheck: public kgmGui
{
  KGM_OBJECT(kgmGuiCheck);
public:

  enum State
  {
    StateNone,
    StateFocus,
  };

  typedef kgmCallback<void, kgmObject*, bool> ClickEventCallback;

  State  m_state;
  bool   m_check;

  ClickEventCallback callback;

public:
  kgmGuiCheck();
  kgmGuiCheck(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  kgmGuiCheck(kgmGui *par, u32 x, u32 y, u32 w, u32 h, ClickEventCallback call);
  virtual ~kgmGuiCheck();

  State getState()
  {
    return m_state;
  }

  bool isCheck()
  {
    return m_check;
  }

  void setCheck(bool check)
  {
    m_check = check;
  }

  void  setClickCallback(ClickEventCallback call)
  {
    callback = call;
  }

private:
  void onMsLeftUp(int key, int x, int y);
  //void onMsLeftDown(int key, int x, int y);
  void onMsInside();
  void onMsOutside();
};
