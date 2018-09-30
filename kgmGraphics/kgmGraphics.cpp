#include "../kgmBase/kgmLog.h"
#include "../kgmMath/kgmBase.h"
#include "../kgmBase/kgmTime.h"

#include "kgmGraphics.h"
#include "kgmGuiProgress.h"
#include "kgmGuiButton.h"
#include "kgmGuiScroll.h"
#include "kgmGuiLabel.h"
#include "kgmGuiCheck.h"
#include "kgmGuiList.h"
#include "kgmGuiMenu.h"
#include "kgmGuiTab.h"
#include "kgmGui.h"
#include "kgmMesh.h"
#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "kgmShader.h"
#include "kgmAnimation.h"
#include "kgmSkeleton.h"
#include "kgmFrustum.h"
#include "kgmCamera.h"

#include "../kgmGame/kgmGameApp.h"
#include "../kgmGame/kgmGameBase.h"

#include "render/LightRender.h"
#include "render/ColorRender.h"
#include "render/ParticlesRender.h"

#include <stdlib.h>

#define MAX_LIGHTS   48
#define MAX_SHADOWS  2

kgmGraphics::GraphicsQuality kgmGraphics::textureQuality = GraphicsQualityHight;
kgmGraphics::GraphicsQuality kgmGraphics::shadowQuality  = GraphicsQualityLow;

struct Vert
{
  vec3 v;
  u32  c;
};

inline float packRgbToFloat(vec4 v)
{
  return ((char)(v.x * 255) << 24) +
      ((char)(v.y * 255) << 16) +
      ((char)(v.z * 255) << 8) +
      (char)(v.w * 255);

  /* decode in shader
float r = (temp & 0xf000)/255.0;
float g = (temp & 0x0f00)/255.0;
float b = (temp & 0x00f0)/255.0;
float a = (temp & 0x000f)/255.0;*/
}

inline vec4 packLight(kgmLight& l, vec3 pos)
{
  float distance = pos.length();
  vec3  normal   = pos.normal();

  kgmVector3d<u16> nc;

  nc.x = (1.0f / normal.x);
  nc.y = (1.0f / normal.y);
  nc.z = (1.0f / normal.z);

  u16 c; /*= (l.color.x * 6 / 256) * 36 +
          (l.color.y * 6 / 256) * 6 +
          (l.color.z * 6 / 256);*/

  u16 i = 1.0f / l.intensity();

  u16 a = l.angle();

  vec4 res;

  res.x = distance;

  u8* p = (u8*) &res.y;

  memcpy(p, &nc.x, 2 * sizeof(u16));
  p += 2 * sizeof(u16);
  memcpy(p, &nc.z, sizeof(u16));
  p += sizeof(u16);
  memcpy(p, &c, sizeof(u16));
  p += sizeof(u16);
  memcpy(p, &i, sizeof(u16));
  p += sizeof(u16);
  memcpy(p, &a, sizeof(u16));

  return res;
}

mtx4       g_mtx_orto;
mtx4       g_mtx_iden;
mtx4*      g_mtx_joints = null;

u32        g_mtx_joints_count = 0;

//vec4       g_vec_light   = vec4(0, 0, 0, 1);
//vec4       g_vec_ambient = vec4(1, 1, 1, 1);

vec4       g_vec_color    = vec4(1, 1, 1, 1);
vec4       g_vec_specular = vec4(1, 1, 1, 1);

kgmShader* g_shd_active = null;

f32        g_fShine = 0.0f;
f32        g_fAmbient = 0.1f;

void*      g_tex_black = null;
void*      g_tex_white = null;
void*      g_tex_gray  = null;
void*      g_map_shadow = null;

kgmTexture*   g_def_style_texture = null;

gchandle g_fbo = null;
gchandle g_tex = null;

inline void sort_lights(kgmLight *lights = null, u32 count = 0, vec3 pos = vec3(0, 0, 0))
{
  if(lights == null || count < 1)
    return;

  for(u32 i = 0; i < count; i++)
  {
    for(u32 j = i + 1; j < count; j++)
    {

    }
  }
}

kgmGraphics::kgmGraphics(kgmIGC *g, kgmIResources* r)
{
  gc = g;
  rc = r;

  font      = null;
  tcolor    = null;
  tdepth    = null;
  shader    = null;
  tnormal   = null;
  tspecular = null;

  m_max_faces = 5000000;

  m_alpha       = false;
  m_depth       = true;
  m_culling     = true;

  m_bg_color    = 0xFF000000;

  m_editor      = false;
  m_wired       = false;

  gui_style = new kgmGuiStyle();


  m_def_material = new kgmMaterial();
  m_def_material->m_color = kgmMaterial::Color(1.0f, 1.0f, 1.0f, 1.0f);
  m_def_material->m_specular = kgmMaterial::Color(1.0f, 1.0f, 1.0f, 1.0f);

  if(g)
  {
    char txd[16] = {0};

    if(!gc)
      return;

    //generic black texture
    memset(txd, 0x00, sizeof(txd));
    g_tex_black = gc->gcGenTexture(txd, 2, 2, gctex_fmt32, gctype_tex2d);
    m_tex_black = new kgmTexture(g_tex_black);

    //generic white texture
    memset(txd, 0xff, sizeof(txd));
    g_tex_white = gc->gcGenTexture(txd, 2, 2, gctex_fmt32, gctype_tex2d);
    m_tex_white = new kgmTexture(g_tex_white);

    //generic gray texture
    memset(txd, 0x80, sizeof(txd));
    g_tex_gray = gc->gcGenTexture(txd, 2, 2, gctex_fmt32, gctype_tex2d);
    m_tex_gray = new kgmTexture(g_tex_gray);

    g_def_style_texture = new kgmTexture(g_tex_white);

    gui_style->gui_image = g_def_style_texture;

    m_def_light = (INode*) new kgmNodeLight();

    g_fbo = g->gcGenTarget(512, 512, true, false);
    g_tex = g->gcGenTexture(null, 512, 512, gctex_fmt24, gctype_tex2d);
    g->gcTexTarget(g_fbo, g_tex, gctype_tex2d);
  }


#ifdef DEBUG
  kgm_log() << "Prepare shaders \n";
#endif

  if(rc != null)
  {
    memset(m_shaders, 0, sizeof(m_shaders));
    m_shaders[kgmMaterial::TypeNone]  = rc->getShader("none.glsl");
    m_shaders[kgmMaterial::TypeBase]  = rc->getShader("base.glsl");
    m_shaders[kgmMaterial::TypeToon]  = rc->getShader("toon.glsl");
    m_shaders[kgmMaterial::TypePhong] = rc->getShader("phong.glsl");
    m_shaders[ShaderShadowKeep]       = rc->getShader("shkeep.glsl");
    m_shaders[ShaderShadowDraw]       = rc->getShader("shdraw.glsl");
  }

  m_rnd_base        = new BaseRender(this);
  m_rnd_lights      = new LightRender(this);
  m_rnd_shadows     = new ShadowRender(this);
  m_rnd_environment = new EnvironmentRender(this);


  m_camera = new kgmCamera();
  m_camera->set(PI / 6, 1, 0.0001, 1000.0, vec3(0, 0, 1), vec3(-1, 0, 0), vec3(0, 0, 1));
  g_mtx_iden.identity();

  m_r_fps    = new FpsRender(this);
  m_r_gui    = new GuiRender(this);
  m_r_sprite = new SpriteRender(this);

  m_a_lights.alloc(MAX_LIGHTS);
}

kgmGraphics::~kgmGraphics()
{
  gc->gcFreeTarget(g_fbo);

  if (m_rnd_lights)
    delete m_rnd_lights;

  if (m_rnd_shadows)
    delete m_rnd_shadows;

  if (m_rnd_environment)
    delete m_rnd_environment;

  delete m_r_fps;
  delete m_r_gui;
  delete m_r_sprite;

  m_particles.clear();
  m_meshes.clear();
  m_lights.clear();
  m_icons.clear();
  m_guis.clear();

  if (m_def_material)
    delete m_def_material;

  if (m_def_light)
    delete m_def_light;

  if(g_tex_black)
    gc->gcFreeTexture(g_tex_black);

  if(g_tex_white)
    gc->gcFreeTexture(g_tex_white);

  if(g_tex_gray)
    gc->gcFreeTexture(g_tex_gray);

  delete m_tex_white;
  delete m_tex_black;
  delete m_tex_gray;

  if (g_def_style_texture)
    delete g_def_style_texture;

  if(gui_style)
    delete gui_style;

  if(m_camera)
    delete m_camera;
}

kgmShader* s_def = null;

void kgmGraphics::build()
{
}

void kgmGraphics::clear()
{
  for (kgmList<kgmResource*>::iterator i = m_resources.begin(); !i.end(); ++i)
  {
    (*i)->release();
  }

  m_resources.clear();
  m_meshes.clear();
  m_lights.clear();
  m_particles.clear();

  m_a_light = m_def_light;

  m_a_light_count  = 0;
  m_a_meshes_count = 0;
}

bool kgmGraphics::resource(kgmResource* r)
{
  if (!r)
    return false;

  kgmResource* keep = resource(r->id());

  if (keep)
    return false;

  m_resources.add(r);

  return true;
}

kgmResource* kgmGraphics::resource(kgmString id)
{
  for (kgmList<kgmResource*>::iterator i = m_resources.begin(); !i.end(); ++i)
  {
    if (id == (*i)->id())
      return (*i);
  }

  return null;
}

void kgmGraphics::setDefaultFont(kgmFont* f)
{
  font = f;

  if(gui_style)
    gui_style->gui_font = f;
}

void kgmGraphics::setProjMatrix(mtx4 &m)
{
  m_g_mtx_proj = m;
}

void kgmGraphics::setViewMatrix(mtx4 &m)
{
  m_g_mtx_view = m;

  mtx4 im = m;

  im.invert();
  m_g_mtx_normal[0] = im[0], m_g_mtx_normal[1] = im[1], m_g_mtx_normal[2] = im[2];
  m_g_mtx_normal[3] = im[4], m_g_mtx_normal[4] = im[5], m_g_mtx_normal[5] = im[6];
  m_g_mtx_normal[6] = im[8], m_g_mtx_normal[7] = im[9], m_g_mtx_normal[8] = im[10];
}

void kgmGraphics::setWorldMatrix(mtx4 &m)
{
  m_g_mtx_world = m;
}

mtx4 kgmGraphics::getProjMatrix()
{
  return m_g_mtx_proj;
}

mtx4 kgmGraphics::getViewMatrix()
{
  return m_g_mtx_view;
}

mtx4 kgmGraphics::getWorldMatrix()
{
  return m_g_mtx_world;
}

void kgmGraphics::setEditor(bool e)
{
  m_editor = e;
}

void kgmGraphics::resize(float width, float height)
{
  gc->gcSetViewport(0, 0, width, height, .1, 1000.0);

  m_camera->set(PI / 6, width / height, .1, 1000.0,
               m_camera->mPos, m_camera->mDir, m_camera->mUp);

  m_camera->viewport((float)width, (float)height);

  g_mtx_orto.ortho(0, width, height, 0, 1, -1);
  m_viewport = iRect(0, 0, width, height);
}

void kgmGraphics::add(INode* nod)
{
  if (!nod)
    return;

  kgmMaterial* mtl = null;

  switch(nod->getNodeType())
  {
  case  NodeMesh:
    m_meshes.add(nod);
    mtl = nod->getNodeMaterial();

    if (mtl && mtl->envType() && mtl->envType() != kgmMaterial::EnvironmentTypeImage)
    {
      gchandle map = null;

      if (mtl->envMapping() == kgmMaterial::EnvironmentMappingPlane)
        map = gc->gcGenTexture(null, 256, 256, 3, gctype_tex2d);
      else if (mtl->envMapping() == kgmMaterial::EnvironmentMappingCube)
        map = gc->gcGenTexture(null, 512, 512, 3, gctype_texcube);

      if (map)
        m_environments.set(nod, map);
    }
    break;
  case NodeLight:
    m_lights.add(nod);
    break;
  case NodeParticles:
    m_particles.add(nod);

    if (nod->getNodeObject())
    {
      ((kgmParticles*)nod->getNodeObject())->camera(m_camera);
    }
    break;
  }
}

void kgmGraphics::render()
{
  mtx4 m;

  bool lighting = false;

  m_a_meshes_count    = 0;
  m_a_bmeshes_count   = 0;
  m_a_particles_count = 0;

  for(kgmList<INode*>::iterator i = m_meshes.begin(); !i.end(); i.next())
  {
    if (!(*i)->isNodeValid())
      continue;

    box3 bound = (*i)->getNodeBound();

    vec3  l = bound.max - bound.min;
    vec3  v = (bound.min + bound.max) * 0.5;

    if(!m_camera->isSphereCross(v, 0.5 * l.length()))
      continue;

    kgmMaterial* m = (*i)->getNodeMaterial();

    if(m && (m->blend() != kgmMaterial::Blend_None || m->transparency() > 0.0f))
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

  setProjMatrix(m_camera->mProj);
  setViewMatrix(m_camera->mView);
  m_g_mtx_world.identity();

  gc->gcBegin();
  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, m_bg_color, 1, 0);


  gc->gcBlend(false, 0, null, null);
  gc->gcAlpha(false, null, null);

  //colect lights in viewport

  m_a_light_count = 0;

  for(kgmList<INode*>::iterator i = m_lights.begin(); !i.end(); i.next())
  {
    if(!(*i)->isNodeValid())
      continue;

    vec3 pos = (*i)->getNodePosition();

    kgmLight* l = (kgmLight*) (*i)->getNodeObject();

    if(!m_camera->isSphereCross(pos, kgmLight::LIGHT_RANGE * l->intensity()))
      continue;

    m_a_lights[m_a_light_count++] = (*i);

    if(m_a_light_count >= MAX_LIGHTS)
      break;
  }

  if (m_a_light_count == 0)
  {
    m_a_lights[0] = m_def_light;

    m_a_light_count = 1;
  }

  m_a_light = m_a_lights[0];

  m_a_particles_count = 0;

  for(kgmList<INode*>::iterator i = m_particles.begin(); !i.end(); i.next())
  {
    if(!(*i)->isNodeValid())
      continue;

    if (m_a_particles_count == m_a_particles.length())
      m_a_particles.realloc(m_a_particles_count + 1024);

    m_a_particles[m_a_particles_count] = (*i);
    m_a_particles_count++;
  }

  //draw scene only lights
  render((kgmMaterial*)null);

  lighting = true;

  m_rnd_lights->render();
  //m_rnd_lights->lightmap();

  //ShadowRender sr(this);
  //sr.render();
  //m_rnd_shadows->render();

  m_rnd_environment->render();

  //draw particles
  //ParticlesRender pr(this);
  //pr.render();

  // Draw alpha objects.

  gc->gcDepth(true, true, gccmp_lequal);

  gc->gcSetShader(null);

  if(lighting)
  {
    lighting = false;
  }

  render_3d();

  gc->gcCull(gccull_back);

  render((kgmShader*)null);

  render(m_def_material);

  m_r_sprite->render();
  m_r_gui->render();

  gc->gcSetShader(null);
  gc->gcDepth(false, false, gccmp_lequal);

  gc2DMode();

  m.identity();
  setWorldMatrix(m);

  render(m_shaders[kgmMaterial::TypeBase]);

  //gcDrawRect(kgmGui::Rect(1, 100, 256, 256), 0xffffffff, g_tex);
  //gcDrawRect(kgmGui::Rect(1, 250, 256, 256), 0xffffffff, m_rnd_environment->m_tx_refraction);
  gcDrawRect(kgmGui::Rect(1, 250, 300, 300), 0xffffffff, (gchandle) font->texture());

  {
    kgmGameApp* gapp = (kgmGameApp*) kgmApp::application();

    if (((kgmGameBase*)gapp->game())->getKeyState(KEY_Z))
      gcDrawRect(m_viewport, 0xffffffff, m_rnd_environment->m_tx_cube);
  }
  render_2d();

#ifdef DEBUG
  m_r_fps->render();
#endif

  render((kgmShader*)null);
  render((kgmMaterial*)null);

  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcEnd();
  gc->gcRender();

  // clear&reset

  gc->gcSetTexture(0, 0);
  gc->gcSetTexture(1, 0);
  gc->gcSetTexture(2, 0);
  gc->gcSetTexture(3, 0);
}

void kgmGraphics::render(kgmCamera &cam, kgmGraphics::Options &op)
{
  //prepare for render
  gc->gcSetViewport(0, 0, op.width, op.height, cam.mNear, cam.mFar);

  gc->gcCull(gccull_back);
  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, m_bg_color, 1, 0);

  gc->gcBlend(false, 0, null, null);
  gc->gcAlpha(false, null, null);

  for(kgmList<INode*>::iterator i = m_meshes.begin(); !i.end(); i.next())
  {
    kgmIGraphics::INode* nod = (*i);

    if (!nod || !(*i)->isNodeValid() || nod == op.discard)
      continue;

    box3 bound = (*i)->getNodeBound();

    vec3  l = bound.max - bound.min;
    vec3  v = (bound.min + bound.max) * 0.5;

    //if(!cam.isSphereCross(v, 0.5 * l.length()))
    //  continue;

    kgmMaterial*         mtl = nod->getNodeMaterial();
    kgmMesh*             msh = (kgmMesh*) nod->getNodeObject();

    if (!mtl)
      mtl = m_def_material;

    if(mtl && (mtl->blend() != kgmMaterial::Blend_None || mtl->transparency() > 0.0f))
    {
      continue;
    }

    kgmNodeLight* light = null;

    mtx4 transform = nod->getNodeTransform();

    if (op.light)
    {
      m_rnd_lights->render(&cam, nod);
    }
    else
    {
      if (op.clipping)
        m_rnd_base->setClipPlane(true, 0, vec4(op.plane[0], op.plane[1], op.plane[2], op.plane[3]));

      m_rnd_base->render(&cam, nod);
    }
  }

  gc->gcSetViewport(0, 0, m_viewport.width(), m_viewport.height(), m_camera->mNear, m_camera->mFar);
}

/*
void kgmGraphics::render(gchandle buf, kgmCamera &cam, kgmGraphics::Options &op)
{
  //prepare for render
  gc->gcSetTarget(buf);
  gc->gcSetViewport(0, 0, op.width, op.height, cam.mNear, cam.mFar);

  gc->gcCull(gccull_back);
  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, m_bg_color, 1, 0);

  gc->gcBlend(false, 0, null, null);
  gc->gcAlpha(false, null, null);

  if (op.clipping)
  {
    kgmIGC::ClipPlane cp = {true, 0, { op.plane[0], op.plane[1], op.plane[2], op.plane[3]}};
    gc->gcSet(gcpar_clipplane, &cp);
  }

  gc->gcBegin();

  for(kgmList<INode*>::iterator i = m_meshes.begin(); !i.end(); i.next())
  {
    kgmIGraphics::INode* nod = (*i);

    if (!nod || !(*i)->isNodeValid() || nod == op.discard)
      continue;

    box3 bound = (*i)->getNodeBound();

    vec3  l = bound.max - bound.min;
    vec3  v = (bound.min + bound.max) * 0.5;

    //if(!cam.isSphereCross(v, 0.5 * l.length()))
    //  continue;

    kgmMaterial*         mtl = nod->getNodeMaterial();
    kgmMesh*             msh = (kgmMesh*) nod->getNodeObject();

    if (!mtl)
      mtl = m_def_material;

    if(mtl && (mtl->blend() != kgmMaterial::Blend_None || mtl->transparency() > 0.0f))
    {
      continue;
    }

    kgmNodeLight* light = null;

    for(kgmList<INode*>::iterator i = m_lights.begin(); !i.end(); i.next())
    {
      if(!(*i)->isNodeValid())
        continue;

      vec3 pos = (*i)->getNodePosition();

      kgmLight* l = (kgmLight*) (*i)->getNodeObject();

      if(!cam.isSphereCross(pos, kgmLight::LIGHT_RANGE * l->intensity()))
        continue;

      if (!light)
      {
        light = (kgmNodeLight*) (*i);
      }
      else
      {
        f32 lforce[2];
        lforce[0] = kgmLight::LIGHT_RANGE * l->intensity();
        lforce[1] = kgmLight::LIGHT_RANGE * ((kgmLight*)light->getNodeObject())->intensity();

        if (lforce[0] > lforce[1])
          light = (kgmNodeLight*) (*i);
      }
    }

    if (!light)
      light = (kgmNodeLight*) m_def_light;

    mtx4 transform = nod->getNodeTransform();

    if (!op.color)
      render(m_def_material);
    else
      render(mtl);

    kgmShader* shader = null;

    if (!op.light)
      shader = m_shaders[kgmMaterial::TypeBase];
    else
      shader = m_shaders[kgmMaterial::TypePhong];

    kgmShader s = (*shader);

    float shine   = mtl->shininess();
    float ambient = g_fAmbient;

    vec4 v_light;
    vec4 v_light_color;
    vec4 v_light_direction;

    if (light)
    {
      kgmLight* l   = (kgmLight*) m_a_light->getNodeObject();
      vec3      pos = m_a_light->getNodePosition();

      v_light           = vec4(pos.x, pos.y, pos.z, l->intensity());
      v_light_direction = vec4(l->direction().x, l->direction().y, l->direction().z, l->angle());
      v_light_color     = kgmColor::toVector(l->color());
    }

    s.setValue("g_fShine",    kgmShader::Val_Float, &shine, 1);
    s.setValue("g_fAmbient",  kgmShader::Val_Float, &ambient, 1);
    s.setValue("g_mProj",     kgmShader::Val_Mtx4,  cam.mProj.m, 1);
    s.setValue("g_mView",     kgmShader::Val_Mtx4,  cam.mView.m, 1);
    s.setValue("g_mTran",     kgmShader::Val_Mtx4,  transform.m, 1);
    s.setValue("g_vColor",    kgmShader::Val_Vec4,  &g_vec_color, 1);
    s.setValue("g_vSpecular", kgmShader::Val_Vec4,  &g_vec_specular, 1);
    s.setValue("g_vLight",          kgmShader::Val_Vec4, &v_light, 1);
    s.setValue("g_vLightColor",     kgmShader::Val_Vec4, &v_light_color, 1);
    s.setValue("g_vLightDirection", kgmShader::Val_Vec4, &v_light_direction, 1);
    s.setValue("g_vEye",            kgmShader::Val_Vec3, &m_camera->mPos, 1);
    s.setValue("g_vLook",           kgmShader::Val_Vec3, &m_camera->mDir, 1);

    if (op.clipping)
    {
      s32 pc  = 1;
      vec4 pv = vec4(op.plane[0], op.plane[1], op.plane[2], op.plane[3]);
      s.setValue("g_iClipping",  kgmShader::Val_Int, &pc, 1);
      s.setValue("g_vClipPlane", kgmShader::Val_Vec4, &pv, 1);
    }

    s.start();
    s.useValues();

    render(msh);

    s.stop();
  }

  render((kgmShader*)null);
  render((kgmMaterial*)null);

  gc->gcEnd();

  if (op.clipping)
  {
    kgmIGC::ClipPlane cp = {false, 0, { op.plane[0], op.plane[1], op.plane[2], op.plane[3]}};
    gc->gcSet(gcpar_clipplane, &cp);
  }

  gc->gcSetTarget(null);
}
*/
void kgmGraphics::render(kgmVisual* visual)
{
  if(!visual)
    return;

  if(visual->m_tm_joints)
  {
    g_mtx_joints       = visual->m_tm_joints;
    g_mtx_joints_count = visual->m_skeleton->m_joints.size();
  }

  switch(visual->type())
  {
  case kgmVisual::TypeNone:
  case kgmVisual::TypeMesh:
  case kgmVisual::TypeShape:
  {
    kgmMesh* msh = visual->getMesh();

    u32  pmt;

    if(!msh) {
      msh = (kgmMesh*) visual->getShape();

      if(!msh)
        return;
    }

    switch(msh->rtype())
    {
    case kgmMesh::RT_LINE:
      pmt = gcpmt_lines;
      break;
    case kgmMesh::RT_POINT:
      pmt = gcpmt_points;
      break;
    case kgmMesh::RT_TRIANGLESTRIP:
      pmt = gcpmt_trianglestrip;
      break;
    default:
      pmt = gcpmt_triangles;
    };

    gc->gcDraw(pmt, msh->fvf(),
               msh->vsize(), msh->vcount(), msh->vertices(),
               2, 3 * msh->fcount(), msh->faces());

    g_mtx_joints       = null;
    g_mtx_joints_count = 0;
  }
    break;
  case kgmVisual::TypeText:
  {
    kgmText* text = visual->getText();
    kgmGui::Rect rc(text->m_rect.x, text->m_rect.y,
                    text->m_rect.w, text->m_rect.h);
    gcDrawText(font, text->m_size / 2, text->m_size, text->m_color, rc, text->m_text);
  }
    break;
  case kgmVisual::TypeParticles:
    setWorldMatrix(g_mtx_iden);
    render(g_shd_active);
    render(visual->getParticles());
    setWorldMatrix(visual->getTransform());
    break;
  }
}

void kgmGraphics::render(kgmParticles* particles)
{
  if(!particles || !particles->getMesh())
    return;

  render(particles->getMesh());
}

void kgmGraphics::render(kgmIcon* icon)
{
  mtx4    mtr = m_g_mtx_view;
  vec3    rv, uv;

  rv = vec3(mtr.m[0], mtr.m[2], mtr.m[1]);
  rv.normalize();
  uv = rv.cross(m_camera->mDir);
  uv.normalize();

  vec3 pos   = icon->getPosition();
  vec3 crv = rv * icon->getWidth(),
       cuv = uv * icon->getHeight();

  kgmMesh::Vertex_P_C_T points[6];

  points[0].pos = (pos - crv + cuv);
  points[0].uv = vec2(0, 1);
  points[1].pos = (pos - crv - cuv);
  points[1].uv = vec2(0, 0);
  points[2].pos = (pos + crv + cuv);
  points[2].uv = vec2(1, 1);

  points[3].pos = (pos + crv - cuv);
  points[3].uv = vec2(1, 0);
  points[4].pos = (pos + crv + cuv);
  points[4].uv = vec2(1, 1);
  points[5].pos = (pos - crv - cuv);
  points[5].uv = vec2(0, 0);

  points[0].col = points[1].col =
  points[2].col = points[3].col =
  points[4].col = points[5].col = 0xffffffff;

  gc->gcDraw(gcpmt_triangles, gcv_xyz|gcv_col|gcv_uv0, sizeof(kgmMesh::Vertex_P_C_T), 6, points, 0, 0, 0);
}

void kgmGraphics::render(kgmMaterial* m)
{
  if(!m)
  {
    gc->gcSetTexture(0, 0);
    gc->gcSetTexture(1, 0);
    gc->gcSetTexture(2, 0);
    gc->gcSetTexture(3, 0);

    if(!m_culling)
    {
      gc->gcCull(1);
      m_culling = true;
    }

    if(m_alpha)
    {
      gc->gcBlend(false, 0, 0, 0);
      m_alpha = false;
    }

    if(!m_depth)
    {
      gc->gcDepth(true, true, gccmp_lequal);
      m_depth = true;
    }

    tcolor = tnormal = tspecular = null;

    g_vec_color = vec4(1, 1, 1, 1);
    g_fShine    = 0.0f;
    m_alpha     = false;
    m_depth     = true;
    m_culling   = true;

    return;
  }

  g_vec_color = m->m_color.get();

  g_fShine = m->shininess();

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

    m_alpha = true;
  }
  else if(m->transparency() > 0.0f)
  {
    gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
    m_alpha = true;
  }

  if(!m->depth())
  {
    gc->gcDepth(false, true, gccmp_less);
    m_depth = false;
  }


  if(!m->cull())
  {
    gc->gcCull(0);
    m_culling = false;
  }

  if(m->hasTexColor())
  {
    gc->gcSetTexture(0, m->getTexColor()->texture());
    tcolor = m->getTexColor()->texture();
  }
  else
  {
    gc->gcSetTexture(0, g_tex_white);
    tcolor = g_tex_white;
  }

  if(m->hasTexNormal())
  {
    gc->gcSetTexture(1, m->getTexNormal()->texture());
    tnormal = m->getTexNormal()->texture();
  }
  else
  {
    gc->gcSetTexture(1, g_tex_gray);
    tnormal = g_tex_gray;
  }

  if(m->hasTexSpecular())
  {
    gc->gcSetTexture(2, m->getTexSpecular()->texture());
    tspecular = m->getTexSpecular()->texture();
  }
  else
  {
    gc->gcSetTexture(2, g_tex_white);
    tspecular = g_tex_white;
  }

  if (m->envType() != kgmMaterial::EnvironmentTypeNone)
  {

  }
}

void kgmGraphics::render(kgmShader* s)
{
  if(s == null)
  {
    gc->gcSetShader(null);
    g_shd_active = null;

    return;
  }

  //send default parameters
  vec4 v_light(0, 0, 0, 10);
  vec4 v_light_color(1, 1, 1, 1);
  vec4 v_light_direction(0, 0, 1, 0);

  if(m_a_light)
  {
    kgmLight* l   = (kgmLight*) m_a_light->getNodeObject();
    vec3      pos = m_a_light->getNodePosition();

    v_light           = vec4(pos.x, pos.y, pos.z, l->intensity());
    v_light_direction = vec4(l->direction().x, l->direction().y, l->direction().z, l->angle());
    //v_light_color     = vec4(1, 1, 1, 1); //l->color;
    v_light_color     = vec4(l->color().x, l->color().y, l->color().z, 1.0);
  }

  float random = (float)rand()/(float)RAND_MAX;

  s->start();
  s->set("g_fTime",           kgmTime::getTime());
  s->set("g_fRandom",         random);
  s->set("g_fShine",          g_fShine);
  s->set("g_fAmbient",        g_fAmbient);
  s->set("g_mProj",           m_g_mtx_proj);
  s->set("g_mView",           m_g_mtx_view);
  s->set("g_mTran",           m_g_mtx_world);
  s->set("g_mNorm",           m_g_mtx_normal);
  s->set("g_vColor",          g_vec_color);
  s->set("g_vSpecular",       g_vec_specular);
  s->set("g_vLight",          v_light);
  s->set("g_vLightColor",     v_light_color);
  s->set("g_vLightDirection", v_light_direction);
  s->set("g_vEye",            m_camera->mPos);
  s->set("g_vLook",           m_camera->mDir);
  s->set("g_iClipping",       0);

  if(tcolor)
    s->set("g_txColor", 0);

  if(tnormal)
    s->set("g_txNormal", 1);

  if(tspecular)
    s->set("g_txSpecular", 2);

  if(g_mtx_joints)
  {
    s->set("g_mJoints", g_mtx_joints[0], g_mtx_joints_count);
  }

  /*if(m_a_light_count > 1)
  {
    static vec4 lights[MAX_LIGHTS - 1];

    for(u32 i = 0; i < (m_a_light_count - 1); i++)
    {
      kgmLight* l = (kgmLight*)
      lights[i] = vec4(g_lights[i + 1]->position.x, g_lights[i + 1]->position.y,
          g_lights[i + 1]->position.z, g_lights[i + 1]->intensity);

    }

    s->set("g_vLights", lights[0], m_a_light_count - 1);
    s->set("g_iLights", m_a_light_count - 1);
  }*/

  g_shd_active = s;
}

void kgmGraphics::render(kgmMesh *m)
{
  if(!m || !m->vcount() || !m->vertices())
    return;

  kgm_log() << "render mesh id " << m->id() << "\n";

  u32  pmt;

  switch(m->m_rtype)
  {
  case kgmMesh::RT_LINE:
    pmt = gcpmt_lines;
    break;
  case kgmMesh::RT_POINT:
    pmt = gcpmt_points;
    break;
  case kgmMesh::RT_TRIANGLESTRIP:
    gcpmt_trianglestrip;
    break;
  default:
    pmt = gcpmt_triangles;
  };

  if (m_wired)
    pmt = gcpmt_lines;

  gc->gcDraw(pmt, m->fvf(), m->vsize(),
             m->vcount(), m->vertices(),
             2, 3 * m->fcount(), m->faces());
}

s32 kgmGraphics::getShaderId(kgmString s)
{
  return -1;
}

u32 kgmGraphics::collectLights(kgmCamera* cam, kgmArray<INode*>& nodes, u32 max)
{
  const s32 max_lights = 64;
  const f32 min_lforce = 0.05;

  if (!cam || nodes.length() < max)
    return 0;

  if (max > max_lights)
    max = max_lights;

  u32 count = 0;

  for(kgmList<INode*>::iterator i = m_lights.begin(); !i.end(); i.next())
  {
    INode* lnod = (*i);

    if(!lnod->isNodeValid())
      continue;

    vec3 pos = lnod->getNodePosition();

    kgmLight* l = (kgmLight*) lnod->getNodeObject();

    f32 lforce = l->intensity() / (1.0 + pos.distance(cam->mPos));

    if (lforce > 1.0)
      lforce = 1.0;

    bool isdir = false;

    if (l->angle() < 0.01 && l->direction().length() > 0.9)
    {
      isdir = true;
      lforce = 1.0;
    }

    if(!isdir && (lforce < min_lforce || !m_camera->isSphereCross(pos, kgmLight::LIGHT_RANGE * l->intensity())))
      continue;

    if (count < max)
    {
      nodes[count++] = lnod;
    }
    else
    {
      s32 min = -1;
      f32 cforce = 1.0;

      for (s32 i = 0; i < max; i++)
      {
        kgmLight* l = (kgmLight*) nodes[i]->getNodeObject();
        f32 lforce = l->intensity() / (1.0 + pos.distance(cam->mPos));

        if (l->angle() < 0.01 && l->direction().length() > 0.9)
        {
          lforce = 1.0;
        }

        if (lforce < cforce)
        {
          cforce = lforce;

          min = i;
        }
      }

      if (lforce > cforce && min > -1)
        nodes[min] = lnod;
    }
  }

  return count;
}

u32 kgmGraphics::collectMeshes(kgmCamera *cam, kgmArray<INode *> &nodes, u32 max)
{
  const s32 max_meshes = 10000;
  const f32 min_mforce = 0.1;

  if (!cam || nodes.length() < max)
    return 0;

  if (max > max_meshes)
    max = max_meshes;

  u32 count = 0;

  for(kgmList<INode*>::iterator i = m_meshes.begin(); !i.end(); i.next())
  {
    INode* mnod = (*i);

    if (!mnod->isNodeValid())
      continue;

    box3 bound = mnod->getNodeBound();

    vec3  pos = mnod->getNodePosition();

    vec3  l = bound.max - bound.min;
    vec3  v = (bound.min + bound.max) * 0.5;

    f32 mforce = l.length() / (1.0 + pos.distance(cam->mPos));

    if(mforce < min_mforce || !m_camera->isSphereCross(v, 0.5 * l.length()))
      continue;


    if (count >= max)
      break;
    else
      nodes[count++] = mnod;
  }

  return count;
}

//*************** VIEW MODE *************
void kgmGraphics::gc2DMode()
{
  setProjMatrix(g_mtx_orto);
  setViewMatrix(g_mtx_iden);
}

void kgmGraphics::gc3DMode()
{
  setProjMatrix(m_camera->mProj);
  setViewMatrix(m_camera->mView);
}

//*************** DRAWING ***************
void kgmGraphics::gcDrawRect(kgmGui::Rect rc, u32 col, kgmTexture* tex)
{
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;

  V v[4];

  v[0].pos = vec3(rc.x,        rc.y,        0); v[0].col = col; v[0].uv = vec2(0, 1);
  v[1].pos = vec3(rc.x,        rc.y + rc.h, 0); v[1].col = col; v[1].uv = vec2(0, 0);
  v[2].pos = vec3(rc.x + rc.w, rc.y,        0); v[2].col = col; v[2].uv = vec2(1, 1);
  v[3].pos = vec3(rc.x + rc.w, rc.y + rc.h, 0); v[3].col = col; v[3].uv = vec2(1, 0);

  if(tex && tex->texture())
    gc->gcSetTexture(0, tex->texture());
  else
    gc->gcSetTexture(0, g_tex_white);

  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);

  if(tex)
    gc->gcSetTexture(0, 0);
}

void kgmGraphics::gcDrawRect(kgmGui::Rect rc, u32 col, void* tex)
{
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;

  V v[4];

  v[0].pos = vec3(rc.x,        rc.y,        0); v[0].col = col; v[0].uv = vec2(0, 1);
  v[1].pos = vec3(rc.x,        rc.y + rc.h, 0); v[1].col = col; v[1].uv = vec2(0, 0);
  v[2].pos = vec3(rc.x + rc.w, rc.y,        0); v[2].col = col; v[2].uv = vec2(1, 1);
  v[3].pos = vec3(rc.x + rc.w, rc.y + rc.h, 0); v[3].col = col; v[3].uv = vec2(1, 0);

  if(tex)
    gc->gcSetTexture(0, tex);
  else
    gc->gcSetTexture(0, g_tex_white);

  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);

  if(tex)
    gc->gcSetTexture(0, 0);
}

void kgmGraphics::gcDrawText(kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor, kgmGui::Rect clip, kgmString& text)
{
  typedef struct{ vec3 pos; u32 col; vec2 uv; } V;

  V v[4];

  u32 tlen = text.length();

  if(tlen < 1)
    return;

  if(!font || !font->texture())
    return;

  float tx = 0.0f, ty = 0.0f;
  float tdx = (float)(1.f / font->m_cols),
      tdy = (float)(1.f / font->m_rows);

  float cx = (float)clip.x, cy = (float)clip.y;

  //gc->gcBlend(true, gcblend_one, gcblend_one);
  gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
  gc->gcSetTexture(0, font->texture());

  for(u32 i = 0; i < tlen; i++)
  {
    char ch = text[i];

    if(ch == '\n')
    {
      cx = (float)clip.x;
      cy += fheight;
      
      continue;
    }

    if((ch == ' ') || (ch == '\t'))
    {
      tx = 0.0f;
      ty = 0.0f;
    }

    u32 rows = (font->m_rows != 0) ? (font->m_rows) : (1);

    tx = (float)(tdx * (ch % rows));
    ty = 1.0f - (float)(tdy * (ch / rows));

    if(clip.inside(cx + fwidth, cy + fheight))
    {
      v[0].pos = vec3(cx, cy, 0),                v[0].col = fcolor, v[0].uv = vec2(tx + 0.001,     ty - 0.001);
      v[1].pos = vec3(cx, cy+fheight, 0),        v[1].col = fcolor, v[1].uv = vec2(tx + 0.001,     ty - tdy);
      v[2].pos = vec3(cx+fwidth, cy, 0),         v[2].col = fcolor, v[2].uv = vec2(tx+tdx - 0.001, ty - 0.001);
      v[3].pos = vec3(cx+fwidth, cy+fheight, 0), v[3].col = fcolor, v[3].uv = vec2(tx+tdx - 0.001, ty - tdy);
      
      gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
    }

    cx += fwidth;
  }

  gc->gcBlend(0, 0, 0, 0);
  gc->gcSetTexture(0, 0);
}

void kgmGraphics::gcDrawText(kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor, kgmGui::Rect clip, s8* text)
{
  typedef struct{ vec3 pos; u32 col; vec2 uv; } V;

  V v[4];

  u32 tlen = strlen(text);

  if(tlen < 1)
    return;

  if(!font || !font->texture())
    return;

  float tx = 0.0f, ty = 0.0f;
  float tdx = (float)(1.f / font->m_cols),
      tdy = (float)(1.f / font->m_rows);

  float cx = (float)clip.x, cy = (float)clip.y;

  gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
  gc->gcSetTexture(0, font->texture());

  for(u32 i = 0; i < tlen; i++)
  {
    char ch = text[i];

    if(ch == '\n')
    {
      cx = (float)clip.x;
      cy += fheight;

      continue;
    }

    if((ch == ' ') || (ch == '\t'))
    {
      tx = 0.0f;
      ty = 0.0f;
    }

    u32 rows = (font->m_rows != 0) ? (font->m_rows) : (1);

    tx = (float)(tdx * (ch % rows));
    ty = 1.0f - (float)(tdy * (ch / rows));

    if(clip.inside(cx + fwidth, cy + fheight))
    {
      v[0].pos = vec3(cx, cy, 0),                v[0].col = fcolor, v[0].uv = vec2(tx + 0.001,     ty - 0.001);
      v[1].pos = vec3(cx, cy+fheight, 0),        v[1].col = fcolor, v[1].uv = vec2(tx + 0.001,     ty - tdy);
      v[2].pos = vec3(cx+fwidth, cy, 0),         v[2].col = fcolor, v[2].uv = vec2(tx+tdx - 0.001, ty - 0.001);
      v[3].pos = vec3(cx+fwidth, cy+fheight, 0), v[3].col = fcolor, v[3].uv = vec2(tx+tdx - 0.001, ty - tdy);

      gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
    }

    cx += fwidth;
  }

  gc->gcBlend(0, 0, 0, 0);
  gc->gcSetTexture(0, 0);
}

void kgmGraphics::gcDrawBillboard(box b, u32 col)
{
  mtx4 mv, mp, m;
  vec3 rv, uv;

  typedef struct{ vec3 pos; u32 col; vec2 uv; } V;

  V v[4];
  vec3 pos(0, 0, 10);
  float2 dim(10, 10);

  gc->gcGetMatrix(gcmtx_view, mv.m);
  gc->gcGetMatrix(gcmtx_proj, mp.m);

  m = mv * mp;

  rv = vec3(m.m[0], m.m[4], m.m[8]); rv.normalize();
  rv.x *= dim.x * 0.5f;
  rv.y *= dim.x * 0.5f;
  rv.z *= dim.x * 0.5f;
  uv = vec3(m.m[1], m.m[5], m.m[9]); uv.normalize();
  uv.x *= dim.y * 0.5f;
  uv.y *= dim.y * 0.5f;
  uv.z *= dim.y * 0.5f;

  v[0].pos = pos - rv + uv;
  v[0].col = 0xff0000ff;
  v[0].uv.x = 0.0f, v[0].uv.y = 0.0f;
  v[1].pos = pos - rv - uv;
  v[1].col = 0x00ff00ff;
  v[1].uv.x = 0.0f, v[1].uv.y = 1.0f;
  v[2].pos = pos + rv + uv;
  v[2].col = 0x0000ffff;
  v[2].uv.x = 1.0f, v[2].uv.y = 0.0f;
  v[3].pos = pos + rv - uv;
  v[3].col = 0xffffffff;
  v[3].uv.x = 1.0f, v[3].uv.y = 1.0f;

  m.identity();
  setViewMatrix(m);
  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
}
