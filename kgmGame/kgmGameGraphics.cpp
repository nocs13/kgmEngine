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

  mtx4 mi;

  mi.identity();
  setWorldMatrix(mi);

  kgmGraphics::render(m_def_material);
  kgmGraphics::render(m_shaders[kgmShader::TypeBase]);

  if (m_gridline)
    kgmGraphics::render((kgmMesh*) m_gridline);

  if (m_pivot)
  {
    mtx4 mt = m_pivot->getTransform();
    setWorldMatrix(mt);
    kgmGraphics::render(m_shaders[kgmShader::TypeBase]);
    kgmGraphics::render((kgmMesh*) m_pivot);
  }

  kgmGraphics::render((kgmShader*) null);

#endif
}

void kgmGameGraphics::render_2d()
{
#ifdef EDITOR
#endif
}
