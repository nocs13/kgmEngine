#pragma once

#include "BaseRender.h"

class Render: public BaseRender
{
  mtx4 mi;
  kgmString text;
  kgmMaterial mtl;

public:
  Render(kgmGraphics* gr);
  ~Render();

  void render();

  void renderMeshes(kgmCamera& cam, kgmList<OLight>& olights, kgmList<OMesh>& meshes);
  void renderBMeshes(kgmCamera& cam, kgmList<OLight>& olights, kgmList<OMesh>& bmeshes);

  void renderGuiMenuItem(kgmGui* m, void *i);
  void renderGuis(kgmList<kgmGui*>& guis);
  void renderGui(kgmGui* gui);

  void renderFPS();
};