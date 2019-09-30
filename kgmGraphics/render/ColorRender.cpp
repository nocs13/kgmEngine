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
  kgmShader* s = gr->m_shaders[kgmGraphics::ShaderBase];

  kgmCamera* cam = gr->m_camera;

  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode*       nod = gr->m_a_meshes[i];
    kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
    kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);

    mtx4     m = nod->getNodeTransform();
    vec4 color = mtl->color();

    material(mtl);

    s->start();

    s->set("g_fShine",          mtl->shininess());
    s->set("g_mProj",           cam->mProj);
    s->set("g_mView",           cam->mView);
    s->set("g_mTran",           m);
    s->set("g_vColor",          color);
    s->set("g_vUp",             cam->mUp);
    s->set("g_vEye",            cam->mPos);
    s->set("g_vLook",           cam->mDir);
    s->set("g_iClipping",       0);

    s->set("g_txColor", 0);

    draw(msh);

    s->stop();

    material(null);
  }
}
