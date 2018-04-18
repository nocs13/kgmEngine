#ifndef SHADOWRENDER_H
#define SHADOWRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "BaseRender.h"

class ShadowRender: public BaseRender
{
public:
  ShadowRender(kgmGraphics* gr);

  void render();
};

#endif // SHADOWRENDER_H
