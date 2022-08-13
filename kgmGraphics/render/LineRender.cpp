#include "LineRender.h"
#include "../../kgmGraphics/kgmGraphics.h"

#define MAX_LIGHTS 8

const u32 g_res = 512;

LineRender::LineRender(kgmGraphics* g)
  :BaseRender(g)
{
}

LineRender::~LineRender()
{
}


void LineRender::render()
{
  kgmShader* s = gr->m_shaders[kgmIGraphics::ShaderLines];

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

    if (nod->getNodeShader() != kgmIGraphics::ShaderLines)
      continue;

    gr->setWorldMatrix(m);
    gr->set(mtl);

    gr->shaderSetPrivate();
    draw(msh);
  }
}
