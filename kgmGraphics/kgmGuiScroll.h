#pragma once
#include "kgmGui.h"

class kgmGuiScroll: public kgmGui{
  KGM_OBJECT(kgmGuiScroll);

  enum ORIENT
  {
    ORIENT_VERTICAL,
    ORIENT_HORIZONTAL
  };

  u32    m_range;
  u32    m_position;

  bool   m_drag;
  s32    m_dx, m_dy;

  ORIENT m_orientation;
 public:
  kgmGuiScroll();
  kgmGuiScroll(kgmGui *par, int x, int y, int w, int h);
  ~kgmGuiScroll();

  Rect getScrollerRect();

  void setRange(u32 r);
  void setOrientation(ORIENT o);

  void onMsMove(int key, u32 x, u32 y);
  void onMsLeftDown(int key, u32 x, u32 y);
  void onMsLeftUp(int key, u32 x, u32 y);
  void onChange(u32 pos);
};
