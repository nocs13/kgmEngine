#ifndef SHADOWRENDER_H
#define SHADOWRENDER_H

#include "../../kgmBase/kgmIGC.h"
#include "BaseRender.h"

class ShadowRender: public BaseRender
{
  gchandle m_target = null;
  kgmArray<gchandle> m_textures;
  kgmArray<mtx4>     m_mvps;

public:
  ShadowRender(kgmGraphics* gr);
  ~ShadowRender();

  void render();
};

#endif // SHADOWRENDER_H
