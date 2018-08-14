#ifndef ENVIRONMENTRENDER_H
#define ENVIRONMENTRENDER_H

#include "BaseRender.h"
#include "../kgmCamera.h"
#include "../kgmIGraphics.h"

class EnvironmentRender: public BaseRender
{
public:
  gchandle   m_target         = null;
  gchandle   m_tx_plane       = null;
  gchandle   m_tx_cube        = null;
  gchandle   m_tx_refraction  = null;

  kgmShader* m_sd_cube  = null;
  kgmShader* m_sd_plane = null;

  u32 m_cubemapside = 0;

  bool m_refraction = false;

  kgmIGraphics::INode* m_discard = null;

public:
  EnvironmentRender(kgmGraphics* gr);
  ~EnvironmentRender();

  void render();

  void render(kgmIGraphics::INode*);
  void render(vec3, box, gchandle);
  void render(vec3, vec3, f32, gchandle);

  void refraction(vec3, box, gchandle);
  void refraction(vec3, vec3, gchandle);

  void prepare(kgmCamera&, vec3, f32, u32);
};

#endif // ENVIRONMENTRENDER_H
