#include "ShadowRender.h"
#include "../../kgmGraphics/kgmGraphics.h"

#define MAX_SHADOWS 2

ShadowRender::ShadowRender(kgmGraphics* g)
  :BaseRender(g)
{
  m_mvps.alloc(MAX_SHADOWS);
  m_textures.alloc(MAX_SHADOWS);

  m_target = gc->gcGenTarget(512, 512, false, false);

  for (u32 i = 0; i < MAX_SHADOWS; i++)
  {
    m_textures[i] = gc->gcGenTexture(null, 512, 512, 24, gctype_texdepth);
  }
}

ShadowRender::~ShadowRender()
{
  gc->gcFreeTarget(m_target);

  for (u32 i = 0; i < MAX_SHADOWS; i++)
  {
    gc->gcFreeTexture(m_textures[i]);
  }
}

void ShadowRender::render()
{
  u32       cmask = 0x00000000;

  /*
  m_shadows[0].valid = true;

  m_shadows[0].lpos  = m_a_light->getNodePosition();
  m_shadows[0].ldir  = vec3(.1, .1, -1);
  m_shadows[0].ldir.normalize();

  {
    mtx4 mp, mv, mb;

    f32 b[16] = {0.5, 0, 0, 0,
                 0,   0.5, 0, 0,
                 0,   0,   0.5, 0,
                 0.5, 0.5, 0.5, 1};

    mp.perspective(PI / 6, 1.0, 1.0, 20);
    mv.lookat(m_shadows[0].lpos, m_shadows[0].ldir, vec3(0, 0, 1));
    mb = mtx4(b);

    m_shadows[0].mv = mv;
    m_shadows[0].mp = mp;
    m_shadows[0].mvp = mb * mv * mp;
  }
  */

  u32 count = MAX_SHADOWS;

  if (count > gr->m_a_light_count)
    count = gr->m_a_light_count;

  for (u32 i = 0; i < count; i++)
  {
    mtx4 mp, mv, mb;

    vec3 lpos  = gr->m_a_lights[i]->getNodePosition();
    vec3 ldir  = ((kgmLight*)gr->m_a_lights[i]->getNodeObject())->direction();

    mp.perspective(PI / 6, 1.0, 1.0, 20);
    mv.lookat(lpos, ldir, vec3(0, 0, 1));
    m_mvps[i] = mv * mp;

    gc->gcSetTarget(m_target);
    gc->gcTexTarget(m_target, m_textures[i], gctype_texdepth);

    gc->gcSetViewport(0, 0, 512, 512, .1f, 1000.0f);

    gr->setProjMatrix(mp);
    gr->setViewMatrix(mv);

    gc->gcDepth(true, true, gccmp_lequal);
    gc->gcClear(gcflag_depth, 0x00, 1.0f, 0);

    gc->gcBegin();

    for(u32 j = 0; j < gr->m_a_meshes_count; j++)
    {
      kgmIGraphics::INode* n = gr->m_a_meshes[j];

      kgmMesh* m = (kgmMesh*)n->getNodeObject();

      mtx4 mt = n->getNodeTransform();
      gr->setWorldMatrix(mt);

      gr->set(gr->m_def_material);
      gr->set(gr->m_shaders[kgmGraphics::ShaderShadowKeep]);

      gr->draw(m);
    }

    gc->gcEnd();
    gc->gcSetTarget(null);
  }

  gc->gcSetViewport(gr->m_viewport.x, gr->m_viewport.y,
                        gr->m_viewport.w, gr->m_viewport.h, .1f, 1000.0f);

  gr->setProjMatrix(gr->camera().mProj);
  gr->setViewMatrix(gr->camera().mView);

  cmask = 0xffffffff;
  gc->gcSet(gcpar_colormask, &cmask);
  gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);

  for (u32 i = 0; i < count; i++)
  {
    for(u32 j = 0; j < gr->m_a_meshes_count; j++)
    {
      kgmIGraphics::INode* n = gr->m_a_meshes[j];

      kgmMesh* m = (kgmMesh*)n->getNodeObject();

      gr->set(gr->m_def_material);
      gc->gcSetTexture(0, m_textures[i]);

      mtx4 mt = n->getNodeTransform();
      gr->setWorldMatrix(mt);

      kgmShader* sh = gr->m_shaders[kgmGraphics::ShaderShadowDraw];

      gr->set(sh);
      sh->set("g_mLight", m_mvps[i]);

      gr->draw(m);
      gr->set((kgmShader*)null);
    }
  }

  gc->gcBlend(false, 0, gcblend_srcalpha, gcblend_srcialpha);
}
