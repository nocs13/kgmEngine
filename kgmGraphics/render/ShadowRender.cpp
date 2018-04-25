#include "ShadowRender.h"
#include "../../kgmGraphics/kgmGraphics.h"

ShadowRender::ShadowRender(kgmGraphics* g)
  :BaseRender(g)
{

}

void ShadowRender::render()
{
  kgmCamera cm;

  for (u32 i = 0; i < gr->m_shadows.length(); i++)
  {
    kgmGraphics::Shadow* s = &gr->m_shadows[i];

    gr->gc->gcSetTarget(s->fbo);
    gr->gc->gcSetViewport(0, 0, s->w, s->h, 1.0f, 1000.0f);

    cm.set(gr->camera().mFov, 1.0f, 1.0f, 1000.f, s->lpos, s->ldir, vec3(0, 0, 1));

    gr->setProjMatrix(s->mp);
    gr->setViewMatrix(s->mv);

    gr->gc->gcBegin();
    gr->gc->gcDepth(true, true, gccmp_lequal);
    gr->gc->gcClear(gcflag_color | gcflag_depth, 0x00, 1.0f, 0);

    for(u32 j = 0; j < gr->m_a_meshes_count; j++)
    {
      kgmIGraphics::INode* n = gr->m_a_meshes[j];

      kgmMesh* m = (kgmMesh*)n->getNodeObject();

      mtx4 mt = n->getNodeTransform();
      gr->setWorldMatrix(mt);

      gr->render(gr->m_def_material);
      gr->render(gr->m_shaders[kgmGraphics::ShaderShadowKeep]);

      gr->render(m);

    }

    gr->gc->gcEnd();
    gr->gc->gcSetTarget(null);
  }

  gr->gc->gcSetViewport(gr->m_viewport.x, gr->m_viewport.y,
                        gr->m_viewport.w, gr->m_viewport.h, 1.0f, 1000.0f);

  gr->setProjMatrix(gr->camera().mProj);
  gr->setViewMatrix(gr->camera().mView);

  //gr->gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);

  for (u32 i = 0; i < gr->m_shadows.length(); i++)
  {
    kgmGraphics::Shadow* s = &gr->m_shadows[i];

    for(u32 j = 0; j < gr->m_a_meshes_count; j++)
    {
      kgmIGraphics::INode* n = gr->m_a_meshes[j];

      kgmMesh* m = (kgmMesh*)n->getNodeObject();

      gr->render(gr->m_def_material);
      gr->gc->gcSetTexture(0, gr->gc->gcTexTarget(s->fbo));

      mtx4 mt = n->getNodeTransform();
      gr->setWorldMatrix(mt);

      kgmShader* sh = gr->m_shaders[kgmGraphics::ShaderShadowDraw];

      gr->render(sh);
      sh->set("g_mLight", s->mvp);

      gr->render(m);
      gr->render((kgmShader*)null);
    }
  }

  gr->gc->gcBlend(false, 0, gcblend_srcalpha, gcblend_srcialpha);
}
