#ifndef LIGHTRENDER_H
#define LIGHTRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "../kgmIGraphics.h"
#include "BaseRender.h"

class LightRender: public BaseRender
{
  kgmShader* m_sh_phong = null;
  kgmShader* m_sh_toon  = null;

public:
  LightRender(kgmGraphics* gr);

  void render();
  void render(kgmCamera*, kgmIGraphics::INode*);
};

#endif // LIGHTRENDER_H
