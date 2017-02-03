#ifndef ICONRENDER_H
#define ICONRENDER_H

#include "../../kgmMath/kgmMath.h"
#include "../kgmGraphics.h"
#include "../kgmTexture.h"
#include "../kgmIcon.h"

class IconRender
{
  kgmGraphics* gr;

public:
  IconRender(kgmGraphics* gr);

  void render();
  void render(kgmList<kgmLight*>&);

private:
  void render(kgmIcon* icon);
};

#endif // ICONRENDER_H
