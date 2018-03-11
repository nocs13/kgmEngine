#ifndef PARTICLESRENDER_H
#define PARTICLESRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "BaseRender.h"

class ParticlesRender : public BaseRender
{
public:
  ParticlesRender(kgmGraphics* gr);

  void render();
};

#endif // PARTICLESRENDER_H
