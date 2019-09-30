#include "LightRender.h"
#include "../../kgmBase/kgmTime.h"
#include "../../kgmGraphics/kgmIGraphics.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmVisual.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

#ifndef MAX_LIGHTS
#define MAX_LIGHTS 8
#endif

const u32 g_res = 512;

LightRender::LightRender(kgmGraphics* g)
  :BaseRender(g)
{
  //m_sh_toon  = gr->m_shaders[kgmMaterial::TypeToon];
  //m_sh_phong = gr->m_shaders[kgmMaterial::TypePhong];

  //m_sh_phong = gr->rc->getShader("phong2.glsl");
  //m_sh_phong = gr->rc->getShader("phongbase.glsl");
  //m_sh_lmap  = gr->rc->getShader("phonglmap.glsl");

  m_target      = gc->gcGenTarget(g_res, g_res, true, false);

  gc->gcSet(gcpar_texfltmag, (void*)gcflt_linear);
  gc->gcSet(gcpar_texfltmin, (void*)gcflt_linear);
  m_tx_lightmap = gc->gcGenTexture(null, g_res, g_res, gctex_fmt24, gctype_tex2d);

}

LightRender::~LightRender()
{
  gc->gcFreeTarget(m_target);
  gc->gcFreeTexture(m_tx_lightmap);
}


void LightRender::render()
{
  mtx4 m4_identity;
  m4_identity.identity();

  gr->setWorldMatrix(m4_identity);

  kgmShader* sh = m_sh_phong;

  gr->wired(false);

  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode* nod = gr->m_a_meshes[i];

    Light lights[MAX_LIGHTS];

    u32 lcount = collect(gr->m_camera, nod, lights);

    if (lcount < 1)
      continue;

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

    material(mtl);
    shader(sh, gr->m_camera, mtl, nod, lights);

    gr->render(msh);

    material(null);
    shader(null, null, null, null, null);
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

    gr->render(sh);

    gr->render(msh);

    gr->render((kgmMaterial*)null);
    gr->render((kgmShader*)null);
  }

  gr->wired(false);
}

void LightRender::render(kgmCamera* cam, kgmIGraphics::INode* nod)
{
  if (!cam || !nod || nod->getNodeType() != kgmIGraphics::NodeMesh)
    return;

  kgmShader* s = m_sh_phong;
  kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
  kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);

  box3    bbound = nod->getNodeBound();
  sphere3 sbound;

  bbound.min = nod->getNodeTransform() * bbound.min;
  bbound.max = nod->getNodeTransform() * bbound.max;

  sbound.center = bbound.center();
  sbound.radius = 0.5f * bbound.dimension().length();

  mtx4 m = nod->getNodeTransform();

  kgmIGraphics::INode* light = null;

  for(kgmList<kgmIGraphics::INode*>::iterator i = gr->m_lights.begin(); !i.end(); i.next())
  {
    if(!(*i)->isNodeValid())
      continue;

    vec3 pos = (*i)->getNodePosition();

    kgmLight* l = (kgmLight*) (*i)->getNodeObject();

    if(!cam->isSphereCross(pos, kgmLight::LIGHT_RANGE * l->intensity()))
      continue;

    if (!light)
    {
      light = (*i);
    }
    else
    {
      f32 lforce[2];
      lforce[0] = kgmLight::LIGHT_RANGE * l->intensity();
      lforce[1] = kgmLight::LIGHT_RANGE * ((kgmLight*)light->getNodeObject())->intensity();

      if (lforce[0] > lforce[1])
        light = (*i);
    }
  }

  if (!light)
    light = gr->m_def_light;

  mtx4 transform = nod->getNodeTransform();

  vec4 v_light;
  vec4 v_light_color;
  vec4 v_light_direction;

  if (light)
  {
    kgmLight* l   = (kgmLight*) light->getNodeObject();
    vec3      pos = light->getNodePosition();

    v_light           = vec4(pos.x, pos.y, pos.z, l->intensity());
    v_light_direction = vec4(l->direction().x, l->direction().y, l->direction().z, l->angle());
    v_light_color     = vec4(l->color().x, l->color().y, l->color().z, 1.0);
  }

  vec4 color     = mtl->m_color.get();
  vec4 specular  = mtl->m_specular.get();
  f32  shininess = mtl->shininess();

  gchandle tcolor = null;
  gchandle tnormal = null;
  gchandle tspecular = null;

  if(mtl->hasTexColor())
  {
    gc->gcSetTexture(0, mtl->getTexColor()->texture());
    tcolor = mtl->getTexColor()->texture();
  }
  else
  {
    gc->gcSetTexture(0, gr->m_tex_white->texture());
    tcolor = gr->m_tex_white->texture();
  }

  if(mtl->hasTexNormal())
  {
    gc->gcSetTexture(1, mtl->getTexNormal()->texture());
    tnormal = mtl->getTexNormal()->texture();
  }
  else
  {
    gc->gcSetTexture(1, gr->m_tex_gray->texture());
    tnormal = gr->m_tex_gray->texture();
  }

  if(mtl->hasTexSpecular())
  {
    gc->gcSetTexture(2, mtl->getTexSpecular()->texture());
    tspecular = mtl->getTexSpecular()->texture();
  }
  else
  {
    gc->gcSetTexture(2, gr->m_tex_white->texture());
    tspecular = gr->m_tex_white->texture();
  }

  if (!s)
    return;

  s->start();

  s->set("g_fShine",          shininess);
  s->set("g_mProj",           cam->mProj);
  s->set("g_mView",           cam->mView);
  s->set("g_mTran",           m);
  s->set("g_vColor",          color);
  s->set("g_vSpecular",       specular);
  s->set("g_vLight",          v_light);
  s->set("g_vLightColor",     v_light_color);
  s->set("g_vLightDirection", v_light_direction);
  s->set("g_vUp",             cam->mUp);
  s->set("g_vEye",            cam->mPos);
  s->set("g_vLook",           cam->mDir);
  s->set("g_iClipping",       0);

  s->set("g_txColor", 0);
  s->set("g_txNormal", 1);
  s->set("g_txSpecular", 2);

  draw(msh);

  s->stop();

  gc->gcSetTexture(0, 0);
  gc->gcSetTexture(1, 0);
  gc->gcSetTexture(2, 0);
  gc->gcSetTexture(3, 0);
}

void LightRender::material(kgmMaterial* m)
{
  if(!m)
  {
    gc->gcSetTexture(0, 0);
    gc->gcSetTexture(1, 0);
    gc->gcSetTexture(2, 0);
    gc->gcSetTexture(3, 0);

    gc->gcCull(1);
    gc->gcBlend(false, 0, 0, 0);
    gc->gcDepth(true, true, gccmp_lequal);

    return;
  }

  if(m->blend())
  {
    switch(m->blend())
    {
    case kgmMaterial::Blend_Add:
      gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_one);
      //gc->gcBlend(true, gcblend_one, gcblend_one);
      break;
    case kgmMaterial::Blend_Mul:
      gc->gcBlend(true, 0, gcblend_dstcol, gcblend_zero);
      break;
    case kgmMaterial::Blend_Sub:
      gc->gcBlend(true, gcblend_eqsub, gcblend_dstcol, gcblend_zero);
      break;
    case kgmMaterial::Blend_Inter:
      gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
      break;
    case kgmMaterial::Blend_CBurn:
      gc->gcBlend(true, 0, gcblend_one, gcblend_srcicol);
      break;
    case kgmMaterial::Blend_LBurn:
      gc->gcBlend(true, 0, gcblend_one, gcblend_one);
      break;
    case kgmMaterial::Blend_CDodge:
      gc->gcBlend(true, 0, gcblend_dstcol, gcblend_zero);
      break;
    case kgmMaterial::Blend_LDodge:
      gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_one);
      break;
    case kgmMaterial::Blend_Screen:
      gc->gcBlend(true, 0, gcblend_one, gcblend_srcicol);
      break;
    case kgmMaterial::Blend_Darken:
      gc->gcBlend(true, gcblend_eqmin, gcblend_one, gcblend_one);
      break;
    case kgmMaterial::Blend_Lighten:
      gc->gcBlend(true, gcblend_eqmax, gcblend_one, gcblend_one);
      break;
    }
  }
  else if(m->transparency() > 0.0f)
  {
    gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
  }

  if(!m->depth())
  {
    gc->gcDepth(false, true, gccmp_less);
  }


  if(!m->cull())
  {
    gc->gcCull(0);
  }

  if(m->hasTexColor())
  {
    gc->gcSetTexture(0, m->getTexColor()->texture());
  }
  else
  {
    gc->gcSetTexture(0, gr->m_tex_white->texture());
  }

  if(m->hasTexNormal())
  {
    gc->gcSetTexture(1, m->getTexNormal()->texture());
  }
  else
  {
    gc->gcSetTexture(1, gr->m_tex_gray->texture());
  }

  if(m->hasTexSpecular())
  {
    gc->gcSetTexture(2, m->getTexSpecular()->texture());
  }
  else
  {
    gc->gcSetTexture(2, gr->m_tex_black->texture());
  }
}

void LightRender::shader(kgmShader* s, kgmCamera* cam, kgmMaterial* mtl, kgmIGraphics::INode* nod, Light lights[8])
{
  if(!s)
  {
    gc->gcSetShader(null);

    return;
  }

  //send default parameters
  vec4 v_light(0, 0, 0, 10);
  vec4 v_light_color(1, 1, 1, 1);
  vec4 v_light_direction(0, 0, 1, 0);

  float random = (float)rand() / (float)RAND_MAX;
  mtx4  transform = nod->getNodeTransform();
  vec4 color = mtl->m_color.get();
  vec4 specular = mtl->m_specular.get();
  f32  shininess = mtl->shininess();
  f32  time = kgmTime::getTime();

  s->start();
  s->set("g_fTime",           time);
  s->set("g_fRandom",         random);
  s->set("g_fShine",          shininess);
  s->set("g_mProj",           cam->mProj);
  s->set("g_mView",           cam->mView);
  s->set("g_mTran",           transform);
  s->set("g_vColor",          color);
  s->set("g_vSpecular",       specular);
  s->set("g_vLight",          v_light);
  s->set("g_vLightColor",     v_light_color);
  s->set("g_vLightDirection", v_light_direction);
  s->set("g_vUp",             cam->mUp);
  s->set("g_vEye",            cam->mPos);
  s->set("g_vLook",           cam->mDir);
  s->set("g_iClipping",       0);

  s->set("g_txColor", 0);
  s->set("g_txNormal", 1);
  s->set("g_txSpecular", 2);

  kgmString lcolor = "g_vLights[*].color";
  kgmString lposition = "g_vLights[*].position";
  kgmString ldirection = "g_vLights[*].direction";


  for(u32 i = 0; i < 8; i++)
  {
    char c = '0' + (char) i;
    lcolor[10] = c;
    lposition[10] = c;
    ldirection[10] = c;

     s->set(lcolor, lights[i].color);
     s->set(lposition, lights[i].position);
     s->set(ldirection, lights[i].direction);
  }
}

u32 LightRender::collect(kgmCamera* cam, kgmIGraphics::INode* nod, Light lights[])
{
  if (!cam || !nod || nod->getNodeType() != kgmIGraphics::NodeMesh)
    return 0;

  u32 count = 0;

  box  bb = nod->getNodeBound();

  vec3 pos = nod->getNodePosition();

  for(kgmList<kgmGraphics::INode*>::iterator i = gr->m_lights.begin(); !i.end(); i.next())
  {
    memset(&lights[count], 0, sizeof(Light));

    if(!(*i)->isNodeValid())
      continue;

    vec3 pos = (*i)->getNodePosition();

    kgmLight* l = (kgmLight*) (*i)->getNodeObject();

    if (!l->active())
      continue;

    if(!cam->isSphereCross(pos, kgmLight::LIGHT_RANGE * l->intensity()))
      continue;

    lights[count].color = vec4(l->color().x, l->color().y, l->color().z, 1.0);
    lights[count].position = vec4(pos.x, pos.y, pos.z, l->intensity());
    lights[count].direction = vec4(l->direction().x, l->direction().y, l->direction().z, l->angle());

    count++;

    if(count >= MAX_LIGHTS)
      break;
  }

  return count;
}

void LightRender::lightmap()
{
  kgmShader* s = m_sh_lmap;

  kgmCamera* cam = gr->m_camera;

  kgmString lcolor = "g_vLights[*].color";
  kgmString lposition = "g_vLights[*].position";
  kgmString ldirection = "g_vLights[*].direction";

  gc->gcTexTarget(m_target, m_tx_lightmap, gctype_tex2d);
  gc->gcSetTarget(m_target);

  gc->gcSetViewport(0, 0, g_res, g_res, 0.1, 1000);

  gc->gcCull(gccull_back);
  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, 0xff000000, 1, 0);

  gc->gcBlend(false, 0, null, null);

  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode* nod = gr->m_a_meshes[i];

    Light lights[MAX_LIGHTS];

    u32 lcount = collect(gr->m_camera, nod, lights);

    if (lcount < 1)
      continue;

    kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
    kgmMaterial* mtl = nod->getNodeMaterial();

    if (!mtl || mtl->envType() == kgmMaterial::EnvironmentTypeNone)
      continue;

    gchandle tcolor = gr->m_tex_white->texture();
    gchandle tnormal = (mtl->getTexNormal())?(mtl->getTexNormal()->texture()):(gr->m_tex_gray->texture());
    gchandle tspecular = (mtl->getTexSpecular())?(mtl->getTexSpecular()->texture()):(gr->m_tex_black->texture());

    mtx4 m = nod->getNodeTransform();
    vec4 color = vec4(1, 1, 1, 1);
    vec4 specular = mtl->specular();
    f32  shininess = mtl->shininess();

    gc->gcSetTexture(0, tcolor);
    gc->gcSetTexture(1, tnormal);
    gc->gcSetTexture(2, tspecular);

    s->start();

    s->set("g_fShine",          shininess);
    s->set("g_mProj",           cam->mProj);
    s->set("g_mView",           cam->mView);
    s->set("g_mTran",           m);
    s->set("g_vColor",          color);
    s->set("g_vSpecular",       specular);
    s->set("g_vUp",             cam->mUp);
    s->set("g_vEye",            cam->mPos);
    s->set("g_vLook",           cam->mDir);
    s->set("g_iClipping",       0);

    s->set("g_txColor", 0);
    s->set("g_txNormal", 1);
    s->set("g_txSpecular", 2);

    for(u32 i = 0; i < 8; i++)
    {
      char c = '0' + (char) i;
      lcolor[10] = c;
      lposition[10] = c;
      ldirection[10] = c;

       s->set(lcolor, lights[i].color);
       s->set(lposition, lights[i].position);
       s->set(ldirection, lights[i].direction);
    }

    draw(msh);

    s->stop();

    material(null);
  }

  gc->gcSetViewport(0, 0, gr->m_viewport.width(), gr->m_viewport.height(), gr->m_camera->mNear, gr->m_camera->mFar);
  gc->gcSetTarget(null);

  if (!m_lightmap)
    m_lightmap = true;
}
