#include "ParticlesRender.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

ParticlesRender::ParticlesRender(kgmGraphics* g)
  :BaseRender(g)
{
}

void ParticlesRender::render()
{
  mtx4 m4_identity;
  m4_identity.identity();

  gr->setWorldMatrix(m4_identity);

  gr->render((kgmMaterial*)null);

  for(s32 i = 0; i < gr->m_a_particles_count; i++)
  {
    kgmIGraphics::INode* np = gr->m_a_particles[i];

    kgmParticles* pr = (kgmParticles*) np->getNodeObject();

    kgmMaterial* mtl = (np->getNodeMaterial()) ? (np->getNodeMaterial()) : (gr->m_def_material);

    gr->render(mtl);

    gr->render(gr->m_shaders[kgmShader::TypeBase]);
    gr->gc->gcDepth(true, false, gccmp_lequal);
    gr->gc->gcBlend(true, gcblend_srcalpha, gcblend_one);

    kgmMesh* pm = pr->getMesh();

    gr->render(pm);

    gr->gc->gcBlend(false, gcblend_one, gcblend_one);
    gr->gc->gcDepth(true, true, gccmp_lequal);
  }

  gr->render((kgmMaterial*)null);
}
