#pragma once

#include "../../kgmBase/kgmIGC.h"
#include "BaseRender.h"

class LightmapRender: public BaseRender
{
  struct Light
  {
    vec4 position;
    vec4 direction;
    vec4 color;
  };

public:
  gchandle   m_target      = null;
  gchandle   m_tx_lightmap = null;

  bool m_lightmap = false;

public:
  LightmapRender(kgmGraphics* gr);
  ~LightmapRender();

  void render();
  void render(kgmCamera*, kgmIGraphics::INode*);

  void basic();
  void lightmap();

  gchandle getLightmap() const
  {
    if (m_lightmap)
      return m_tx_lightmap;

    return null;
  }

private:
  void material(kgmMaterial*);
  void shader(kgmShader*, kgmCamera*, kgmMaterial*, kgmIGraphics::INode*, Light*, u32);
  u32  collect(kgmCamera*, kgmIGraphics::INode*, Light[8]);
};
