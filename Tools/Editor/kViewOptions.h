#ifndef KVIEWOPTIONS_H
#define KVIEWOPTIONS_H

#include "../../kgmGraphics/kgmGui.h"
#include "../../kgmGraphics/kgmGuiTab.h"

class kViewOptions : public kgmGui
{
  kgmGuiTab* tab;

public:
  kViewOptions(int x, int y, int w, int h);
};

#endif // KVIEWOPTIONS_H
