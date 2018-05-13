#ifndef WATERRENDER_H
#define WATERRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "BaseRender.h"

class kgmGraphics;

class WaterRender: public BaseRender
{
  kgmGraphics* gr;

public:
  WaterRender(kgmGraphics* gr);

  void build();
  void clear();

  void render();
};

#endif // WATERRENDER_H
