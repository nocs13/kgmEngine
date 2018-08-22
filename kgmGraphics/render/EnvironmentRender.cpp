#include "EnvironmentRender.h"
#include "../kgmCamera.h"
#include "../kgmGraphics.h"

#include "../kgmSystem/inc/GL/gl.h"

vec3 move(0, 0, 0);

EnvironmentRender::EnvironmentRender(kgmGraphics* g)
  :BaseRender(g)
{
  m_target   = gc->gcGenTarget(512, 512, true, false);

  m_tx_cube       = gc->gcGenTexture(null, 512, 512, gctex_fmt24, gctype_texcube);
  m_tx_plane      = gc->gcGenTexture(null, 512, 512, gctex_fmt24, gctype_tex2d);
  m_tx_refraction = gc->gcGenTexture(null, 512, 512, gctex_fmt24, gctype_tex2d);

  m_sd_cube  = gr->rc->getShader("envcube.glsl");
  m_sd_plane = gr->rc->getShader("envplane.glsl");

  m_cubemapside = 0;

  gc->gcSet(gcpar_cubemapside, &m_cubemapside);
}

EnvironmentRender::~EnvironmentRender()
{
  gc->gcFreeTarget(m_target);
  gc->gcFreeTexture(m_tx_cube);
  gc->gcFreeTexture(m_tx_plane);
  gc->gcFreeTexture(m_tx_refraction);
}

void EnvironmentRender::render()
{
  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode* nod = gr->m_a_meshes[i];

    kgmMaterial* mtl = nod->getNodeMaterial();

    if (!mtl || mtl->envType() != kgmMaterial::EnvironmentTypeNone)
      render(nod);
  }

  for (s32 i = 0; i < gr->m_a_bmeshes_count; i++)
  {
    kgmIGraphics::INode* nod = gr->m_a_bmeshes[i];
    kgmMaterial* mtl = nod->getNodeMaterial();

    if (!mtl ||mtl->envType() != kgmMaterial::EnvironmentTypeNone)
      render(nod);
  }
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

  vec4 col  = mtl->color();
  vec4 spec = mtl->specular();

  kgmShader* sh = null;
  gchandle   tx = null;

  if (mtl->envMapping() == kgmMaterial::EnvironmentMappingCube)
  {
    m_discard = n;

    reflection(p, b, m_tx_cube);
    sh = m_sd_cube;
    tx = m_tx_cube;

    if (mtl->transparency() > 0.01)
      refraction(p, b, m_tx_refraction);
  }
  else if (mtl->envMapping() == kgmMaterial::EnvironmentMappingPlane)
  {
    m_discard = n;

    reflection(p, nr, gr->m_camera->mPos.z, m_tx_plane);

    sh = m_sd_plane;
    tx = m_tx_plane;

    if (mtl->transparency() > 0.01)
      refraction(p, nr, m_tx_refraction);
  }
  else
  {
    return;
  }

  if (!sh)
  {
    return;
  }

  gc->gcSetViewport(0, 0, gr->m_viewport.width(), gr->m_viewport.height(),
                    gr->m_camera->mNear, gr->m_camera->mFar);

  if (mtl->getTexNormal())
    gc->gcSetTexture(1, mtl->getTexNormal()->texture());
  else
    gc->gcSetTexture(1, gr->m_tex_gray->texture());

  if (mtl->transparency() > 0.01)
    gc->gcSetTexture(2, m_tx_refraction);
  else
    gc->gcSetTexture(2, gr->m_tex_white->texture());

  gc->gcSetTexture(3, tx);

  m.m[0] += 0.001;
  m.m[5] += 0.001;
  m.m[10] += 0.001;

  vec3 cn = gr->m_camera->mPos - vec3(m.m[12], m.m[13], m.m[14]);

  cn.normalize();
  m.m[12] += 0.001 * cn.x;
  m.m[13] += 0.001 * cn.y;
  m.m[14] += 0.001 * cn.z;

  move.x += 0.001 * mtl->move().x;
  move.y += 0.001 * mtl->move().y;

  f32 force = mtl->envIntensity();
  f32 random = (f32) rand() / (f32) RAND_MAX;
  f32 fresnel = mtl->fresnel();
  f32 distortion = 0.02 + mtl->distortion();

  gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
  //gc->gcBlend(true, 0, gcblend_dstcol, gcblend_zero);
  //gc->gcBlend(true, 0, gcblend_one, gcblend_one);

  sh->start();
  sh->set("g_mProj", gr->m_camera->mProj);
  sh->set("g_mView", gr->m_camera->mView);
  sh->set("g_mTran", m);
  sh->set("g_vUp",   gr->m_camera->mUp);
  sh->set("g_vEye",  gr->m_camera->mPos);
  sh->set("g_vLook", gr->m_camera->mDir);
  sh->set("g_vColor",  col);
  sh->set("g_vMove",   move);
  sh->set("g_fRandom", random);
  sh->set("g_fFresnel", fresnel);
  sh->set("g_fForce",   force);
  sh->set("g_fDistort", distortion);

  sh->set("g_txNormal", 1);
  sh->set("g_txSpecular", 2);
  sh->set("g_txEnvironment", 3);

  gc->gcBegin();
  gr->render(mesh);
  gc->gcEnd();

  sh->stop();
  gc->gcBlend(false, 0, 0, 0);
}

void EnvironmentRender::reflection(vec3 pos, box bnd, gchandle tex)
{
  kgmCamera cam;

  f32 shift = 0.5 * bnd.min.distance(bnd.max);

  kgmGraphics::Options o;

  o.width = 512;
  o.height = 512;

  o.clipping = false;
  o.light    = true;

  for (u32 i = 0; i < 6; i++)
  {
    m_cubemapside = i;
    gc->gcSet(gcpar_cubemapside, &m_cubemapside);
    prepare(cam, pos, shift, m_cubemapside);
    gc->gcTexTarget(m_target, tex, gctype_texcube);
    gr->render(m_target, cam, o);
  }
}

void EnvironmentRender::reflection(vec3 pos, vec3 nor, f32 dis, gchandle tex)
{
  kgmCamera cam;

  vec3 cpos = gr->m_camera->mPos;
  vec3 cdir = gr->m_camera->mDir;
  f32  fov  = gr->m_camera->mFov;
  f32  asp  = gr->m_camera->mAspect;
  f32  zfar  = gr->m_camera->mFar;
  f32  zner  = gr->m_camera->mNear;

  cpos.z *= -1;
  cdir.z *= -1;

  cam.set(fov, asp, zner, zfar, cpos, cdir, vec3(0, 0, 1));

  kgmGraphics::Options o;

  o.width = 512;
  o.height = 512;

  o.clipping = true;
  o.light    = false;

  nor.normalize();

  plane plane(nor, pos);
  o.plane[0] = plane.x;
  o.plane[1] = plane.y;
  o.plane[2] = plane.z;
  o.plane[3] = plane.w;

  o.discard = m_discard;

  gc->gcTexTarget(m_target, tex, gctype_tex2d);
  gr->render(m_target, cam, o);
}

void EnvironmentRender::refraction(vec3 pos, box bb, gchandle tex)
{
  kgmCamera cam;

  vec3 cpos = gr->m_camera->mPos;
  vec3 cdir = gr->m_camera->mDir;
  f32  fov  = gr->m_camera->mFov;
  f32  asp  = gr->m_camera->mAspect;

  cam.set(fov, asp, gr->m_camera->mNear, gr->m_camera->mFar, cpos, cdir, vec3(0, 0, 1));

  kgmGraphics::Options o;

  o.width = 512;
  o.height = 512;

  o.light = true;
  o.clipping = false;

  o.discard = m_discard;

  gc->gcTexTarget(m_target, tex, gctype_tex2d);
  gr->render(m_target, cam, o);
}

void EnvironmentRender::refraction(vec3 pos, vec3 nor, gchandle tex)
{
  kgmCamera cam;

  vec3 cpos = gr->m_camera->mPos;
  vec3 cdir = gr->m_camera->mDir;
  f32  fov  = gr->m_camera->mFov;
  f32  asp  = gr->m_camera->mAspect;

  cam.set(fov, asp, gr->m_camera->mNear, gr->m_camera->mFar, cpos, cdir, vec3(0, 0, 1));

  kgmGraphics::Options o;

  o.width = 512;
  o.height = 512;

  o.clipping = true;

  vec3 inor = nor;
  inor.normalize();
  inor.invert();
  o.plane[0] = inor.x;
  o.plane[1] = inor.y;
  o.plane[2] = inor.z;
  o.plane[3] = pos.length();

  o.discard = m_discard;

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
