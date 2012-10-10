#pragma once
#include "kgmGui.h"


class kgmGuiTab: public kgmGui
{
  KGM_OBJECT(kgmGuiTab);

private:
  uint	  m_index;

public:
 kgmGuiTab();
 kgmGuiTab(kgmGui *par, int x, int y, uint w, uint h);
 virtual ~kgmGuiTab();

 uint set(uint i);
 uint next();
 uint previous();
};




