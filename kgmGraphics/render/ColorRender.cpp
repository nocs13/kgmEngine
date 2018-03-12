#include "ColorRender.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

ColorRender::ColorRender(kgmGraphics* g)
  :BaseRender(g)
{

}

void ColorRender::render()
{
  mtx4 m4_identity;
  m4_identity.identity();

  gr->setWorldMatrix(m4_identity);

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

    m.m[0]  *= 1.000001f;
    m.m[5]  *= 1.000001f;
    m.m[10] *= 1.000001f;

    gr->setWorldMatrix(m);

    gr->render(mtl);

    gr->render(gr->m_shaders[kgmShader::TypeBase]);

    gc->gcBlend(true, gcblend_dstcol, gcblend_zero);

    gr->render(msh);

    gc->gcBlend(false, null, null);
  }
}
