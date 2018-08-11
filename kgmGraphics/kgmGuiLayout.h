#ifndef KGMGUILAYOUT_H
#define KGMGUILAYOUT_H

#include "kgmGui.h"

class kgmGuiLayout : public kgmGui
{
public:
  kgmGuiLayout();
  kgmGuiLayout(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
};

#endif // KGMGUILAYOUT_H
