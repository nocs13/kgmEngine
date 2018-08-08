#include "LightRender.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmVisual.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#define MAX_LIGHTS 16

LightRender::LightRender(kgmGraphics* g)
  :BaseRender(g)
{

}

void LightRender::render()
{
  mtx4 m4_identity;
  m4_identity.identity();

  gr->setWorldMatrix(m4_identity);

  //kgmShader* shader = gr->m_shaders[kgmMaterial::TypeToon];
  kgmShader* shader = gr->m_shaders[kgmMaterial::TypePhong];

  gr->wired(false);

  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode*       nod = gr->m_a_meshes[i];
    kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
    kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);

    box3    bbound = nod->getNodeBound();
    sphere3 sbound;

    bbound.min = nod->getNodeTransform() * bbound.min;
    bbound.max = nod->getNodeTransform() * bbound.max;

    sbound.center = bbound.center();
    sbound.radius = 0.5f * bbound.dimension().length();

    mtx4 m = nod->getNodeTransform();
    gr->setWorldMatrix(m);

    gr->render(mtl);
    
    if (mtl->envType())
    {
      
    }

    for(s32 j = 0; j < gr->m_a_light_count; j++)
    {
      gr->m_a_light = gr->m_a_lights[j];

      if(j > 0)
        gc->gcBlend(true, 0, gcblend_one, gcblend_one);

      gr->render(shader);

      if (mtl->envType() == kgmMaterial::EnvironmentTypeNone)
        gr->render(msh);

      if(j > 0)
        gc->gcBlend(false, 0, gcblend_one, gcblend_one);
    }

    gr->render((kgmMaterial*)null);
    gr->render((kgmShader*)null);

    if (mtl->envType() != kgmMaterial::EnvironmentTypeNone)
    {
      gr->m_rnd_environment->render(nod);
    }
  }

  gc->gcBlend(false, 0, null, null);

  gr->m_a_light = gr->m_a_lights[0];

  for (s32 i = 0; i < gr->m_a_bmeshes_count; i++)
  {
    kgmIGraphics::INode*       nod = gr->m_a_bmeshes[i];
    kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
    kgmMaterial* mtl = nod->getNodeMaterial();

    if(!mtl)
      continue;

    mtx4 m = nod->getNodeTransform();
    gr->setWorldMatrix(m);

    gr->render(mtl);

    gr->render(shader);

    gr->render(msh);

    gr->render((kgmMaterial*)null);
    gr->render((kgmShader*)null);
  }

  gr->wired(false);
}

/*
void LightRender::render()
{

  vec3 lpositions[MAX_LIGHTS], ldirections[MAX_LIGHTS], lcolors[MAX_LIGHTS];
  f32  lintensities[MAX_LIGHTS], langles[MAX_LIGHTS];

  kgmList<kgmVisual*> alpha;

  kgmShader* s = gr->m_shaders[kgmShader::TypePhong];

  if (!gr || !s)
    return;

  mtx4 im = gr->camera().mView;

  im.invert();

  mtx3 g_mtx_normal;

  g_mtx_normal[0] = im[0], g_mtx_normal[1] = im[1], g_mtx_normal[2] = im[2];
  g_mtx_normal[3] = im[4], g_mtx_normal[4] = im[5], g_mtx_normal[5] = im[6];
  g_mtx_normal[6] = im[8], g_mtx_normal[7] = im[9], g_mtx_normal[8] = im[10];

  gc->gcBlend(true, gcblend_srcalpha, gcblend_one);

  for (int i = 0; i < visuals.length(); i++) {
    kgmVisual* v = visuals[i];

    if (!v)
      continue;

    kgmMaterial* m = v->getMaterial();

    if (m && m->alpha()) {
      alpha.add(v);

      continue;
    }

    kgmTexture* tnormal  = m->getTexNormal();
    kgmTexture* tspecular = m->getTexSpecular();

    if (tnormal && tnormal->texture())
      gr->gc->gcSetTexture(0, tnormal->texture());

    if (tnormal && tspecular->texture())
      gr->gc->gcSetTexture(1, tspecular->texture());

    kgmList<kgmLight*>::iterator j = lights.begin();

    u32 k = 0;

    while (!j.end()) {
      kgmLight* l = (*j);

      ++j;

      //lpositions[k]   = l->position;
      ldirections[k]  = l->direction;
      //lcolors[k]      = vec3(l->color.x, l->color.y, l->color.z);
      lintensities[k] = l->intensity;
      langles[k]      = l->angle;

      k++;

      if (k == 15 || j.end()) {
        s->start();
        s->set("g_mProj",           gr->camera().mProj);
        s->set("g_mView",           gr->camera().mView);
        s->set("g_mTran",           v->getTransform());
        s->set("g_mNorm",           g_mtx_normal);
        //s->set("g_vLight",          l->position);
        //s->set("g_vLightColor",     l->color);
        //s->set("g_vLightDirection", l->direction);
        s->set("g_vEye",            gr->camera().mPos);
        s->set("g_vEyeDir",         gr->camera().mDir);

        s->set("g_txNormal", 0);
        s->set("g_txSpecular", 1);

        s->set("g_vLpositions", lpositions[0], k);
        s->set("g_vLdirections", ldirections[0], k);
        s->set("g_iLights", k);

        draw(v);

        s->stop();
      }
    }
  }

  gc->gcBlend(false, null, null);
}
*/
