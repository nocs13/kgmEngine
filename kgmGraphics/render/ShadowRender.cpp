#include "ShadowRender.h"
#include "../../kgmGraphics/kgmGraphics.h"

ShadowRender::ShadowRender(kgmGraphics* g)
  :BaseRender(g)
{

}

void ShadowRender::render()
{
  kgmCamera *cm = gr->m_camera;

  f32 fov  = cm->mFov;
  f32 asp  = cm->mAspect;
  f32 zn   = cm->mNear;
  f32 zf   = cm->mFar;
  vec3 pos = cm->mPos;
  vec3 dir = cm->mDir;
  vec3 zup = cm->mUp;

  for (u32 i = 0; i < gr->m_shadows.length(); i++)
  {
    kgmGraphics::Shadow* s = &gr->m_shadows[i];

    gr->gc->gcSetTarget(s->fbo);
    gr->gc->gcSetViewport(0, 0, s->w, s->h, 1.0f, 1000.0f);
    cm->set(fov, 1.0f, 1.0f, 1000.f, s->lpos, s->ldir, vec3(0, 0, 1));

    gr->gc->gcBegin();
    gr->gc->gcDepth(true, true, gccmp_lequal);
    gr->gc->gcClear(gcflag_color | gcflag_depth, 0xff, 1.0f, 0);

    for(u32 j = 0; j < gr->m_a_meshes_count; j++)
    {
      kgmIGraphics::INode* n = gr->m_a_meshes[j];

      kgmMesh* m = (kgmMesh*)n->getNodeObject();
      gr->render(gr->m_shaders[kgmShader::TypeNone]);
      gr->render(gr->m_def_material);
      gr->render(m);

    }

    gr->gc->gcEnd();
    gr->gc->gcSetTarget(null);
  }

  cm->set(fov, asp, zn, zf, pos, dir, zup);
  gr->gc->gcSetViewport(gr->m_viewport.x, gr->m_viewport.y,
                        gr->m_viewport.w, gr->m_viewport.h, 1.0f, 1000.0f);
}
