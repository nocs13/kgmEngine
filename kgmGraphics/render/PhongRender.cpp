#include "PhongRender.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"


PhongRender::PhongRender(kgmGraphics* g)
  :BaseRender(g)
{
}

PhongRender::~PhongRender()
{
}

void PhongRender::render()
{
  kgmShader* s = gr->m_shaders[kgmGraphics::ShaderPhong];

  if (!s)
    return;

  kgmCamera* cam = gr->m_camera;

  gr->set(s);
  gr->shaderSetGeneral();

  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode*       nod = gr->m_a_meshes[i];
    kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
    kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);

    mtx4     m = nod->getNodeTransform();

    if (nod->getNodeShading() != kgmIGraphics::ShadingFull)
      continue;

    gr->setWorldMatrix(m);
    gr->set(mtl);

    gr->shaderSetPrivate();
    draw(msh);
  }    
}
