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
public:
  gchandle   m_target      = null;
  gchandle   m_tx_lightmap = null;

  kgmShader* m_sh_phong = null;
  kgmShader* m_sh_toon  = null;
  kgmShader* m_sh_lmap  = null;

  bool m_lightmap = false;

public:
  LightRender(kgmGraphics* gr);
  ~LightRender();

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
  void shader(kgmShader*, kgmCamera*, kgmMaterial*, kgmIGraphics::INode*, Light[8]);
  u32  collect(kgmCamera*, kgmIGraphics::INode*, Light[8]);
};

#endif // LIGHTRENDER_H
