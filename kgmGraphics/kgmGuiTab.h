#pragma once
#include "kgmGui.h"


class kgmGuiTab: public kgmGui
{
  KGM_OBJECT(kgmGuiTab);

private:
  u32	  m_index;

public:
 kgmGuiTab();
 kgmGuiTab(kgmGui *par, int x, int y, u32 w, u32 h);
 virtual ~kgmGuiTab();

 u32 set(u32 i);
 u32 next();
 u32 previous();
};




