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

  gr->set((kgmMaterial*)null);

  for(s32 i = 0; i < gr->m_a_particles_count; i++)
  {
    kgmIGraphics::INode* np = gr->m_a_particles[i];

    kgmParticles* pr = (kgmParticles*) np->getNodeObject();

    kgmMaterial* mtl = (np->getNodeMaterial()) ? (np->getNodeMaterial()) : (gr->m_def_material);

    gr->set(mtl);

    gr->set(gr->m_shaders[kgmMaterial::TypeBase]);
    gr->gc->gcDepth(true, false, gccmp_lequal);

    if (pr->fade())
      gr->gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_one);

    kgmMesh* pm = pr->getMesh();

    if (pm)
      gr->draw(pm);

    if (pr->fade())
      gr->gc->gcBlend(false, 0, gcblend_one, gcblend_one);

    gr->gc->gcDepth(true, true, gccmp_lequal);
  }

  gr->set((kgmMaterial*)null);
}
