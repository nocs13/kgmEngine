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

    kgmMaterial* mtl = (np->getNodeMaterial()) ? (np->getNodeMaterial()) : (gr->m_def_material);

    //gr->render(mtl);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

    gr->gc->gcDepth(false, true, gccmp_lequal);
    gr->gc->gcSetTexture(0, mtl->getTexColor()->texture());
    gr->gc->gcBlend(true, gcblend_srcicol, gcblend_one);
    gr->render(gr->m_shaders[kgmShader::TypeBase]);

    kgmMesh* pm = pr->getMesh();

    //gr->render(pm);
    kgmMesh::Vertex_P_C_T* v = (kgmMesh::Vertex_P_C_T*)pm->vertices();

    for(u32 i = 0; i < pr->count(); i++)
    {
      gr->gc->gcDraw(gcpmt_triangles, pm->fvf(), pm->vsize(),
                     6, &v[i * 6], 2, 0, null);

    }
    gr->gc->gcDepth(true, true, gccmp_lequal);
    gr->gc->gcBlend(false, gcblend_one, gcblend_one);
  }

  gr->render((kgmMaterial*)null);
}
