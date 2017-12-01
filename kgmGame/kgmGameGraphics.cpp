#include "kgmGameGraphics.h"

kgmGameGraphics::kgmGameGraphics(kgmIGC *g, kgmIResources* r)
  :kgmGraphics(g, r)
{
}

kgmGameGraphics::~kgmGameGraphics()
{
}

void kgmGameGraphics::render_a()
{
#ifdef EDITOR

  kgmGraphics::render(m_def_material);
  kgmGraphics::render(m_shaders[kgmShader::TypeBase]);

  if (m_gridline)
    kgmGraphics::render((kgmMesh*) m_gridline);

  if (m_pivot)
    kgmGraphics::render((kgmMesh*) m_pivot);

  kgmGraphics::render((kgmShader*) null);

#endif
}
