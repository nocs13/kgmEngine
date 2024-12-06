#include "SceneRenderBasic.h"
#include "../kgmCamera.h"

SceneRenderBasic::SceneRenderBasic(kgmGraphics* gr)
:BaseRender(gr)
{

}

SceneRenderBasic::~SceneRenderBasic()
{

}

void SceneRenderBasic::preRender()
{

}

void SceneRenderBasic::Render()
{
  mtx4 m;

  bool lighting = false;

  m_a_meshes_count    = 0;
  m_a_bmeshes_count   = 0;
  m_a_particles_count = 0;

  m_light_data.count = 0;

  //prepare for render

  gc->gcSetTarget(null);
  gc->gcSetViewport(0, 0, m_viewport.width(), m_viewport.height(), m_camera->mNear, m_camera->mFar);

  gc->gcCull(1);
  //gc->gcCull(0);

  setProjMatrix(m_camera->mProj, m_g_mtx_proj);
  setViewMatrix(m_camera->mView, m_g_mtx_view, m_g_mtx_normal);
  m_g_mtx_world.identity();

  gc->gcBegin();
  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, 0xff000000, 1, 0);


  gc->gcBlend(false, 0, null, null);


  gc->gcDepth(true, true, gccmp_lequal);

  gc->gcSetShader(null);

  gc->gcCull(gccull_back);

  gc->gcRender();
}

void SceneRenderBasic::postRender()
{

}

