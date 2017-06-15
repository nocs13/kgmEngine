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

void LightRender::l_render(kgmList<kgmLight*>& lights, kgmArray<kgmVisual*>& visuals)
{

  vec3 lpositions[MAX_LIGHTS], ldirections[MAX_LIGHTS], lcolors[MAX_LIGHTS];
  f32  lintensities[MAX_LIGHTS], langles[MAX_LIGHTS];

  kgmList<kgmVisual*> alpha;

  kgmShader* s = gr->shaders.get(kgmShader::TypeLight).data();

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

    if (tnormal && tnormal->m_texture)
      gr->gc->gcSetTexture(0, tnormal->m_texture);

    if (tnormal && tspecular->m_texture)
      gr->gc->gcSetTexture(1, tspecular->m_texture);

    kgmList<kgmLight*>::iterator j = lights.begin();

    u32 k = 0;

    while (j != lights.end()) {
      kgmLight* l = (*j);

      ++j;

      lpositions[k]   = l->position;
      ldirections[k]  = l->direction;
      lcolors[k]      = vec3(l->color.x, l->color.y, l->color.z);
      lintensities[k] = l->intensity;
      langles[k]      = l->angle;

      k++;

      if (k == 15 || j == lights.end()) {
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
