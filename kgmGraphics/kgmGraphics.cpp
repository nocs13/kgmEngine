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
#include "kgmNodeLight.h"

#include "render/LightRender.h"
#include "render/ColorRender.h"
#include "render/ParticlesRender.h"

#include <stdlib.h>

#define MAX_LIGHTS 48

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
mtx4       g_mtx_proj;
mtx4       g_mtx_view;
mtx4*      g_mtx_joints = null;

mtx3       g_mtx_normal;

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

//void* g_fbo = null;

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
  m_def_material->setShader(null);
  m_def_material->shade(false);
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

    m_def_light = new kgmNodeLight();

    //g_fbo = g->gcGenTarget(512, 512, 0, true);
    m_shadows.alloc(1);
    m_shadows[0].valid = false;
    m_shadows[0].w = m_shadows[0].h = 512;
    m_shadows[0].fbo = g->gcGenTarget(512, 512, gctype_texdepth, true);
  }


#ifdef DEBUG
  kgm_log() << "Prepare shaders \n";
#endif

  if(rc != null)
  {
    memset(m_shaders, 0, sizeof(m_shaders));
    m_shaders[kgmShader::TypeNone]  = rc->getShader("none.glsl");
    m_shaders[kgmShader::TypeBase]  = rc->getShader("base.glsl");
    m_shaders[kgmShader::TypeToon]  = rc->getShader("toon.glsl");
    m_shaders[kgmShader::TypePhong] = rc->getShader("phong.glsl");
    m_shaders[ShaderShadowKeep]     = rc->getShader("shkeep.glsl");
    m_shaders[ShaderShadowDraw]     = rc->getShader("shdraw.glsl");
  }

  m_camera = new kgmCamera();
  m_camera->set(PI / 6, 1, 1, 1000, vec3(0, 0, 1), vec3(-1, 0, 0), vec3(0, 0, 1));
  g_mtx_iden.identity();

  m_r_fps    = new FpsRender(this);
  m_r_gui    = new GuiRender(this);
  m_r_sprite = new SpriteRender(this);

  m_a_lights.alloc(MAX_LIGHTS);
}

kgmGraphics::~kgmGraphics()
{
  //gc->gcFreeTarget(g_fbo);

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
  g_mtx_proj = m;
}

void kgmGraphics::setViewMatrix(mtx4 &m)
{
  g_mtx_view = m;

  mtx4 im = m;

  im.invert();
  g_mtx_normal[0] = im[0], g_mtx_normal[1] = im[1], g_mtx_normal[2] = im[2];
  g_mtx_normal[3] = im[4], g_mtx_normal[4] = im[5], g_mtx_normal[5] = im[6];
  g_mtx_normal[6] = im[8], g_mtx_normal[7] = im[9], g_mtx_normal[8] = im[10];
}

void kgmGraphics::setWorldMatrix(mtx4 &m)
{
  m_g_mtx_world = m;
}

mtx4 kgmGraphics::getProjMatrix()
{
  return g_mtx_proj;
}

mtx4 kgmGraphics::getViewMatrix()
{
  return g_mtx_view;
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

  /*{
    static u32 zzz = 0;

    if (zzz == 0)
    {
      gc->gcSetTarget(g_fbo);
      gc->gcSetViewport(0, 0, 512, 512, 1.0, 100.0);
      zzz++;
    }
    else
    {
      if (zzz == 1)
      {
        gc->gcSetTarget(null);
        gc->gcSetViewport(0, 0, m_viewport.width(), m_viewport.height(), 1.0, 100.0);
      }

      zzz++;

      if (zzz == 100)
        zzz = 0;
    }
  }*/

  gc->gcCull(1);

  setProjMatrix(m_camera->mProj);
  setViewMatrix(m_camera->mView);
  m_g_mtx_world.identity();

  gc->gcSetTarget(null);
  gc->gcBegin();
  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, m_bg_color, 1, 0);

  //prepare for render

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

  m_shadows[0].valid = true;

  m_shadows[0].lpos  = m_a_light->getNodePosition();
  m_shadows[0].ldir  = vec3(.1, .1, -1);
  m_shadows[0].ldir.normalize();
  //m_shadows[0].lpos  = m_camera->mPos;
  //m_shadows[0].ldir  = m_camera->mDir;

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
    m_shadows[0].mvp = /*mb */ mv * mp;
  }

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

  LightRender lr(this);
  //lr.render();

  ShadowRender sr(this);
  sr.render();

  //draw particles
  ParticlesRender pr(this);
  pr.render();

  // Draw alpha objects.

  gc->gcDepth(true, true, gccmp_lequal);

  gc->gcSetShader(null);

  if(lighting)
  {
    lighting = false;
  }

  render_3d();

  gc->gcCull(gccull_back);

#ifndef NO_SHADERS

  render((kgmShader*)null);

#endif

  render(m_def_material);

  m_r_sprite->render();
  m_r_gui->render();

  gc->gcSetShader(null);
  gc->gcDepth(false, 0, 0);

  gc2DMode();

  m.identity();
  setWorldMatrix(m);

  render(m_shaders[kgmShader::TypeBase]);

  gcDrawRect(kgmGui::Rect(1, 100, 256, 256), 0xffffffff, gc->gcTexTarget(m_shadows[0].fbo));

  render_2d();

#ifdef DEBUG
  m_r_fps->render();
#endif

  render((kgmShader*)null);
  render((kgmMaterial*)null);

  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcSetTarget(null);
  gc->gcEnd();
  gc->gcRender();

  // clear&reset

  gc->gcSetTexture(0, 0);
  gc->gcSetTexture(1, 0);
  gc->gcSetTexture(2, 0);
  gc->gcSetTexture(3, 0);
}

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
      msh = visual->getShape();

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
  mtx4    mtr = g_mtx_view;
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
    v_light_color     = kgmColor::toVector(l->color());
  }

  float random = (float)rand()/(float)RAND_MAX;

  s->start();
  s->set("g_fTime",           kgmTime::getTime());
  s->set("g_fRandom",         random);
  s->set("g_fShine",          g_fShine);
  s->set("g_fAmbient",        g_fAmbient);
  s->set("g_mProj",           g_mtx_proj);
  s->set("g_mView",           g_mtx_view);
  s->set("g_mTran",           m_g_mtx_world);
  s->set("g_mNorm",           g_mtx_normal);
  s->set("g_vColor",          g_vec_color);
  s->set("g_vSpecular",       g_vec_specular);
  s->set("g_vLight",          v_light);
  s->set("g_vLightColor",     v_light_color);
  s->set("g_vLightDirection", v_light_direction);
  s->set("g_vEye",            m_camera->mPos);
  s->set("g_vEyeDir",         m_camera->mDir);

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
  if(!m)
    return;

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
