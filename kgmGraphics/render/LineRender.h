#pragma once

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "../kgmIGraphics.h"
#include "BaseRender.h"

class LineRender: public BaseRender
{
public:
public:
  LineRender(kgmGraphics* gr);
  ~LineRender();

  void render();
};

