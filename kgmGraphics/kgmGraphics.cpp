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
#include "../kgmBase/kgmLog.h"
#include "../kgmMath/kgmBase.h"
#include "../kgmBase/kgmTime.h"

#include "render/LightRender.h"

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

mtx4       g_mtx_orto;
mtx4       g_mtx_iden;
mtx4       g_mtx_proj;
mtx4       g_mtx_view;
mtx4       g_mtx_world;
mtx4*      g_mtx_joints = null;

mtx3       g_mtx_normal;

u32        g_mtx_joints_count = 0;

//vec4       g_vec_light   = vec4(0, 0, 0, 1);
//vec4       g_vec_ambient = vec4(1, 1, 1, 1);

vec4       g_vec_color    = vec4(1, 1, 1, 1);
vec4       g_vec_specular = vec4(1, 1, 1, 1);

kgmShader* g_shd_active = null;

kgmLight*  g_light_active = null;
kgmLight*  g_lights[MAX_LIGHTS] = {null};
u32        g_lights_count = 0;
f32        g_fShine = 0.0f;
f32        g_fAmbient = 0.1f;
void*      g_tex_black = null;
void*      g_tex_white = null;
void*      g_tex_gray  = null;
void*      g_map_shadow = null;

kgmLight*     g_def_light = null;
kgmMaterial*  g_def_material = null;

kgmTexture*   g_def_style_texture = null;

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

  m_shadowmap = null;

  m_max_faces = 5000000;

  m_alpha       = false;
  m_depth       = true;
  m_culling     = true;

  m_bg_color    = 0xFF000000;

  m_editor      = false;

  gui_style = new kgmGuiStyle();

  m_visible_sprites.alloc(4096);
  m_visible_visuals.alloc(4096);
  m_visible_visuals_alpha.alloc(4096);

  g_def_material = new kgmMaterial();
  g_def_material->setShader(null);
  g_def_material->shade(false);
  g_def_material->m_color = kgmMaterial::Color(1.0f, 1.0f, 1.0f, 1.0f);

  if(g)
  {
    char txd[16] = {0};

    if(!gc)
      return;

    //generic black texture
    memset(txd, 0x00, sizeof(txd));
    g_tex_black = gc->gcGenTexture(txd, 2, 2, gctex_fmt32, gctype_tex2d);

    //generic white texture
    memset(txd, 0xff, sizeof(txd));
    g_tex_white = gc->gcGenTexture(txd, 2, 2, gctex_fmt32, gctype_tex2d);

    //generic gray texture
    memset(txd, 0x80, sizeof(txd));
    g_tex_gray = gc->gcGenTexture(txd, 2, 2, gctex_fmt32, gctype_tex2d);

    g_def_style_texture = new kgmTexture(g_tex_white);
  }

  m_shadows.alloc(1);

#ifdef DEBUG
  kgm_log() << "Prepare shaders \n";
#endif

  if(rc != null)
  {
    shaders.set(kgmShader::TypeNone,   rc->getShader("none.glsl"));
    shaders.set(kgmShader::TypeBase,   rc->getShader("base.glsl"));
    shaders.set(kgmShader::TypeLight,  rc->getShader("light.glsl"));
    shaders.set(kgmShader_TypeGui,     rc->getShader("gui.glsl"));
    shaders.set(kgmShader::TypeAmbient, rc->getShader("ambient.glsl"));
  }

  m_camera = new kgmCamera();
  m_camera->set(PI / 6, 1, 1, 1000, vec3(0, 0, 1), vec3(-1, 0, 0), vec3(0, 0, 1));
  g_mtx_iden.identity();

  m_r_fps = new FpsRender(this);
  m_r_gui = new GuiRender(this);
  m_r_sprite = new SpriteRender(this);

  gui_style->gui_image = g_def_style_texture;
  gui_style->sgui.image = g_def_style_texture;
  gui_style->sbutton.image = g_def_style_texture;
  gui_style->smenu.image = g_def_style_texture;
  gui_style->scheck.image = g_def_style_texture;
  gui_style->slist.image = g_def_style_texture;
  gui_style->slabel.image = g_def_style_texture;
  gui_style->sprogess.image = g_def_style_texture;
}

kgmGraphics::~kgmGraphics()
{
  delete m_r_fps;
  delete m_r_gui;
  delete m_r_sprite;

  m_visuals.clear();

  m_lights.clear();

  m_icons.clear();

  m_guis.clear();

  for(kgmTab<u16, kgmShader*>::iterator i = shaders.begin(); i != shaders.end(); ++i)
    if(i.data())
      rc->remove(i.data());

  shaders.clear();

#ifdef DEBUG
  m_bodies.clear();
  m_obstacles.clear();
#endif

  if(g_tex_black)
    gc->gcFreeTexture(g_tex_black);

  if(g_tex_white)
    gc->gcFreeTexture(g_tex_white);

  if(g_tex_gray)
    gc->gcFreeTexture(g_tex_gray);

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

void kgmGraphics::clean()
{
  for (kgmList<kgmResource*>::iterator i = m_resources.begin(); i != m_resources.end(); ++i)
  {
    (*i)->release();
  }

  m_resources.clear();
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
  for (kgmList<kgmResource*>::iterator i = m_resources.begin(); i != m_resources.end(); ++i)
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

void kgmGraphics::setGuiStyle(kgmGuiStyle* s)
{
  if(!s)
    return;

  if(gui_style)
  {
    delete gui_style;
  }

  gui_style = s;
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
  g_mtx_world = m;
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
  return g_mtx_world;
}

void kgmGraphics::setEditor(bool e)
{
  m_editor = e;
}

void kgmGraphics::resize(float width, float height)
{
  gc->gcSetViewport(0, 0, width, height, 1.0, 100000.0);

  m_camera->set(PI / 6, width / height, .1f, 100000.0,
               m_camera->mPos, m_camera->mDir, m_camera->mUp);

  m_camera->viewport((float)width, (float)height);

  g_mtx_orto.ortho(0, width, height, 0, 1, -1);
  m_viewport = iRect(0, 0, width, height);
}

void kgmGraphics::render()
{
  mtx4 m;
  int k = 0;
  bool lighting = false;

  s32 count_sprites = 0, count_visible = 0, count_visible_alpha = 0;

  kgmList<kgmLight*> lights;

  for(kgmList<kgmVisual*>::iterator i = m_visuals.begin(); i != m_visuals.end(); i.next())
  {
    if((*i) == null)
    {
      i = m_visuals.erase(i);

      continue;
    }
    else if(!(*i)->valid() || !(*i)->visible())
    {
      continue;
    }
    else if((*i)->type() == kgmVisual::TypeText ||
            (*i)->type() == kgmVisual::TypeSprite)
    {
      if(count_sprites == m_visible_sprites.length())
        m_visible_sprites.realloc(m_visible_sprites.length() + 4096);

      m_visible_sprites[count_sprites++] = (*i);
    }
    else
    {
      vec3 v(0, 0, 0);

      v = (*i)->getTransform() * v;

      vec3  l = (*i)->getBound().max - (*i)->getBound().min;

      if(m_camera->isSphereCross(v, 0.5 * l.length()))
      {
        if((*i)->getMaterial() && (*i)->getMaterial()->alpha())
        {
          if(count_visible_alpha == m_visible_visuals_alpha.length())
            m_visible_visuals_alpha.realloc(m_visible_visuals_alpha.length() + 4096);

          m_visible_visuals_alpha[count_visible_alpha++] = (*i);
        }
        else
        {
          if(count_visible == m_visible_visuals.length())
            m_visible_visuals.realloc(m_visible_visuals.length() + 4096);

          m_visible_visuals[count_visible++] = (*i);
        }
      }
      else
      {
        k++;
      }
    }
  }

  if(count_sprites == m_visible_sprites.length())
    m_visible_sprites.realloc(m_visible_sprites.length() + 1024);

  m_visible_sprites[count_sprites + 1] = null;

  gc->gcCull(1);

  setProjMatrix(m_camera->mProj);
  setViewMatrix(m_camera->mView);
  g_mtx_world.identity();

  gc->gcBegin();
  gc->gcDepth(true, true, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, m_bg_color, 1, 0);

  //prepare for render

  gc->gcBlend(false, null, null);
  gc->gcAlpha(false, null, null);

  //colect lights in viewport

  g_lights_count = 0;

  for(kgmList<kgmLight*>::iterator i = m_lights.begin(); i != m_lights.end(); i.next())
  {
    if((*i) == null)
    {
      i = m_lights.erase(i);

      continue;
    }

    if(!(*i)->active)
      continue;

    if(!m_camera->isSphereCross((*i)->position, kgmLight::LIGHT_RANGE * (*i)->intensity))
      continue;

    g_lights[g_lights_count++] = (*i);

    lights.add((*i));

    if(g_lights_count >= MAX_LIGHTS)
      break;
  }

  /*
   * 1. draw all meshes just with light effect.
   * 2. draw all meshes without lighting parts.
   * 3. multiply step 1 on step 2
   * glBlendFunc(GL_DST_COLOR, GL_ZERO) //Blending formula with these factors : srcColor*destColor+0
   * GL_SRC_COLOR,GL_SRC_COLOR? dst_color, src_color
  */

  //I pass: draw scene only lights

  lighting = true;

  LightRender lr(this);

  //lr.l_render(lights, m_visible_visuals);

  mtx4 m4_identity;
  m4_identity.identity();
  setWorldMatrix(m4_identity);

  for (s32 i = 0; i < count_visible; i++)
  {
    kgmVisual* vis = m_visible_visuals[i];
    kgmMaterial* mtl = (vis->getMaterial())?(vis->getMaterial()):(g_def_material);
    
    box3    bbound = vis->getBound();
    sphere3 sbound;

    bbound.min    = vis->getTransform() * bbound.min;
    bbound.max    = vis->getTransform() * bbound.max;
    sbound.center = bbound.center();
    sbound.radius = 0.5f * bbound.dimension().length();

    setWorldMatrix(vis->getTransform());

    render(mtl);

    if(!mtl->shade() || g_lights_count < 1)
    {
      render(shaders.get(kgmShader::TypeBase).data());
    }
    else
    {
      g_light_active = g_lights[0];

      render(shaders[kgmShader::TypeAmbient].data());
    }

    render(vis);

    render((kgmMaterial*)null);
    render((kgmShader*)null);
  }

  // Sort alpha objects.

  for(s32 i = 0; i < (count_visible_alpha - 1); i++)
  {
    vec3 pos_i(0, 0, 0);

    pos_i = m_visible_visuals_alpha[i]->getTransform() * pos_i;

    for(s32 j = i + 1; j < count_visible_alpha; j++)
    {
      vec3 pos_j(0, 0, 0);
      pos_j = m_visible_visuals_alpha[j]->getTransform() * pos_i;

      if(camera().mPos.distance(pos_j) > camera().mPos.distance(pos_i))
      {
        kgmVisual* vis = m_visible_visuals_alpha[i];

        m_visible_visuals_alpha[i] = m_visible_visuals_alpha[j];
        m_visible_visuals_alpha[j] = vis;
      }
    }
  }

  // Draw alpha objects.

  gc->gcDepth(true, false, gccmp_lequal);

  for(s32 i = 0; i < count_visible_alpha; i++)
  {
    kgmVisual* vis = m_visible_visuals_alpha[i];
    kgmMaterial* mtl = vis->getMaterial();

    box3    bbound = vis->getBound();
    sphere3 sbound;

    bbound.min    = vis->getTransform() * bbound.min;
    bbound.max    = vis->getTransform() * bbound.max;
    sbound.center = bbound.center();
    sbound.radius = 0.5f * bbound.dimension().length();

    setWorldMatrix(vis->getTransform());

    render(mtl);

    if(!mtl->shade() || g_lights_count < 1)
    {
      render(shaders[kgmShader::TypeBase].data());
    }
    else
    {
      g_light_active = g_lights[0];

      render(shaders[kgmShader::TypeAmbient].data());
    }

    render(vis);


    render((kgmMaterial*)null);
    render((kgmShader*)null);
  }

  gc->gcDepth(true, true, gccmp_lequal);

  gc->gcSetShader(null);

  if(lighting)
  {
    lighting = false;
  }

  //draw particles

  kgmList<kgmVisual*>  depthless_particles;

  //I pass with depth category
  //if(m_has_shaders)
  //  render((kgmShader*)shaders[kgmMaterial::ShaderNone]);

  /*for(int i = 0; i < vis_particles.size(); i++)
  {
    kgmVisual*    vis = vis_particles[i];
    kgmParticles* par = vis->getParticles();
    kgmMaterial*  mtl = vis->getMaterial();

    if(par && mtl)
    {
      if(mtl->depth() == false)
      {
        depthless_particles.add(vis);

        continue;
      }

      setWorldMatrix(vis->getTransform());
      render(mtl);
      render(mtl->getShader());
      render(vis->getParticles());
      render((kgmShader*)null);
      render((kgmMaterial*)null);
    }
  }*/

  // depthless meshes

  //depthless particles

  for(int i = 0; i < depthless_particles.size(); i++)
  {
    kgmVisual*    vis = depthless_particles[i];
    kgmMaterial*  mtl = vis->getMaterial();

    setWorldMatrix(vis->getTransform());
    render(mtl);
    render(mtl->getShader());
    render(vis->getParticles());
    render((kgmShader*)null);
    render((kgmMaterial*)null);
  }

  depthless_particles.clear();

  gc->gcCull(gccull_back);

#ifndef NO_SHADERS

  render((kgmShader*)null);

#endif

  // draw icons

  if(m_editor)
  {
    mtx4 mtx;
    mtx.identity();
    setWorldMatrix(mtx);

    for(int i = m_icons.size(); i > 0; i--)
    {
      kgmIcon* icon = m_icons[i - 1];

      if(icon == null)
      {
        m_icons.erase(i - 1);
      }
      else
      {
        kgmMaterial mtl;

        gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);
        mtl.setTexColor(icon->getIcon());
        render(&mtl);
        render(shaders[kgmShader::TypeBase].data());
        render(icon);
        render((kgmShader*)null);
        render((kgmMaterial*)null);
        gc->gcBlend(false, gcblend_srcalpha, gcblend_srcialpha);
      }
    }
  }

  m_r_sprite->render();
  m_r_gui->render();

  gc->gcSetShader(null);
  gc->gcDepth(false, 0, 0);
  gc2DMode();

  m.identity();
  setWorldMatrix(m);

  render(shaders[kgmShader_TypeGui].data());

  gc->gcSetShader(null);
  gc->gcDepth(false, 0, 0);
  gc2DMode();
  m.identity();
  setWorldMatrix(m);

  render(shaders[kgmShader_TypeGui].data());

#ifdef DEBUG
  m_r_fps->render();
#endif

  render((kgmShader*)null);

  gc3DMode();

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
  {
    kgmVisual::Mesh* msh = visual->getMesh();

    u32  pmt;

    if(!msh)
      return;

    switch(msh->getRenderType())
    {
    case kgmMesh::RT_LINE:
      pmt = gcpmt_lines;
      break;
    case kgmMesh::RT_POINT:
      pmt = gcpmt_points;
      break;
    default:
      pmt = gcpmt_triangles;
    };

    gc->gcDraw(pmt, msh->getFvf(),
               msh->getVsize(), msh->getVcount(), msh->getVertices(),
               2, 3 * msh->getFcount(), msh->getFaces());

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
      gc->gcBlend(false, gcblend_srcalpha, gcblend_one);
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
    gc->gcBlend(true, m->srcblend(), m->dstblend());
    m_alpha = true;
  }
  else if(m->alpha() || m->transparency() > 0.0f)
  {
    gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);
    m_alpha = true;
  }

  if(!m->depth())
  {
    gc->gcDepth(true, false, gccmp_less);
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

  if(g_light_active)
  {
    v_light = vec4(g_light_active->position.x, g_light_active->position.y, g_light_active->position.z,
                   g_light_active->intensity);
    v_light_direction = vec4(g_light_active->direction.x, g_light_active->direction.y, g_light_active->direction.z,
                             g_light_active->angle);
    v_light_color = g_light_active->color;
  }

  float random = (float)rand()/(float)RAND_MAX;

  s->start();
  s->set("g_fTime",           kgmTime::getTime());
  s->set("g_fRandom",         random);
  s->set("g_fShine",          g_fShine);
  s->set("g_fAmbient",        g_fAmbient);
  s->set("g_mProj",           g_mtx_proj);
  s->set("g_mView",           g_mtx_view);
  s->set("g_mTran",           g_mtx_world);
  s->set("g_mNorm",           g_mtx_normal);
  s->set("g_vColor",          g_vec_color);
  s->set("g_vSpecular",       g_vec_specular);
  s->set("g_vLight",          v_light);
  s->set("g_vLightColor",     v_light_color);
  s->set("g_vLightDirection", v_light_color);
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

  if(g_lights_count > 1)
  {
    static vec4 lights[MAX_LIGHTS - 1];

    for(u32 i = 0; i < (g_lights_count - 1); i++)
    {
      lights[i] = vec4(g_lights[i + 1]->position.x, g_lights[i + 1]->position.y,
          g_lights[i + 1]->position.z, g_lights[i + 1]->intensity);

    }

    s->set("g_vLights", lights[0], g_lights_count - 1);
    s->set("g_iLights", g_lights_count - 1);
  }

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
  default:
    pmt = gcpmt_triangles;
  };

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
  gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);
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

  gc->gcBlend(0, 0, 0);
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

  gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);
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

  gc->gcBlend(0, 0, 0);
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

kgmShader* kgmGraphics::toShader(kgmShader::Shader shader)
{
  auto i = shaders.get(shader);

  if(i.data())
    return i.data();

  return shaders.begin().data();
}
