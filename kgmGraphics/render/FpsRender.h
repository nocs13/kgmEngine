#ifndef FPSRENDER_H
#define FPSRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "../kgmMaterial.h"
#include "BaseRender.h"


class kgmGraphics;

class FpsRender: public BaseRender
{
  mtx4 mi;

  kgmString text;

  kgmMaterial mtl;

public:
  FpsRender(kgmGraphics* g);
  void render();
};

#endif // FPSRENDER_H
