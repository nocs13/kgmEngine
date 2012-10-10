#pragma once
#include "kgmGui.h"

class kgmGuiScroll: public kgmGui{
  KGM_OBJECT(kgmGuiScroll);
  uint  m_range;
  uint  m_position;
  uint2 m_dimension;

  uint  m_colScroll;
 public:
  kgmGuiScroll();
  kgmGuiScroll(kgmGui *par, int x, int y, int w, int h);
  ~kgmGuiScroll();

  void setRange(uint r);
  void onMsMove(int key, uint x, uint y);
};
