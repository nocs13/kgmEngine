#include "LightmapRender.h"
#include "../../kgmSystem/kgmTime.h"
#include "../../kgmGraphics/kgmIGraphics.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

const u32 g_res = 512;

LightmapRender::LightmapRender(kgmGraphics* g)
  :BaseRender(g)
{
  m_target = gc->gcGenTarget(g_res, g_res, true, false);

  gc->gcSet(gcpar_texfltmag, (void*)gcflt_linear);
  gc->gcSet(gcpar_texfltmin, (void*)gcflt_linear);
  m_tx_lightmap = gc->gcGenTexture(null, g_res, g_res, gctex_fmt24, gctype_tex2d);
}

LightmapRender::~LightmapRender()
{
  gc->gcFreeTarget(m_target);
  gc->gcFreeTexture(m_tx_lightmap);
}


void LightmapRender::render()
{
  mtx4 m4_identity;
  m4_identity.identity();

  gr->setWorldMatrix(m4_identity);

  kgmShader* sh = gr->m_shaders[kgmIGraphics::ShaderBase];

  Light lights[MAX_LIGHTS];

  if (gr->m_a_light_count < 1)
    return;

  for (u32 i = 0; i < gr->m_a_light_count; i++)
  {
    kgmLight* l = (kgmLight *) gr->m_a_lights[i]->getNodeObject();

    vec3 vec;


    vec = l->color();
    memcpy(&lights[i].color, &vec, 3 * sizeof(float));

    vec = gr->m_a_lights[i]->getNodePosition();
    memcpy(&lights[i].position, &vec, 3 * sizeof(float));

    vec = l->direction();
    memcpy(&lights[i].direction, &vec, 3 * sizeof(float));

    lights[i].position.w = l->intensity();
    lights[i].direction.w = l->angle();
  }

  for (s32 i = 0; i < gr->m_a_meshes_count; i++)
  {
    kgmIGraphics::INode* nod = gr->m_a_meshes[i];


    kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
    kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);


    mtx4 m = nod->getNodeTransform();
    gr->setWorldMatrix(m);

    material(mtl);
    shader(sh, gr->m_camera, mtl, nod, lights, gr->m_a_light_count);

    gr->draw(msh);

    material(null);
    shader(null, null, null, null, null, null);
  }

  gc->gcBlend(false, 0, null, null);

  for (s32 i = 0; i < gr->m_a_bmeshes_count; i++)
  {
    kgmIGraphics::INode*       nod = gr->m_a_bmeshes[i];
    kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
    kgmMaterial* mtl = nod->getNodeMaterial();

    if(!mtl)
      continue;

    mtx4 m = nod->getNodeTransform();
    gr->setWorldMatrix(m);

    gr->set(mtl);

    gr->set(sh);

    gr->draw(msh);

    gr->set((kgmMaterial*)null);
    gr->set((kgmShader*)null);
  }

  gr->wired(false);
}

void LightmapRender::render(kgmCamera* cam, kgmIGraphics::INode* nod)
{
  if (!cam || !nod || nod->getNodeType() != kgmIGraphics::NodeMesh)
    return;

  kgmShader* s = gr->m_shaders[kgmGraphics::ShaderBase];
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

  for(kgmList<kgmIGraphics::INode*>::iterator i = gr->m_lights.begin(); i != gr->m_lights.end(); i.next())
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

void LightmapRender::material(kgmMaterial* m)
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
    case kgmMaterial::BlendAdd:
      gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_one);
      //gc->gcBlend(true, gcblend_one, gcblend_one);
      break;
    case kgmMaterial::BlendMul:
      gc->gcBlend(true, 0, gcblend_dstcol, gcblend_zero);
      break;
    case kgmMaterial::BlendSub:
      gc->gcBlend(true, gcblend_eqsub, gcblend_dstcol, gcblend_zero);
      break;
    case kgmMaterial::BlendMix:
    default:
      break;
    /*case kgmMaterial::Blend_Inter:
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
      break;*/
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

void LightmapRender::shader(kgmShader* s, kgmCamera* cam, kgmMaterial* mtl, kgmIGraphics::INode* nod,
                         Light* lights, u32 lcount)
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
  s->set("g_vUp",             cam->mUp);
  s->set("g_vEye",            cam->mPos);
  s->set("g_vLook",           cam->mDir);
  s->set("g_iClipping",       0);
  s->set("g_iLights",         lcount);

  s->set("g_txColor", 0);
  s->set("g_txNormal", 1);
  s->set("g_txSpecular", 2);

  kgmString lcolor = "g_vLightCol[*]";
  kgmString lposition = "g_vLightPos[*]";
  kgmString ldirection = "g_vLightDir[*]";


  for(u32 i = 0; i < lcount; i++)
  {
    char c = '0' + (char) i;
    //lcolor[12] = c;
    //lposition[12] = c;
    //ldirection[12] = c;
	
	memcpy(&((char*)lcolor.data())[12], (void*) &c, 1);
	memcpy(&((char*)lposition.data())[12], (void*) &c, 1);
	memcpy(&((char*)ldirection.data())[12], (void*) &c, 1);
	
    s->set(lcolor,     lights[i].color);
    s->set(lposition,  lights[i].position);
    s->set(ldirection, lights[i].direction);
  }
}

u32 LightmapRender::collect(kgmCamera* cam, kgmIGraphics::INode* nod, Light* lights)
{
  if (!cam || !nod || nod->getNodeType() != kgmIGraphics::NodeMesh)
    return 0;

  u32 count = 0;

  box  bb = nod->getNodeBound();

  vec3 pos = nod->getNodePosition();

  for(kgmList<kgmGraphics::INode*>::iterator i = gr->m_lights.begin(); i != gr->m_lights.end(); i.next())
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

void LightmapRender::lightmap()
{
  kgmShader* s = null;

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
      //lcolor[10] = c;
      //lposition[10] = c;
      //ldirection[10] = c;
	  
      memcpy(&((char*)lcolor.data())[12], (void*) &c, 1);
	  memcpy(&((char*)lposition.data())[12], (void*) &c, 1);
	  memcpy(&((char*)ldirection.data())[12], (void*) &c, 1);


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
