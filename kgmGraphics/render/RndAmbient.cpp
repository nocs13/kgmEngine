#include "RndAmbient.h"
#include "kgmGraphics.h"

RndAmbient::RndAmbient(kgmGraphics* g)
  :BaseRender(g)
{
  gr = g;
}

void RndAmbient::render()
{
  mtx4 m4_identity;
  m4_identity.identity();

  gr->setWorldMatrix(m4_identity);

  gr->m_a_light = gr->m_def_light;

  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode*       nod = gr->m_a_meshes[i];
    kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
    kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);

    box3    bbound = nod->getNodeBound();
    sphere3 sbound;

    bbound.min    = nod->getNodeTransform() * bbound.min;
    bbound.max    = nod->getNodeTransform() * bbound.max;

    sbound.center = bbound.center();
    sbound.radius = 0.5f * bbound.dimension().length();

    mtx4 m = nod->getNodeTransform();
    gr->setWorldMatrix(m);

    gr->render(mtl);

    gr->render(gr->m_shaders[kgmShader::TypeAmbient]);

    gr->render(msh);

    gr->render((kgmMaterial*)null);
    gr->render((kgmShader*)null);
  }
}
