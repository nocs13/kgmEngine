#include "SceneRenderBasic.h"

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

  for(kgmList<INode*>::iterator i = m_meshes.begin(); i != m_meshes.end(); i.next())
  {
    if (!(*i)->isNodeValid())
      continue;

    box3 bound = (*i)->getNodeBound();

    vec3  l = bound.max - bound.min;
    vec3  v = (bound.min + bound.max) * 0.5;

    if(!m_camera->isSphereCross(v, 0.5 * l.length())) {
      if (!bound.isin(m_camera->mPos))
        continue;
    }

    kgmMaterial* m = (*i)->getNodeMaterial();

    if(m && (m->blend() != kgmMaterial::BlendNone || m->transparency() > 0.0f))
    {
      if (m_a_bmeshes_count == m_a_bmeshes.length())
        m_a_bmeshes.realloc(m_a_bmeshes_count + 1024);

      m_a_bmeshes[m_a_bmeshes_count] = (*i);
      m_a_bmeshes_count++;
    }
    else
    {
      if (m_a_meshes_count == m_a_meshes.length())
        m_a_meshes.realloc(m_a_meshes_count + 1024);

      m_a_meshes[m_a_meshes_count] = (*i);
      m_a_meshes_count++;
    }
  }

  if (m_a_meshes.length() > m_a_meshes_count)
    m_a_meshes[m_a_meshes_count] = null;

  //prepare for render

  gc->gcSetTarget(null);
  gc->gcSetViewport(0, 0, m_viewport.width(), m_viewport.height(), m_camera->mNear, m_camera->mFar);

  gc->gcCull(1);
  //gc->gcCull(0);

  setProjMatrix(m_camera->mProj);
  setViewMatrix(m_camera->mView);
  m_g_mtx_world.identity();

  gc->gcBegin();
  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, m_bg_color, 1, 0);


  gc->gcBlend(false, 0, null, null);

  //colect lights in viewport

  m_a_light_count = 0;

  m_a_light_count = collectLights(m_camera, m_a_lights, MAX_LIGHTS);

  if (m_a_light_count == 0)
  {
    m_a_lights[0] = m_def_light;

    m_a_light_count = 1;
  }

  m_a_particles_count = collectParticles(m_camera, m_a_particles, 128);

  //draw scene only lights
  set((kgmMaterial*)null);

  if (m_rnd_color)
    m_rnd_color->render();

  lighting = true;

  if (m_rnd_phong)
    m_rnd_phong->render();

  //m_rnd_lights->lightmap();

  //ShadowRender sr(this);
  //sr.render();
  //m_rnd_shadows->render();

  //m_rnd_environment->render();

  //draw terrain
  //if (m_terrain)
  //  m_rnd_terrain->render();

  //draw particles
  //ParticlesRender pr(this);
  //pr.render();

  // Draw alpha objects.

  gc->gcDepth(true, true, gccmp_lequal);

  gc->gcSetShader(null);

  if(m_rnd_lines)
    m_rnd_lines->render();

  gc->gcCull(gccull_back);

  gc->gcRender();
}

void SceneRenderBasic::postRender()
{

}

