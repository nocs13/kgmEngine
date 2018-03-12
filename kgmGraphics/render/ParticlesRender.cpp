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

  for(s32 i = 0; i < gr->m_a_particles_count; i++)
  {
    kgmIGraphics::INode* np = gr->m_a_particles[i];

    kgmParticles* pr = (kgmParticles*) np->getNodeObject();

    kgmMesh* pm = pr->getMesh();

    gr->render(pm);
  }

}
