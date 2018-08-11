#ifndef FPSRENDER_H
#define FPSRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"

class kgmGraphics;

class FpsRender
{
  kgmGraphics* gr;

  mtx4 mi;

  kgmString text;

public:
  FpsRender(kgmGraphics* g);
  void render();
};

#endif // FPSRENDER_H
