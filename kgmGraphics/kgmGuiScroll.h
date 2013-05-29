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

  ORIENT m_orientation;
 public:
  kgmGuiScroll();
  kgmGuiScroll(kgmGui *par, int x, int y, int w, int h);
  ~kgmGuiScroll();

  void setRange(u32 r);
  void setOrientation(ORIENT o);
  void onMsMove(int key, u32 x, u32 y);

  void onChange(u32 pos);
};
