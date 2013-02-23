#pragma once
#include "kgmGui.h"

class kgmGuiScroll: public kgmGui{
  KGM_OBJECT(kgmGuiScroll);
  u32  m_range;
  u32  m_position;
  u322 m_dimension;

  u32  m_colScroll;
 public:
  kgmGuiScroll();
  kgmGuiScroll(kgmGui *par, int x, int y, int w, int h);
  ~kgmGuiScroll();

  void setRange(u32 r);
  void onMsMove(int key, u32 x, u32 y);
};
