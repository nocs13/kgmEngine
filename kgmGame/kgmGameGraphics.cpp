#include "kgmGameGraphics.h"

kgmGameGraphics::kgmGameGraphics(kgmIGC *g, kgmIResources* r)
  :kgmGraphics(g, r)
{
}

kgmGameGraphics::~kgmGameGraphics()
{
}

void kgmGameGraphics::render_3d()
{
#ifdef EDITOR

  gc3DMode();

  kgmGraphics::render(m_def_material);
  kgmGraphics::render(m_shaders[kgmShader::TypeBase]);

  if (m_gridline)
    kgmGraphics::render((kgmMesh*) m_gridline);

  if (m_pivot)
    kgmGraphics::render((kgmMesh*) m_pivot);

  kgmGraphics::render((kgmShader*) null);

#endif
}

void kgmGameGraphics::render_2d()
{
#ifdef EDITOR
#endif
}
