#pragma once

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "../kgmMaterial.h"

class kgmGraphics;

class GuiRender
{
  kgmGraphics* gr;

  mtx4 mi;

  kgmMaterial mtl;

public:
  GuiRender(kgmGraphics* gr);

  void render();

private:
  void render(kgmGui* gui);
  void renderGuiMenuItem(kgmGui* menu, void *i);
};
