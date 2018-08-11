#include "EnvironmentRender.h"
#include "../kgmCamera.h"
#include "../kgmGraphics.h"

EnvironmentRender::EnvironmentRender(kgmGraphics* g)
  :BaseRender(g)
{
  m_target   = gc->gcGenTarget(512, 512, true, false);
  m_tx_plane = gc->gcGenTexture(null, 512, 512, gctex_fmt24, gctype_tex2d);
  m_tx_cube  = gc->gcGenTexture(null, 512, 512, gctex_fmt24, gctype_texcube);
  m_sd_cube  = gr->rc->getShader("envcube.glsl");
  m_sd_plane = gr->rc->getShader("envplane.glsl");

  m_cubemapside = 5;

  gc->gcSet(gcpar_cubemapside, &m_cubemapside);
}

EnvironmentRender::~EnvironmentRender()
{
  gc->gcFreeTarget(m_target);
}

void EnvironmentRender::render()
{
}

void EnvironmentRender::render(kgmIGraphics::INode* n)
{
  if (!n || n->getNodeType() != kgmIGraphics::NodeMesh)
    return;

  kgmMesh* mesh = (kgmMesh*) n->getNodeObject();

  if (!mesh)
    return;

  kgmMaterial* mtl = n->getNodeMaterial();

  if (!mtl)
    return;

  mtx4 m = n->getNodeTransform();
  vec3 p = vec3(m.m[12], m.m[13], m.m[14]);
  box3 b = mesh->bound();

  vec3 nr = mesh->normal();

  vec4 col = mtl->m_color.get();
  col.w = mtl->envIntensity();

  kgmShader* sh = null;
  gchandle   tx = null;

  if (mtl->envMapping() == kgmMaterial::EnvironmentMappingCube)
  {
    render(p, b, m_tx_cube);
    sh = m_sd_cube;
    tx = m_tx_cube;
  }
  else if (mtl->envMapping() == kgmMaterial::EnvironmentMappingPlane)
  {
    render(p, nr, gr->m_camera->mPos.z, m_tx_plane);
    sh = m_sd_plane;
    tx = m_tx_plane;
  }
  else
  {
    return;
  }

  gc->gcSetViewport(0, 0, gr->m_viewport.width(), gr->m_viewport.height(),
                    gr->m_camera->mNear, gr->m_camera->mFar);
  gc->gcSetTexture(3, tx);

  //gc->gcBlend(true, 0, gcblend_one, gcblend_one);
  sh->start();
  sh->set("g_mProj",  gr->m_camera->mProj);
  sh->set("g_mView",  gr->m_camera->mView);
  sh->set("g_mTran",  m);
  sh->set("g_vColor", col);

  sh->set("g_txEnvironment", 3);

  gc->gcBegin();
  gr->render(mesh);
  gc->gcEnd();

  sh->stop();
  gc->gcBlend(false, 0, 0, 0);
}

void EnvironmentRender::render(vec3 pos, box bnd, gchandle tex)
{
  kgmCamera cam;

  f32 shift = 0.5 * bnd.min.distance(bnd.max);

  kgmGraphics::Options o;

  o.width = 512;
  o.height = 512;

  for (u32 i = 0; i < 6; i++)
  {
    m_cubemapside = i;
    gc->gcSet(gcpar_cubemapside, &m_cubemapside);
    prepare(cam, pos, shift, m_cubemapside);
    gc->gcTexTarget(m_target, tex, gctype_texcube);
    gr->render(m_target, cam, o);
  }
}

void EnvironmentRender::render(vec3 pos, vec3 nor, f32 dis, gchandle tex)
{
  kgmCamera cam;

  f32 shift = dis;

  vec3 cpos = gr->m_camera->mPos;
  vec3 cdir = gr->m_camera->mDir;
  f32  fov  = gr->m_camera->mFov;
  f32  asp  = gr->m_camera->mAspect;

  cpos.z *= -1;
  cdir.z *= -1;

  cam.set(fov, asp, 0.1, 1000, cpos, cdir, vec3(0, 0, 1));
  //cam.set(PI / 6, 1, 0.1, 1000, cpos, cdir, vec3(0, 0, 1));

  kgmGraphics::Options o;

  o.width = 512;
  o.height = 512;

  gc->gcTexTarget(m_target, tex, gctype_tex2d);
  gr->render(m_target, cam, o);
}

void EnvironmentRender::prepare(kgmCamera& cam, vec3 pos, f32 shift, u32 face)
{
  vec3 cpos, cdir, cup;

  cpos = pos;

  switch(face)
  {
  case 0:
    //cpos.x += shift;
    cdir = vec3(1, 0.0, 0.0);
    cup = vec3(0, 0, 1);
    break;
  case 1:
    //cpos.x -= shift;
    cdir = vec3(-1, 0.0, 0.0);
    cup = vec3(0, 0, 1);
    break;
  case 2:
    //cpos.y += shift;
    cdir = vec3(0.0, 0.0, -1);
    cup = vec3(0, 1, 0);
    break;
  case 3:
    //cpos.y -= shift;
    cdir = vec3(0.0, 0.0, 1);
    cup = vec3(0, -1, 0);
    break;
  case 4:
    //cpos.z += shift;
    cdir = vec3(0.0, 1, 0.0);
    cup = vec3(0, 0, 1);
    break;
  case 5:
    //cpos.z -= shift;
    cdir = vec3(0.0, -1, 0.0);
    cup = vec3(0, 0, 1);
    break;
  }

  f32  cfov = gr->m_camera->mFov;
  f32  casp = gr->m_camera->mAspect;
  f32  cfar = gr->m_camera->mFar;
  f32  cner = gr->m_camera->mNear;

  cam.set(cfov, casp, cner, cfar, cpos, cdir, cup);
}
