#ifndef ICONRENDER_H
#define ICONRENDER_H

#include "../../kgmMath/kgmMath.h"
#include "BaseRender.h"

class kgmGraphics;

class IconRender : public BaseRender
{
public:
  IconRender(kgmGraphics* gr);

  void render();
  void render(kgmList<kgmLight*>&);

private:
  void render(kgmIcon* icon);
};

#endif // ICONRENDER_H
