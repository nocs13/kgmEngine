#ifndef ENVIRONMENTRENDER_H
#define ENVIRONMENTRENDER_H

#include "BaseRender.h"
#include "../kgmCamera.h"

class EnvironmentRender: public BaseRender
{
  gchandle m_target = null;

public:
  EnvironmentRender(kgmGraphics* gr);
  ~EnvironmentRender();

  void render();

  void render(vec3, box, gchandle);

  void prepare(kgmCamera&, vec3, f32, u32);
};

#endif // ENVIRONMENTRENDER_H
