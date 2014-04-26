#pragma once
#include "kgmGui.h"


class kgmGuiLabel: public kgmGui
{
  KGM_OBJECT(kgmGuiLabel)

public:
  kgmGuiLabel();
  kgmGuiLabel(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
  virtual ~kgmGuiLabel();
};
