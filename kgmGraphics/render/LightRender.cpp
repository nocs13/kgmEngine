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
  gc->gcBlend(true, gcblend_srcalpha, gcblend_one);

  kgmShader* s = gr->m_shaders[kgmShader::TypePhong];

  for(u32 il = 0; il < gr->m_a_light_count; il++)
  {
    kgmIGraphics::INode* light = gr->m_a_lights[il];

    for(u32 im = 0; im < gr->m_a_meshes_count; im++)
    {
      kgmIGraphics::INode* mesh = gr->m_a_meshes[il];

      gr->render(mesh->getNodeMaterial());


    }
  }

  gc->gcBlend(false, null, null);
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
