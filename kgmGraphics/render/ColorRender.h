#ifndef COLORRENDER_H
#define COLORRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"
#include "BaseRender.h"

class ColorRender : public BaseRender
{
  kgmShader* m_sh_color = null;
public:
  ColorRender(kgmGraphics* gr);

  void render();
};

#endif // COLORRENDER_H
