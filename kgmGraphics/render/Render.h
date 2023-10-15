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

  void renderMeshes(kgmCamera& cam, kgmList<kgmIGraphics::INode*>& olights,
                    kgmList<kgmIGraphics::INode*>& meshes, kgmList<kgmIGraphics::INode*>& bmeshes);

  void renderGuiMenuItem(kgmGui* m, void *i);
  void renderGuis(kgmList<kgmGui*>& guis);
  void renderGui(kgmGui* gui);

  void renderFPS();

private:
  void use(kgmMaterial*);
  void use(kgmShader*);
  void shaderSetLights(kgmShader* s, kgmList<kgmIGraphics::INode*>& olights);
  void shaderSetDefaultLight(kgmShader* s);
};