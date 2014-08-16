#ifndef KGUITEXTVARIABLE_H
#define KGUITEXTVARIABLE_H

#include "../kgmGraphics/kgmGuiText.h"

class kGuiTextVariable: public kgmGuiText
{
public:
  kGuiTextVariable()
  {

  }

  kGuiTextVariable(kgmGui* parent, int x, int y, int w, int z)
    :kgmGuiText(parent, x, y, w, h)
  {

  }
};

#endif // KGUITEXTVARIABLE_H
