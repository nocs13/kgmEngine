#pragma once
#include "kgmGui.h"
#include "../kgmBase/kgmCallback.h"

class kgmGuiScroll: public kgmGui
{
  KGM_OBJECT(kgmGuiScroll);
public:

  typedef kgmCallback<void, kgmObject*, u32> ChangeEventCallback;

  enum ORIENT
  {
    ORIENT_VERTICAL,
    ORIENT_HORIZONTAL
  };

  u32    m_range;
  u32    m_position;

  //pixel per position
  f32    m_ppp;

  bool   m_drag;
  s32    m_dx, m_dy;
  s32    m_msp;

  ORIENT m_orientation;
  ChangeEventCallback callback;

 public:
  kgmGuiScroll();
  kgmGuiScroll(kgmGui *par, int x, int y, int w, int h);
  ~kgmGuiScroll();

  Rect getScrollerRect();

  void setRange(u32 r);
  void setPosition(u32 p);
  void setOrientation(ORIENT o);

  void setChangeEventCallback(ChangeEventCallback call)
  {
    callback = call;
  }

  void onMsMove(int key, int x, int y);
  void onMsLeftDown(int key, int x, int y);
  void onMsLeftUp(int key, int x, int y);
};
