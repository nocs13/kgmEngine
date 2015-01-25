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

#include <stdlib.h>

////////////////////
KGMOBJECT_IMPLEMENT(kgmGraphics,  kgmObject);
KGMOBJECT_IMPLEMENT(kgmFrustum,   kgmObject);
KGMOBJECT_IMPLEMENT(kgmCamera,    kgmFrustum);

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
vec4       g_vec_ambient = vec4(1, 1, 1, 1);

kgmShader* g_shd_active = null;

kgmLight*  g_light_active = null;
kgmLight*  g_lights[MAX_LIGHTS] = {null};
u32        g_lights_count = 0;
f32        g_fAmbient = 0.91f;
void*      g_tex_black = null;
void*      g_tex_white = null;
void*      g_tex_gray  = null;
void*      g_map_shadow = null;

kgmLight     g_def_light;
kgmMaterial  g_def_material;

enum
{
kgmShader_TypeGui = 100,
kgmShader_TypeIcon,
kgmShader_TypeLight,
kgmShader_TypeLights,
kgmShader_TypeAmbient
};

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

  font = null;

  m_max_faces = 500000;

  m_has_shaders = false;
  m_has_buffers = false;
  m_alpha       = false;
  m_depth       = true;
  m_culling     = true;

  m_bg_color    = 0xFF000000;

  gui_style = new kgmGuiStyle();

  g_def_material.setShader(kgmShader::TypeNone);
  g_def_material.m_color = kgmMaterial::Color(0.7, 0.7, 0.7, 1.0);

  if(g)
  {
    int val;

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

    g->gcGet(gcsup_shaders, &val);

    if(val != 0)
      m_has_shaders = true;

    g->gcGet(gcsup_fbuffers, &val);

    if(val != 0)
      m_has_buffers = true;
  }

  //m_has_shaders = false;

  if(m_has_shaders)
  {
#ifdef DEBUG
    kgm_log() << "Prepare shaders \n";
#endif

    if(rc != null)
    {
      kgmShader* shader = null;

      shaders.add(kgmShader::TypeNone,   rc->getShader("none.glsl"));
      shaders.add(kgmShader::TypeBase,   rc->getShader("base.glsl"));
      shaders.add(kgmShader::TypeBlend,  rc->getShader("blend.glsl"));
      shaders.add(kgmShader::TypeSkin,   rc->getShader("skin.glsl"));
      shaders.add(kgmShader_TypeGui,     rc->getShader("gui.glsl"));
      shaders.add(kgmShader_TypeIcon,    rc->getShader("icon.glsl"));
      shaders.add(kgmShader_TypeLight,   rc->getShader("light.glsl"));
      shaders.add(kgmShader_TypeAmbient, rc->getShader("ambient.glsl"));

      //shader = rc->getShader("lights.glsl");
      shader = rc->getShader("base.glsl");

      if(shader)
      {
        shader->m_input |= kgmShader::IN_VEC4_LIGHTS;
        shaders.add(kgmShader_TypeLights, shader);
      }
    }
  }

  if(m_has_buffers)
  {
    //gc->gcGenTexture(null);
  }

  m_camera.set(PI / 6, 1, 1, 1000, vec3(0, 0, 1), vec3(-1, 0, 0), vec3(0, 0, 1));
  g_mtx_iden.identity();

  //m_editor = false;
}

kgmGraphics::~kgmGraphics()
{
  if(g_tex_black)
    gc->gcFreeTexture(g_tex_black);

  if(g_tex_white)
    gc->gcFreeTexture(g_tex_white);

  if(g_tex_gray)
    gc->gcFreeTexture(g_tex_gray);

  gui_style->release();
  m_guis.clear();
  clear();
}

void kgmGraphics::clear()
{
  for(int i = 0; i < m_materials.size(); i++)
    m_materials[i]->release();

  m_materials.clear();

  for(int i = 0; i < m_visuals.size(); i++)
    m_visuals[i]->release();

  m_visuals.clear();

  for(int i = 0; i < m_lights.size(); i++)
    m_lights[i].light->release();

  m_lights.clear();

  for(int i = 0; i < m_icons.size(); i++)
    m_icons[i]->release();

  m_icons.clear();

#ifdef DEBUG
  for(int i = 0; i < m_bodies.size(); i++)
  {
    m_bodies[i]->release();
  }

  m_bodies.clear();
#endif
}

kgmShader* s_def = null;
void kgmGraphics::build()
{
}

void kgmGraphics::setDefaultFont(kgmFont* f)
{
  font = f;
  gui_style->gui_font = f;
}

void kgmGraphics::setGuiStyle(kgmGuiStyle* s)
{
  if(!s)
    return;

  if(gui_style)
    gui_style->release();

  gui_style = s;
}

void kgmGraphics::setProjMatrix(mtx4 &m)
{
  g_mtx_proj = m;

  if(!m_has_shaders)
    gc->gcSetMatrix(gcmtx_proj, m.m);
}

void kgmGraphics::setViewMatrix(mtx4 &m)
{
  g_mtx_view = m;

  if(!m_has_shaders)
    gc->gcSetMatrix(gcmtx_view, m.m);
  else
  {
    mtx4 im = m;

    im.invert();
    g_mtx_normal[0] = im[0], g_mtx_normal[1] = im[1], g_mtx_normal[2] = im[2];
    g_mtx_normal[3] = im[4], g_mtx_normal[4] = im[5], g_mtx_normal[5] = im[6];
    g_mtx_normal[6] = im[8], g_mtx_normal[7] = im[9], g_mtx_normal[8] = im[10];
  }
}

void kgmGraphics::setWorldMatrix(mtx4 &m)
{
  g_mtx_world = m;

  mtx4 mw = m * g_mtx_view;

  if(!m_has_shaders)
    gc->gcSetMatrix(gcmtx_view, mw.m);
}

void kgmGraphics::setEditor(bool e)
{
  m_editor = e;
}

void kgmGraphics::resize(float width, float height)
{
  gc->gcSetViewport(0, 0, width, height, 1.0, 100000.0);

  m_camera.set(PI / 6, width / height, .1f, 100000.0,
                 m_camera.mPos, m_camera.mDir, m_camera.mUp);

  m_camera.viewport((float)width, (float)height);

  g_mtx_orto.ortho(0, width, height, 0, 1, -1);
  m_viewport = iRect(0, 0, width, height);
}

/*
 * Main Graphical Render process
*/

void kgmGraphics::render()
{
  static float alpha = 0.0;
  static float m_time[4];
  s32 rect[4];
  vec3 v[2];
  mtx4 m;
  vec4 myvar;
  int i = 0, k = 0;
  bool lighting = false;

  mtx4 mvw, mpr;

  kgmList<kgmVisual*> vis_mesh, vis_text, vis_blend, vis_sprite, vis_particles;

  // parse visible visual objects

  for(kgmList<kgmVisual*>::iterator i = m_visuals.begin(); i != m_visuals.end(); i.next())
  {
    if((*i)->removed())
    {
      (*i)->release();
      i = m_visuals.erase(i);

      continue;
    }
    else if(!(*i)->valid() || !(*i)->visible())
    {
      continue;
    }
    else if((*i)->type() == kgmVisual::TypeText)
    {
      vis_text.add(*i);
    }
    else if((*i)->type() == kgmVisual::TypeSprite)
    {
      vis_sprite.add(*i);
    }
    else
    {
      vec3 v(0, 0, 0);

      v = (*i)->getTransform() * v;

      vec3  l = (*i)->getBound().max - (*i)->getBound().min;

      if(m_camera.isSphereCross(v, 0.5 * l.length()))
      {
        if((*i)->type() == kgmVisual::TypeParticles)
        {
          vis_particles.add(*i);
        }
        else if((*i)->getMaterial() && (*i)->getMaterial()->m_blend)
        {
            vis_blend.add(*i);
        }
        else
        {
          vis_mesh.add(*i);
        }
      }
      else
      {
        k++;
      }
    }
  }

  gc->gcCull(1);

  setProjMatrix(m_camera.mProj);
  setViewMatrix(m_camera.mView);
  g_mtx_world.identity();

  gc->gcBegin();
  gc->gcDepth(true, 1, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, m_bg_color, 1, 0);

#ifdef DEBUG
  //Grid
  Vert lines[] = {{{0, 0, 0}, 0xff0000ff}, {{1000, 0, 0}, 0xff0000ff},
                  {{0, 0, 0}, 0xff00ff00}, {{0, 1000, 0}, 0xff00ff00},
                  {{0, 0, 0}, 0xffff0000}, {{0, 0, 1000}, 0xffff0000}};
  if(m_has_shaders)
  {
    render(shaders[kgmShader::TypeNone]);
  }

  gc->gcDraw(gcpmt_lines, gcv_xyz | gcv_col, sizeof(Vert), 6, lines, 0, 0, null);

  if(m_has_shaders)
  {
    render((kgmShader*)null);
  }
#endif

  //prepare for render

  gc->gcBlend(false, null, null);
  gc->gcAlpha(false, null, null);

  //colect lights in viewport

  g_lights_count = 0;

  for(kgmList<Light>::iterator i = m_lights.begin(); i != m_lights.end(); i.next())
  {
    if((*i).remove == true)
    {
      (*i).light->release();

      i = m_lights.erase(i);

      if(i == m_lights.end())
        break;

      continue;
    }

    if(!(*i).light->active)
      continue;

    if(!m_camera.isSphereCross((*i).light->position, kgmLight::LIGHT_RANGE * (*i).light->intensity))
       continue;

    g_lights[g_lights_count++] = (*i).light;

    if(g_lights_count >= MAX_LIGHTS)
      break;
  }

  //I pass: draw scene by ambient

  lighting = true;

  if(m_has_shaders)
  {
    mtx4 m;
    m.identity();
    setWorldMatrix(m);
  }

  for(kgmList<kgmVisual*>::iterator i = vis_mesh.begin(); i != vis_mesh.end(); ++i)
  {
    kgmMaterial* mtl = ((*i)->getMaterial())?((*i)->getMaterial()):(&g_def_material);
    
    box3    bbound = (*i)->getBound();
    sphere3 sbound;

    bbound.min    = (*i)->getTransform() * bbound.min;
    bbound.max    = (*i)->getTransform() * bbound.max;
    sbound.center = bbound.center();
    sbound.radius = 0.5f * bbound.dimension().length();

    setWorldMatrix((*i)->getTransform());

    tcolor = (mtl->getTexColor())?(mtl->getTexColor()->m_texture):(g_tex_white);
    gc->gcSetTexture(0, tcolor);
    tnormal = null;
    gc->gcSetTexture(1, tnormal);
    tspecular = null;
    gc->gcSetTexture(2, tspecular);

    if(m_has_shaders)
    {
      render(shaders[kgmShader_TypeAmbient]);
    }

    render(*i);

    render((kgmMaterial*)null);
  }

  // draw meshes light by light and blend to framebuffer

  gc->gcBlend(true, gcblend_one, gcblend_one);

  for(int i = 0; i < g_lights_count; i++)
  {
    g_light_active = g_lights[i];

    for(kgmList<kgmVisual*>::iterator i = vis_mesh.begin(); i != vis_mesh.end(); ++i)
    {
      kgmMaterial* mtl = ((*i)->getMaterial())?((*i)->getMaterial()):(&g_def_material);

      box3    bbound = (*i)->getBound();
      sphere3 sbound;

      bbound.min    = (*i)->getTransform() * bbound.min;
      bbound.max    = (*i)->getTransform() * bbound.max;
      sbound.center = bbound.center();
      sbound.radius = 0.5f * bbound.dimension().length();

      setWorldMatrix((*i)->getTransform());

      tcolor = g_tex_white;
      gc->gcSetTexture(0, tcolor);
      tnormal = (mtl->getTexNormal())?(mtl->getTexNormal()->m_texture):(g_tex_black);
      gc->gcSetTexture(1, tnormal);
      tspecular = (mtl->getTexSpecular())?(mtl->getTexSpecular()->m_texture):(g_tex_black);
      gc->gcSetTexture(2, tspecular);

      if(m_has_shaders)
      {
        render(shaders[kgmShader_TypeLight]);
      }

      render(*i);

      render((kgmMaterial*)null);
    }

    g_light_active = null;
  }

  gc->gcBlend(false, null, null);

  if(m_has_shaders)
  {
    gc->gcSetShader(null);
  }

  if(lighting)
  {
    lighting = false;
  }

  //draw particles
  kgmList<kgmVisual*>  depthless_particles;

  //I pass with depth category
  //if(m_has_shaders)
  //  render((kgmShader*)shaders[kgmMaterial::ShaderNone]);

  for(int i = 0; i < vis_particles.size(); i++)
  {
    kgmVisual*    vis = vis_particles[i];
    kgmParticles* par = vis->getParticles();
    kgmMaterial*  mtl = vis->getMaterial();

    if(par && mtl)
    {
      if(mtl->m_depth == false)
      {
        depthless_particles.add(vis);

        continue;
      }

      setWorldMatrix(vis->getTransform());
      render(mtl);
      render(toShader(mtl->getShader()));
      render(vis->getParticles());
      render((kgmShader*)null);
      render((kgmMaterial*)null);
    }
  }

  // depthless meshes

  //depthless particles

  for(int i = 0; i < depthless_particles.size(); i++)
  {
    kgmVisual*    vis = vis_particles[i];
    kgmParticles* par = vis->getParticles();
    kgmMaterial*  mtl = vis->getMaterial();

    setWorldMatrix(vis->getTransform());
    render(mtl);
    render(toShader(mtl->getShader()));
    render(vis->getParticles());
    render((kgmShader*)null);
    render((kgmMaterial*)null);
  }

  depthless_particles.clear();

  gc->gcCull(gccull_back);

  if(m_has_shaders)
    render((kgmShader*)null);

  // draw icons

  if(m_editor)
  {
    mtx4 mtx;
    mtx.identity();
    setWorldMatrix(mtx);

    for(int i = m_icons.size(); i > 0; i--)
    {
      Icon* icon = m_icons[i -1];

      if(icon->remove)
      {
        icon->release();
        m_icons.erase(i - 1);
      }
      else
      {
        kgmMaterial mtl;
        gc->gcAlpha(true, gccmp_great, 0.5);
        mtl.setTexColor(icon->getIcon());
        render(&mtl);
        render(shaders[kgmShader_TypeIcon]);
        render(icon);
        render((kgmShader*)null);
        render((kgmMaterial*)null);
        gc->gcAlpha(false, gccmp_great, 0.0);
      }
    }
  }

#ifdef DEBUG
  mtx4 mid;
  mid.identity();
  setViewMatrix(m_camera.mView);
  setWorldMatrix(mid);

  if(m_has_shaders)
  {
    render(shaders[kgmShader::TypeNone]);
  }

  for(int i = m_bodies.size(); i > 0;  i--)
  {
    kgmBody* body = m_bodies[i - 1];

    if(body->removed())
    {
      body->release();
      m_bodies.erase(i - 1);

      continue;
    }
    else if(body->m_intersect)
    {
      continue;
    }

    mtx4 mtx_tr;
    vec3 vec_points[8];
    s16  lines[24] = {0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,4, 1,5, 2,6, 3,7};

    kgmMesh::Vertex_P_C gr_points[8];

    obox3 ob = body->getOBox();
    ob.points(vec_points);

    for(int i = 0; i < 8; i++)
    {
      gr_points[i].pos = vec_points[i];
      gr_points[i].col = 0xffffffff;
    }

    gc->gcDraw(gcpmt_lines, gcv_xyz|gcv_col, sizeof(kgmMesh::Vertex_P_C), 8, gr_points, 2, 24, lines);
  }

  for(int i = m_lights.size(); i > 0;  i--)
  {
    kgmLight* light = m_lights[i - 1].light;

    box3 bb;
    bb.min = light->position - vec3(1,1,1);
    bb.max = light->position + vec3(1,1,1);
    vec3 vec_points[8];
    s16  lines[24] = {0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,4, 1,5, 2,6, 3,7};

    kgmMesh::Vertex_P_C gr_points[8];

    bb.points(vec_points);

    for(int i = 0; i < 8; i++)
    {
      gr_points[i].pos = vec_points[i];
      gr_points[i].col = 0xffffffff;
    }

    gc->gcDraw(gcpmt_lines, gcv_xyz|gcv_col, sizeof(kgmMesh::Vertex_P_C), 8, gr_points, 2, 24, lines);
  }

  if(m_has_shaders)
  {
    gc->gcSetShader(null);
  }
#endif

  // draw gui

  gc->gcSetShader(null);
  gc->gcDepth(false, 0, 0);
  gc2DMode();

  if(m_has_shaders)
  {
    mtx4 m;
    m.identity();
    setWorldMatrix(m);

    render((kgmShader*)shaders[kgmShader_TypeGui]);
  }

  //draw sprites

  for(kgmList<kgmVisual*>::iterator i = vis_sprite.begin(); i != vis_sprite.end(); ++i)
  {
    render((*i)->getSprite());
  }

  // render guis

  for(int i = m_guis.size(); i > 0; i--)
  {
    kgmGui* gui = m_guis[i - 1];

    if(gui->erased())
    {
      gui->release();
      m_guis.erase(i - 1);
    }
    else if(gui->visible())
    {
      render(gui);
    }
  }

  // render text

  for(int i = 0; i < vis_text.size(); i++)
  {
    kgmText* text = vis_text[i]->getText();
    kgmGui::Rect rc(text->m_rect.x, text->m_rect.y,
                    text->m_rect.w, text->m_rect.h);
    gcDrawText(font, text->m_size / 2, text->m_size, text->m_color, rc, text->m_text);
  }

#ifdef DEBUG
  char info[4096] = {0};
  sprintf(info, "camera direction: %f %f %f \ncamera position: %f %f %f \nobject count: %i \
  unvisible: %i\n",
          m_camera.mDir.x, m_camera.mDir.y, m_camera.mDir.z,
          m_camera.mPos.x, m_camera.mPos.y, m_camera.mPos.z,
          kgmObject::objectCount(), k);
  kgmString text(info);
  //gcDrawText(font, 10, 15, 0xffffffff, kgmGui::Rect(1, 400, 600, 200), text);

  static u32 fps_start_time = kgmTime::getTicks();
  static u32 fps_frame_time = 0;
  static u32 fps_frames = 0;
  static kgmString fps_text;

  if(kgmTime::getTicks() - fps_start_time > 1000)
  {
    memset(info, 0, sizeof(info));
    sprintf(info, "FPS: %i\0", fps_frames);
    fps_text = info;
    fps_frames = 0;
    fps_start_time = kgmTime::getTicks();
  }
  else
  {
    fps_frames++;
  }

  gcDrawText(font, 10, 15, 0xffffffff, kgmGui::Rect(m_viewport.width() - 200, 1, 100, 20), fps_text);
#endif

  if(m_has_shaders)
    render((kgmShader*)null);

  gc3DMode();
  gc->gcEnd();
  gc->gcRender();

  // clear&reset

  gc->gcSetTexture(0, 0);
  gc->gcSetTexture(1, 0);
  gc->gcSetTexture(2, 0);
  gc->gcSetTexture(3, 0);

  vis_particles.clear();
  vis_sprite.clear();
  vis_blend.clear();
  vis_text.clear();
  vis_mesh.clear();
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
  }

  //visual->update();

#ifdef DEBUG
  /*
  if(visual->m_tm_joints)
  {
    int joints = visual->m_skeleton->m_joints.size();

    for(int i = 0; i < joints; i++)
    {
      vec3 v(0, 0, 0);
      Vert line[2];
      int  ipar = visual->m_skeleton->m_joints[i]->i;

      if(ipar < 0 || ipar >= visual->m_skeleton->m_joints.size())
        continue;

      line[0].v = visual->m_tm_joints[ipar] * v;
      line[1].v = visual->m_tm_joints[i] * v;
      line[0].c = line[1].c = 0xffffffff;
      gc->gcDraw(gcpmt_lines, gcv_xyz | gcv_col, sizeof(Vert), 2, line, 0, 0, null);
    }
  }
  //*/
#endif
}

void kgmGraphics::render(kgmSprite* sprite)
{
  struct Point{ vec3 pos; u32 col; vec2 uv; };
  Point v[4];

  v[0].pos = vec3(sprite->m_rect.x, sprite->m_rect.y, 0);
  v[0].uv.x = 0.0f, v[0].uv.y = 0.0f;
  v[1].pos = vec3(sprite->m_rect.x, sprite->m_rect.y + sprite->m_rect.h, 0);
  v[1].uv.x = 0.0f, v[1].uv.y = 1.0f;
  v[2].pos = vec3(sprite->m_rect.x + sprite->m_rect.w, sprite->m_rect.y, 0);
  v[2].uv.x = 1.0f, v[2].uv.y = 0.0f;
  v[3].pos = vec3(sprite->m_rect.x + sprite->m_rect.w, sprite->m_rect.y + sprite->m_rect.h, 0);
  v[3].uv.x = 1.0f, v[3].uv.y = 1.0f;

  v[0].col = v[1].col = v[2].col = v[3].col = sprite->m_color.color;
  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(Point), 4, v, 0, 0, 0);
}

void kgmGraphics::render(kgmParticles* particles)
{
  if(!particles || !particles->getMesh())
    return;

  mtx4    mtr = g_mtx_view;
  vec3    rv, uv;
  //float2  dim(1, 1);


  //rv = vec3(m_camera.camera.mView.m[0], m_camera.camera.mView.m[1], m_camera.camera.mView.m[2]);
  rv = vec3(mtr.m[0], mtr.m[2], mtr.m[1]);
  rv.normalize();
  //uv = vec3(m_camera.camera.mView.m[4], m_camera.camera.mView.m[5], m_camera.camera.mView.m[6]);
  uv = rv.cross(m_camera.mDir);//vec3(mtr.m[4], mtr.m[5], mtr.m[6]);
  uv.normalize();

  s32           count;

  if(particles->m_typerender == kgmParticles::RTypeBillboard)
  {
    count = particles->m_count;
    kgmMesh::Vertex_P_C_T* points = (kgmMesh::Vertex_P_C_T*)particles->getMesh()->vertices();

    for(s32 i = 0; i < count; i++)
    {
      vec3    pos   = particles->m_particles[i].pos;
      float   scale = particles->m_particles[i].scale;
      float   time  = particles->m_particles[i].time;
      float   life  = particles->m_particles[i].life;
      vec3    crv = rv * scale,
              cuv = uv * scale;

      float   txu_s = 0.0f, txv_s = 0.0f;
      float   txu_e = 1.0f, txv_e = 1.0f;

      if(particles->tex_slide_cols > 1 || particles->tex_slide_rows > 1)
      {
        u32 frames = particles->tex_slide_cols * particles->tex_slide_rows;
        u32 frame  = (u32)((float)frames * time / life);

        float sw = (particles->tex_slide_cols > 1) ? (1.0f / particles->tex_slide_cols) : (1.0f);
        float sh = (particles->tex_slide_rows > 1) ? (1.0f / particles->tex_slide_rows) : (1.0f);

        u32   ir = frame / particles->tex_slide_cols;
        u32   ic = frame % particles->tex_slide_cols;

        txu_s = sw * ic;
        txv_s = sh * ir;
        txu_e = sw * (ic + 1);
        txv_e = sh * (ir + 1);
      }

      points[i].pos = (pos - crv + cuv);
        points[i].uv = vec2(txu_s, txv_s);
      points[i + 1].pos = (pos - crv - cuv);
        points[i + 1].uv = vec2(txu_s, txv_e);
      points[i + 2].pos = (pos + crv + cuv);
        points[i + 2].uv = vec2(txu_e, txv_s);

      points[i + 3].pos = (pos + crv - cuv);
        points[i + 3].uv = vec2(txu_e, txv_e);
      points[i + 4].pos = (pos + crv + cuv);
        points[i + 4].uv = vec2(txu_e, txv_s);
      points[i + 5].pos = (pos - crv - cuv);
        points[i + 5].uv = vec2(txu_s, txv_e);

      points[i].col = points[i + 1].col =
      points[i + 2].col = points[i + 3].col =
      points[i + 4].col = points[i + 5].col = particles->m_particles[i].col.color;
    }

    //gc->gcDraw(gcpmt_triangles, gcv_xyz|gcv_col|gcv_uv0, sizeof(PrPoint), 6 * count, points, 0, 0, 0);
  }

  render(particles->getMesh());
}

void kgmGraphics::render(Icon* icon)
{
  mtx4    mtr = g_mtx_view;
  vec3    rv, uv;

  rv = vec3(mtr.m[0], mtr.m[2], mtr.m[1]);
  rv.normalize();
  uv = rv.cross(m_camera.mDir);
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

void kgmGraphics::render(kgmMaterial* m){

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
      gc->gcDepth(true, true, gccmp_less);
      m_depth = true;
    }

    tcolor = tnormal = tspecular = null;

    return;
  }

  if(!m->m_depth)
  {
    gc->gcDepth(false, false, gccmp_less);
    m_depth = false;
  }

  if(m->m_blend)
  {
    gc->gcBlend(true, m->m_srcblend, m->m_dstblend);
    m_alpha = true;
  }

  if(m->m_2side)
  {
    gc->gcCull(0);
    m_culling = false;
  }

  if(m->hasTexColor())
  {
    gc->gcSetTexture(0, m->getTexColor()->m_texture);
    tcolor = m->getTexColor()->m_texture;
  }
  else
  {
    gc->gcSetTexture(0, g_tex_white);
    tcolor = g_tex_white;
  }

  if(m->hasTexNormal())
  {
    gc->gcSetTexture(1, m->getTexNormal()->m_texture);
    tnormal = m->getTexNormal()->m_texture;
  }
  else
  {
    gc->gcSetTexture(1, g_tex_black);
    tnormal = g_tex_black;
  }

  if(m->hasTexSpecular())
  {
    gc->gcSetTexture(2, m->getTexSpecular()->m_texture);
    tspecular = m->getTexSpecular()->m_texture;
  }
  else
  {
    gc->gcSetTexture(2, g_tex_black);
    tspecular = g_tex_black;
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

  if(g_light_active)
    v_light = vec4(g_light_active->position.x, g_light_active->position.y, g_light_active->position.z,
                   g_light_active->intensity);

  float random = (float)rand()/(float)RAND_MAX;

  s->start();
  s->set("g_fTime",     kgmTime::getTime());
  s->set("g_fRandom",   random);
  s->set("g_fShine",    0.5f);
  s->set("g_fAmbient",  g_fAmbient);
  s->set("g_mProj",     g_mtx_proj);
  s->set("g_mView",     g_mtx_view);
  s->set("g_mTran",     g_mtx_world);
  s->set("g_mNorm",     g_mtx_normal);
  s->set("g_vAmbient",  g_vec_ambient);
  s->set("g_vLight",    v_light);
  s->set("g_vEye",      m_camera.mPos);
  s->set("g_vEyeDir",   m_camera.mDir);

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

    for(int i = 0; i < (g_lights_count - 1); i++)
    {
      lights[i] = vec4(g_lights[i + 1]->position.x, g_lights[i + 1]->position.y,
      g_lights[i + 1]->position.z, g_lights[i + 1]->intensity);

    }

    s->set("g_vLights", lights[0], g_lights_count - 1);
    s->set("g_iLights", g_lights_count - 1);
  }

  g_shd_active = s;
}

void kgmGraphics::render(kgmMesh *m){
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

void kgmGraphics::render(kgmGui* gui){
  kgmGui::Rect rect;
  kgmString    text;

  if(!gui)
    return;

  gui->getRect(rect, true);

  text = gui->getText();

  if(gui->m_hasAlpha)
    gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);

  if(gui->isClass(kgmGuiButton::Class))
  {
    u32 fwidth = (u32)((float)rect.w / (float)(text.length() + 1));
    u32 fheight = (u32)((float)rect.h * (float)0.75f);
    u32 tlen = text.length();
    u32 fw = (tlen) * fwidth;
    u32 fh = fheight;
    kgmGui::Rect tClip = rect;

    tClip.x = rect.x + rect.w / 2 - fw / 2;
    tClip.y = rect.y + rect.h / 2 - fh / 2;
    tClip.w = fw;
    tClip.h = fh;

    switch(((kgmGuiButton*)gui)->getState())
    {
    case kgmGuiButton::StateFocus:
      gcDrawRect(rect, gui_style->sbutton.ac_color, gui_style->sbutton.image);
      break;
    case kgmGuiButton::StateClick:
      gcDrawRect(rect, gui_style->sbutton.fg_color, gui_style->sbutton.image);
      break;
    case kgmGuiButton::StateNone:
    default:
      gcDrawRect(rect, gui_style->sbutton.bg_color, gui_style->sbutton.image);
    }

    if(text.length() > 0)
      gcDrawText(gui_style->gui_font, fwidth, fheight, gui_style->sbutton.tx_color, tClip, text);
  }
  else if(gui->isClass(kgmGuiScroll::Class))
  {
    kgmGuiScroll* gscroll = (kgmGuiScroll*)gui;
    kgmGui::Rect  srect = gscroll->getScrollerRect();

    srect = gscroll->toAbsolute(srect);

    gcDrawRect(rect, gui_style->sscroll.bg_color, gui_style->sscroll.image);
    gcDrawRect(srect, gui_style->sscroll.fg_color, gui_style->sscroll.image);
  }
  else if(gui->isClass(kgmGuiList::Class))
  {
    kgmGuiList* glist = (kgmGuiList*)gui;

    u32 item_cnt = ((kgmGuiList*)gui)->m_items.size();

    kgmGui::Rect srect = glist->getItemRect(glist->getFirstVisibleItem());
    srect = glist->toAbsolute(srect);

    //Draw Main Rect
    gcDrawRect(rect, gui_style->slist.bg_color, gui_style->slist.image);

    //Draw Items Rects
    for(int i = glist->getFirstVisibleItem();
            i < (glist->getFirstVisibleItem() + glist->getVisibleItemsCount());
            i++)
    {
      if(i >= item_cnt)
        break;

      kgmString item;
      item = ((kgmGuiList*)gui)->m_items[i];

      kgmGui::Rect frect;
      frect = glist->getItemRect(i);
      frect = glist->toAbsolute(frect);

      if(i == glist->getSel())
      {
        gcDrawRect(frect, gui_style->slist.fg_color, gui_style->slist.image);
      }

      if(rect.inside(frect))
      {
        gcDrawText(gui_style->gui_font, frect.height() / 2, frect.height(),
                   gui_style->slist.tx_color, frect, item);
      }
    }

    if(glist->m_scroll && glist->m_scroll->visible())
    {
      render(glist->m_scroll);
    }
  }
  else if(gui->isType(kgmGuiText::Class))
  {
    kgmGuiText* gtext = (kgmGuiText*)gui;

    if(gui->m_hasMouse )
    {
      gcDrawRect(rect, gui_style->stext.fg_color, gui_style->stext.image);
    }
    else
    {
      gcDrawRect(rect, gui_style->stext.bg_color, gui_style->stext.image);
    }

    if(text.length() > 0)
    {
      if(!gtext->isReadOnly() && gui->getFocus() == gui)
      {
        u32 i = gtext->getCursor();
        kgmGui::Rect rc(rect.x + 1 + (u32)(0.5 * gui_style->stext.ft_size) * i, rect.y + 1,
                        (u32)(0.5 * gui_style->stext.ft_size - 1), gui_style->stext.ft_size);

        if(rect.inside(rc))
          gcDrawRect(rc, gui_style->stext.cr_color, null);
      }

      kgmString c_text;
      u32       c_len = rect.width() / (u32)(0.5 * gui_style->stext.ft_size);
      u32       c_idx = gtext->getIndex() - gtext->getCursor();

      if(c_idx + c_len > text.length())
        c_len = text.length() - c_idx;

      c_text.alloc(c_len);

      for(u32 i = 0; i < c_len; i++)
      {
        if(c_idx + i >= text.length())
          break;

        c_text.data()[i] = text.data()[c_idx + i]; 
      }

      gcDrawText(gui_style->gui_font, (u32)(0.5 * gui_style->stext.ft_size),
                 gui_style->stext.ft_size, gui_style->stext.tx_color, rect, c_text);
    }
  }
  else if(gui->isClass(kgmGuiMenu::Class))
  {
    /*if(gui->m_hasMouse )
    {
      gcDrawRect(rect, gui_style->smenu.fg_color, gui_style->smenu.image);
    }
    else
    {
      gcDrawRect(rect, gui_style->smenu.bg_color, gui_style->smenu.image);
    }*/

    kgmGuiMenu* menu = (kgmGuiMenu*)gui;

    if(menu->getItem())
      renderGuiMenuItem(menu, menu->getItem());
  }
  else if(gui->isClass(kgmGuiTab::Class))
  {
    //render((kgmGuiTab*)gui);
  }
  else if(gui->isClass(kgmGuiProgress::Class))
  {
    //render((kgmGuiProgress*)gui);
  }
  else if(gui->isClass(kgmGuiLabel::Class))
  {
    u32 fwidth = (u32)((float)rect.w / (float)(text.length() + 1));
    u32 fheight = (u32)((float)rect.h * (float)0.75f);
    u32 tlen = text.length();
    u32 fw = (tlen) * fwidth;
    u32 fh = fheight;
    kgmGui::Rect tClip = rect;

    tClip.x = rect.x + rect.w / 2 - fw / 2;
    tClip.y = rect.y + rect.h / 2 - fh / 2;
    tClip.w = fw;
    tClip.h = fh;

    fwidth = ((kgmGuiLabel*)gui)->getFontWidth();
    fheight = ((kgmGuiLabel*)gui)->getFontHeight();

    gcDrawRect(rect, gui_style->slabel.bg_color, gui_style->slabel.image);

    if(text.length() > 0)
      gcDrawText(gui_style->gui_font, fwidth, fheight, gui_style->sbutton.tx_color, tClip, text);
  }
  else if(gui->isClass(kgmGuiCheck::Class))
  {
    u32 fwidth = (u32)((float)(rect.w - 25) / (float)(text.length() + 1));
    u32 fheight = (u32)((float)rect.h * (float)0.75f);
    u32 tlen = text.length();
    u32 fw = (tlen) * fwidth;
    u32 fh = fheight;
    kgmGui::Rect tClip = rect;

    tClip.x = rect.x + rect.w / 2 - fw / 2;
    tClip.y = rect.y + rect.h / 2 - fh / 2;
    tClip.w = fw;
    tClip.h = fh;

    kgmGui::Rect btClip = rect;
    kgmGui::Rect txClip = rect;
    kgmGui::Rect chClip = rect;

    btClip.x += 2;
    btClip.y += 2;
    btClip.w = 20;
    btClip.h = rect.h - 4;

    chClip = btClip;
    chClip.x += 3;
    chClip.y += 3;
    chClip.w -= 6;
    chClip.h -= 6;

    txClip.x += 25;
    txClip.w -= 25;

    fwidth = ((kgmGuiCheck*)gui)->getFontWidth();
    fheight = ((kgmGuiCheck*)gui)->getFontHeight();

    switch(((kgmGuiCheck*)gui)->getState())
    {
    case kgmGuiCheck::StateFocus:
      gcDrawRect(rect, gui_style->scheck.ac_color, gui_style->scheck.image);
      break;
    case kgmGuiCheck::StateNone:
    default:
      gcDrawRect(rect, gui_style->scheck.bg_color, gui_style->scheck.image);
    }

    gcDrawRect(btClip, gui_style->scheck.bg_check, gui_style->scheck.image);

    if(((kgmGuiCheck*)gui)->isCheck())
      gcDrawRect(chClip, gui_style->scheck.fg_check, gui_style->scheck.image);

    if(text.length() > 0)
      gcDrawText(gui_style->gui_font, fwidth, fheight, gui_style->sbutton.tx_color, txClip, text);
  }
  else if(gui->isType(kgmGui::Class))
  {
    if(gui->m_hasMouse )
    {
      gcDrawRect(rect, gui_style->sgui.fg_color, gui_style->sgui.image);
    }
    else
    {
      gcDrawRect(rect, gui_style->sgui.bg_color, gui_style->sgui.image);
    }
  }

  if(gui->m_hasAlpha)
    gc->gcBlend(false, gcblend_zero, gcblend_zero);

  for(int i = 0; i < gui->m_childs.length(); i++)
  {
    if(gui->m_childs[i]->m_view)
      render(gui->m_childs[i]);
  }
}

void kgmGraphics::renderGuiMenuItem(kgmGui* menu, void *i)
{
  kgmGuiMenu::Item* item = (kgmGuiMenu::Item*)i;

  kgmGui::Rect prect;

  menu->getRect(prect, true);

  iRect rect = item->getRect();

  for(int i = 0; i < item->getItemsCount(); i++)
  {
    kgmGuiMenu::Item* citem = item->getItem(i);

    kgmGui::Rect rc = item->getRect(i);

    rc.x += prect.x;
    rc.y += prect.y;

    if(item->getSelected() == i)
    {
      if(citem->getType() == kgmGuiMenu::Item::TypeMenu)
        renderGuiMenuItem(menu, citem);

      gcDrawRect(rc, gui_style->smenu.fg_color, gui_style->smenu.image);
    }
    else
    {
      gcDrawRect(rc, gui_style->smenu.bg_color, gui_style->smenu.image);
    }

    kgmString title = citem->getTitle();

    float asp = (float)m_viewport.width() / (float)m_viewport.height();
    gcDrawText(font, 8, 19, gui_style->smenu.tx_color, rc, title);
    //gcDrawText(font, 10, 10, 0xff000000, rc, title);
  }
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
  setProjMatrix(g_mtx_proj);
  setViewMatrix(g_mtx_view);
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

  if(tex && tex->m_texture)
    gc->gcSetTexture(0, tex->m_texture);
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

  if(!font || !font->m_texture)
    return;

  float tx = 0.0f, ty = 0.0f;
  float tdx = (float)(1.f / font->m_cols),
      tdy = (float)(1.f / font->m_rows);

  float cx = (float)clip.x, cy = (float)clip.y;

  //gc->gcBlend(true, gcblend_one, gcblend_one);
  gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);
  gc->gcSetTexture(0, font->m_texture);

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

    tx = (float)(tdx * (ch % font->m_rows));
    ty = 1.0f - (float)(tdy * (ch / font->m_rows));

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
  if(shaders.hasKey(shader))
    return shaders[shader];

  return shaders[0];
}
