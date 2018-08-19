#ifndef LIGHTRENDER_H
#define LIGHTRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "../kgmIGraphics.h"
#include "BaseRender.h"

class LightRender: public BaseRender
{
  struct Light
  {
    vec4 position;
    vec4 direction;
    vec4 color;
  };

  kgmShader* m_sh_phong = null;
  kgmShader* m_sh_toon  = null;

public:
  LightRender(kgmGraphics* gr);

  void render();
  void render(kgmCamera*, kgmIGraphics::INode*);

private:
  void material(kgmMaterial*);
  void shader(kgmShader*, kgmCamera*, kgmMaterial*, kgmIGraphics::INode*, Light[8]);
  u32  collect(kgmCamera*, kgmIGraphics::INode*, Light[8]);
};

#endif // LIGHTRENDER_H
