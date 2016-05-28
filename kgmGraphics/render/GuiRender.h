#pragma once

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"

class GuiRender
{
  class kgmGraphics;

  //kgmIGC* gc;
  //kgmList<kgmGui*>& guis;

  kgmGraphics* gr;

public:
  //GuiRender(kgmIGC* gc, kgmList<kgmGui*>& guis);
  GuiRender(kgmGraphics* gr);

  void render();
};
