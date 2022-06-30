#include "kgmGuiContainer.h"


kgmGuiContainer::kgmGuiContainer()
    :kgmGui(){
}

kgmGuiContainer::kgmGuiContainer(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
  :kgmGui(par, x, y, w, h)
{
}

kgmGuiContainer::~kgmGuiContainer()
{
}

