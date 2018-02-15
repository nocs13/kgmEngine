#ifndef RNDAMBIENT_H
#define RNDAMBIENT_H

#include "BaseRender.h"

class kgmGraphics;

class RndAmbient : public BaseRender
{
public:
  RndAmbient(kgmGraphics* g);

  void render();
};

#endif // RNDAMBIENT_H
