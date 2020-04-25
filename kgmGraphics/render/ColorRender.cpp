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
  kgmShader* s = gr->m_shaders[kgmGraphics::ShaderColor];

  if (!s)
    return;

  kgmCamera* cam = gr->m_camera;

  //s->start();
  gr->set(s);
  gr->shaderSetGeneral();

  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode*       nod = gr->m_a_meshes[i];
    kgmIMesh*     msh = (kgmIMesh*) nod->getNodeObject();
    kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);

    mtx4     m = nod->getNodeTransform();

    gr->setWorldMatrix(m);
    gr->set(mtl);
    //vec4 color = mtl->color();

    //material(mtl);

    //s->start();

    //s->set("g_fShine",          mtl->shininess());
    //s->set("g_mProj",           cam->mProj);
    //s->set("g_mView",           cam->mView);
    //s->set("g_mTran",           m);
    //s->set("g_vColor",          color);
    //s->set("g_vUp",             cam->mUp);
    //s->set("g_vEye",            cam->mPos);
    //s->set("g_vLook",           cam->mDir);
    //s->set("g_iClipping",       0);
    //s->set("g_txColor", 0);

    gr->shaderSetPrivate();
    draw(msh);

    //s->stop();
    //material(null);
  }
}
