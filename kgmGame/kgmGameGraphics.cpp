/*
 * kgmGameRenderer.cpp
 *
 *  Created on: May 27, 2012
 *      Author: goga
 */

#include "kgmGameGraphics.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmTime.h"
#include "../kgmGraphics/kgmGuiProgress.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGuiScroll.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiTab.h"

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

mtx4       g_mtx_proj;
mtx4       g_mtx_view;
mtx4       g_mtx_world;
mtx4*      g_mtx_joints = null;

u32        g_mtx_joints_count = 0;

vec4       g_vec_light   = vec4(0, 0, 0, 1);
vec4       g_vec_ambient = vec4(1, 1, 1, 1);

kgmShader* g_shd_active = null;

void*      g_tex_black = null;
void*      g_tex_white = null;

kgmGameGraphics::kgmGameGraphics(kgmIGraphics *g, kgmIResources* r){
  gc = g;
  rc = r;

  m_has_shaders = false;
  m_has_buffers = false;
  m_alpha       = false;
  m_culling     = true;

  gui_style = new kgmGuiStyle();

  linkCamera(null, 0, 0);

  if(g)
  {
    int val;

    char txd[16] = {0};

    if(!gc)
      return;

    //generic black texture
    memset(txd, 0x00, sizeof(txd));
    g_tex_black = gc->gcGenTexture(txd, 2, 2, 32, 0);

    //generic white texture
    memset(txd, 0xffffffff, sizeof(txd));
    g_tex_white = gc->gcGenTexture(txd, 2, 2, 32, 0);


    g->gcGetParameter(gcsup_shaders, &val);

    if(val != 0)
      m_has_shaders = true;

    g->gcGetParameter(gcsup_fbuffers, &val);

    if(val != 0)
      m_has_buffers = true;
  }

  m_has_shaders = false;

  if(m_has_shaders)
  {
    kgmVisual::AnimateVertices = false;

    if(rc != null)
    {
      shaders.add(kgmMaterial::ShaderNone, rc->getShader("none.glsl"));
      shaders.add(kgmMaterial::ShaderBase, rc->getShader("base.glsl"));
      shaders.add(kgmMaterial::ShaderSkin, rc->getShader("skin.glsl"));
    }
  }
}

kgmGameGraphics::~kgmGameGraphics()
{
  if(g_tex_black)
    gc->gcFreeTexture(g_tex_black);

  if(g_tex_white)
    gc->gcFreeTexture(g_tex_white);

  gui_style->release();
  m_guis.clear();
  clear();
}

void kgmGameGraphics::clear(){
  for(int i = 0; i < m_meshes.length(); i++)
  {
    kgmMesh* msh = null;
    kgmMaterial* mtl = null;

    m_meshes.get(i, msh, mtl);

    if(msh) msh->release();
    if(mtl) mtl->release();
  }
  m_meshes.clear();

  for(int i = 0; i < m_materials.size(); i++){
    m_materials[i]->release();
  }
  m_materials.clear();

  for(int i = 0; i < m_visuals.size(); i++){
    m_visuals[i]->release();
  }
  m_visuals.clear();

  for(int i = 0; i < m_vis_text.size(); i++){
    m_vis_text[i]->release();
  }
  m_vis_text.clear();

  for(int i = 0; i < m_vis_blend.size(); i++){
    m_vis_blend[i]->release();
  }
  m_vis_blend.clear();

  for(int i = 0; i < m_vis_sprite.size(); i++){
    m_vis_sprite[i]->release();
  }
  m_vis_sprite.clear();

  for(int i = 0; i < m_vis_particles.size(); i++){
    m_vis_particles[i]->release();
  }
  m_vis_particles.clear();

  for(int i = 0; i < m_lights.size(); i++){
    m_lights[i]->release();
  }
  m_lights.clear();

#ifdef TEST
  for(int i = 0; i < m_bodies.size(); i++)
  {
    m_bodies[i]->release();
  }
  m_bodies.clear();
#endif

  linkCamera(null, 0, 0);
}

kgmShader* s_def = null;
void kgmGameGraphics::build(){
}

void kgmGameGraphics::setDefaultFont(kgmFont* f){
  font = f;
  gui_style->gui_font = f;
}

void kgmGameGraphics::setGuiStyle(kgmGuiStyle* s){
  if(!s)
    return;

  if(gui_style)
    gui_style->release();

  gui_style = s;
}

void kgmGameGraphics::setProjMatrix(mtx4 &m)
{
  g_mtx_proj = m;
  gc->gcSetMatrix(gcmtx_proj, m.m);
}

void kgmGameGraphics::setViewMatrix(mtx4 &m)
{
  g_mtx_view = m;
  gc->gcSetMatrix(gcmtx_view, m.m);
}

void kgmGameGraphics::setWorldMatrix(mtx4 &m)
{
  g_mtx_world = m;

  mtx4 mw = m * g_mtx_view;
  gc->gcSetMatrix(gcmtx_view, mw.m);
}

void kgmGameGraphics::resize(float width, float height){
  gc->gcSetViewport(0, 0, width, height, 1.0, 10000.0);
  m_camera.camera.set(PI / 2, width / height, .1f, 10000.0,
                      m_camera.camera.mPos,
                      m_camera.camera.mDir,
                      m_camera.camera.mUp);
}

void kgmGameGraphics::render(){
  static float alpha = 0.0;
  static float m_time[4];
  s32 rect[4];
  vec3 v[2];
  mtx4 m;
  vec4 myvar;
  int i = 0;
  bool lighting = false;

  mtx4 mvw, mpr;

  //kgmMaterial           mbase;
  kgmList<kgmMesh*>     meshes;
  kgmList<kgmVisual*>   vis_mesh, vis_blend, vis_particles;

  // parse visible objects
  for(kgmList<kgmVisual*>::iterator i = m_visuals.begin(); i != m_visuals.end(); i.next())
  {
    if((*i)->removed())
    {
      (*i)->release();
      i = m_visuals.erase(i);

      continue;
    }
    else if(!(*i)->valid())
    {
      continue;
    }
    else if((*i)->m_visuals.length() > 0)
    {
      vis_mesh.add((*i));
    }
  }

  for(kgmList<kgmVisual*>::iterator i = m_vis_blend.begin(); i != m_vis_blend.end(); ++i)
  {
    if((*i)->removed())
    {
      (*i)->release();
      m_vis_blend.erase(i);

      continue;
    }
    else if((*i)->valid() && (*i)->m_visuals.length() > 0)
    {
      vis_blend.add((*i));
    }
  }

  for(kgmList<kgmVisual*>::iterator i = m_vis_particles.begin(); i != m_vis_particles.end(); ++i)
  {
    if((*i)->removed())
    {
      (*i)->release();
     i =  m_vis_particles.erase(i);
    }
    else if((*i)->valid())
    {
      vis_particles.add(*i);
    }
  }

  for(kgmList<kgmVisual*>::iterator i = m_vis_text.begin(); i != m_vis_text.end(); ++i)
  {
    if((*i)->removed())
    {
      (*i)->release();
      i = m_vis_text.erase(i);
    }
  }
  //---

  gc->gcCull(1);

  setProjMatrix(m_camera.camera.mProj);
  setViewMatrix(m_camera.camera.mView);
  g_mtx_world.identity();

  gc->gcBegin();
  gc->gcDepth(true, 1, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, 0xFF000000, 1, 0);
  //gc->gcClear(gcflag_color | gcflag_depth, 0xFF777777, 1, 0);

#ifdef TEST
  //Grid
  Vert lines[] = {{{0, 0, 0}, 0xff0000ff},   {{1000, 0, 0}, 0xff0000ff},
                  {{0, 0, 0}, 0xff00ff00},   {{0, 1000, 0}, 0xff00ff00},
                  {{0, 0, 0}, 0xffff0000},   {{0, 0, 1000}, 0xffff0000}};
  gc->gcDraw(gcpmt_lines, gcv_xyz | gcv_col, sizeof(Vert), 6, lines, 0, 0, null);
#endif

  if(this->m_lights.size() > 0)
  {
    gc->gcSetParameter(gcpar_lighting, (void*)true);
    lighting = true;

    for(int i = 0; i < m_lights.size(); i++)
    {
      kgmLight* l = m_lights[i];

      if(l)
      {
        gc->gcSetLight(i, (float*)&l->position, l->intensity);
        g_vec_light = vec4(l->position.x, l->position.y,
                           l->position.z, l->intensity);
      }
    }
  }

  //render 3D
  for(int i = 0; i < m_meshes.length(); i++)
  {
    kgmMesh* msh = null;
    kgmMaterial* mtl = null;

    if(m_meshes.get(i, msh, mtl))
    {
      if(mtl)
        render(mtl);

      if(msh)
        render(msh);

      render((kgmMaterial*)null);
    }
  }

  for(int i = vis_mesh.size(); i > 0;  i--)
  {
    render(vis_mesh[i - 1]);
  }

  if(lighting)
  {
    gc->gcSetParameter(gcpar_lighting, null);
    gc->gcSetLight(-1, null, 0.0);
    gc->gcSetLight(-2, null, 0.0);
    gc->gcSetLight(-3, null, 0.0);
    gc->gcSetLight(-4, null, 0.0);
    gc->gcSetLight(-5, null, 0.0);
    gc->gcSetLight(-6, null, 0.0);
    gc->gcSetLight(-7, null, 0.0);
    gc->gcSetLight(-8, null, 0.0);
    gc->gcSetLight(-9, null, 0.0);

    lighting = false;
  }

  for(int i = vis_blend.size(); i > 0;  i--)
  {
    render(vis_blend[i - 1]);
  }

  for(int i = 0; i < vis_particles.size(); i++)
  {
    kgmParticles* ptcl = vis_particles[i]->getParticles();

    setWorldMatrix(vis_particles[i]->m_transform);

    if(ptcl)
    {
      render(ptcl->m_material);
      render(vis_particles[i]->getParticles());
      render((kgmMaterial*)null);
      vis_particles[i]->update();
    }
  }
  gc->gcCull(gccull_back);

#ifdef TEST
  setViewMatrix(m_camera.camera.mView);

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

    obox3 ob = body->getOBox();
    ob.points(vec_points);

    gc->gcDraw(gcpmt_lines, gcv_xyz, sizeof(vec3), 8, vec_points, 2, 24, lines);
  }
#endif

  //For last step draw gui
  gc->gcSetShader(null);
  gc->gcDepth(false, 0, 0);
  gc->gc2DMode();

  for(kgmList<kgmVisual*>::iterator i = m_vis_sprite.begin(); i != m_vis_sprite.end(); ++i)
  {
    if((*i)->removed())
    {
      (*i)->release();

      i = m_vis_sprite.erase(i);
    }
    else if((*i)->valid())
    {
      render((*i)->getSprite());
    }
  }

  // render guis
  for(int i = 0; i < m_guis.size(); i++)
  {
    if(m_guis[i]->m_view)
    {
      render(m_guis[i]);
    }
  }
  //---

  //render text's
  for(int i = 0; i < m_vis_text.size(); i++)
  {
    kgmText* text = m_vis_text[i]->getText();
    kgmGui::Rect rc(text->m_rect.x, text->m_rect.y,
                    text->m_rect.w, text->m_rect.h);
    gcDrawText(font, 10, 15, text->m_color, rc, text->m_text);
  }
  //---

#ifdef TEST
  char info[4096] = {0};
  sprintf(info, "camera direction: %f %f %f \ncamera position: %f %f %f \nobject count: %i\n",
          m_camera.camera.mDir.x, m_camera.camera.mDir.y, m_camera.camera.mDir.z,
          m_camera.camera.mPos.x, m_camera.camera.mPos.y, m_camera.camera.mPos.z,
          kgmObject::objectCount());
  kgmString text(info);
  gcDrawText(font, 10, 15, 0xffffffff, kgmGui::Rect(1, 400, 600, 200), text);
#endif

  gc->gc3DMode();

  gc->gcEnd();

  gc->gcRender();

  //clear&reset
  gc->gcSetTexture(0, 0);
  gc->gcSetTexture(1, 0);
  gc->gcSetTexture(2, 0);
  gc->gcSetTexture(3, 0);

  vis_particles.clear();
  vis_blend.clear();
  vis_mesh.clear();
}

void kgmGameGraphics::render(kgmVisual* visual)
{
  if(!visual)
    return;

  mtx4 tr = visual->m_transform * m_camera.camera.mView;
  setWorldMatrix(visual->m_transform);

  if(visual->m_tm_joints)
  {
    g_mtx_joints       = visual->m_tm_joints;
    g_mtx_joints_count = visual->m_skeleton->m_joints.size();
  }

  switch(visual->m_typerender)
  {
  case kgmVisual::RenderNone:
  case kgmVisual::RenderMesh:
  {
    for(int i = 0; i < visual->m_visuals.size(); i++)
    {
      kgmVisual::Visual* v = visual->m_visuals[i];

      kgmMaterial* mtl = v->getMaterial();
      render(mtl);

      if(visual->m_tm_joints)
        render((kgmShader*)this->shaders[2]);
      else if(mtl && mtl->m_shader == kgmMaterial::ShaderNone && shaders.length() > 0)
        render((kgmShader*)this->shaders[0]);
      else if(shaders.length() > 1)
        render((kgmShader*)this->shaders[1]);
      else
        render((kgmShader*)null);

      // /*
      u32  pmt;

      switch(v->getRenderType())
      {
      case kgmMesh::RT_LINE:
        pmt = gcpmt_lines;
        break;
      case kgmMesh::RT_POINT:
//        render((kgmShader*)null);
//        render((kgmMaterial*)null);
        pmt = gcpmt_points;
        break;
      default:
        pmt = gcpmt_triangles;
      };

      gc->gcDraw(pmt, v->getFvf(),
                 v->getVsize(), v->getVcount(), v->getVertices(),
                 2, 3 * v->getFcount(), v->getFaces());
      // */
      render((kgmShader*)null);
      render((kgmMaterial*)null);
    }

    g_mtx_joints       = null;
    g_mtx_joints_count = 0;
  }
    break;
  case kgmVisual::RenderText:
  {
    kgmText* text = visual->getText();
    kgmGui::Rect rc(text->m_rect.x, text->m_rect.y,
                    text->m_rect.w, text->m_rect.h);
    gcDrawText(font, text->m_size / 2, text->m_size, text->m_color, rc, text->m_text);
  }
    break;
  }

  visual->update();

#ifdef TEST
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

void kgmGameGraphics::render(kgmSprite* sprite)
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

void kgmGameGraphics::render(kgmParticles* particles)
{
#define MAX_PARTICLES 100
  struct PrPoint{ vec3 pos; u32 col; vec2 uv; };

  if(!particles)
    return;

  mtx4    mtr = g_mtx_view;//g_mtx_world * g_mtx_view;
  vec3    rv, uv;
  float2  dim(1, 1);


  //rv = vec3(m_camera.camera.mView.m[0], m_camera.camera.mView.m[1], m_camera.camera.mView.m[2]);
  rv = vec3(mtr.m[0], mtr.m[2], mtr.m[1]);
  rv.normalize();
  //uv = vec3(m_camera.camera.mView.m[4], m_camera.camera.mView.m[5], m_camera.camera.mView.m[6]);
  uv = rv.cross(m_camera.camera.mDir);//vec3(mtr.m[4], mtr.m[5], mtr.m[6]);
  uv.normalize();

  PrPoint       points[MAX_PARTICLES][6];
  s32           count;

  if(particles)
  {
    count = (particles->m_count > MAX_PARTICLES) ? (MAX_PARTICLES) : (particles->m_count);

    for(s32 i = 0; i < count; i++)
    {
      PrPoint v[4];
      vec3    pos   = particles->m_particles[i].pos;
      float   scale = particles->m_particles[i].scale;

      /*
      float scale = particles->m_particles[i].scale;
      points[i][0].v = particles->m_particles[i].pos + vec3(-scale, 0, -scale);
      points[i][1].v = particles->m_particles[i].pos + vec3( scale, 0, -scale);
      points[i][2].v = particles->m_particles[i].pos + vec3(-scale, 0,  scale);
      points[i][3].v = particles->m_particles[i].pos + vec3(-scale, 0,  scale);
      points[i][4].v = particles->m_particles[i].pos + vec3( scale, 0, -scale);
      points[i][5].v = particles->m_particles[i].pos + vec3( scale, 0,  scale);
      */
      v[0].pos = (pos - rv + uv) * scale;
      v[0].uv.x = 0.0f, v[0].uv.y = 0.0f;
      v[1].pos = (pos - rv - uv) * scale;
      v[1].uv.x = 0.0f, v[1].uv.y = 1.0f;
      v[2].pos = (pos + rv + uv) * scale;
      v[2].uv.x = 1.0f, v[2].uv.y = 0.0f;
      v[3].pos = (pos + rv - uv) * scale;
      v[3].uv.x = 1.0f, v[3].uv.y = 1.0f;

      v[0].col = v[1].col = v[2].col = v[3].col = particles->m_particles[i].col.color;

      gc->gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(PrPoint), 4, v, 0, 0, 0);
    }

    //gc->gcDraw(gcpmt_triangles, gcv_xyz | gcv_col | gcv_uv0,
    //           sizeof(PrPoint), 6 * count, particles, 0, 0, 0);
  }
}

void kgmGameGraphics::render(kgmMaterial* m){

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
      gc->gcDepth(true, true, gccmp_less);
      gc->gcBlend(false, gcblend_srcalpha, gcblend_one);
      m_alpha = false;
    }

    return;
  }

  if(m->m_blend)
  {
    gc->gcDepth(true, false, gccmp_less);
    //gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);
    //gc->gcBlend(true, gcblend_srcialpha, gcblend_zero);
    gc->gcBlend(true, m->m_srcblend, m->m_dstblend);
    m_alpha = true;
  }

  if(m->m_2side)
  {
    gc->gcCull(0);
    m_culling = false;
  }

  if(m->m_tex_color){
    gc->gcSetTexture(0, m->m_tex_color->m_texture);
    tcolor = m->m_tex_color->m_texture;
  }
  else
  {
    gc->gcSetTexture(0, g_tex_white);
    tcolor = g_tex_white;
  }

  if(m->m_tex_normal){
    gc->gcSetTexture(1, m->m_tex_normal->m_texture);
    tnormal = m->m_tex_normal->m_texture;
  }
  else
  {
    gc->gcSetTexture(1, g_tex_black);
    tnormal = g_tex_black;
  }

  if(m->m_tex_specular){
    gc->gcSetTexture(2, m->m_tex_specular->m_texture);
    tspecular = m->m_tex_specular->m_texture;
  }
  else
  {
    gc->gcSetTexture(2, g_tex_black);
    tspecular = g_tex_black;
  }

  if(m->m_shader == kgmMaterial::ShaderNone)
  {
    render((kgmShader*)null);
  }
}

void kgmGameGraphics::render(kgmShader* s){
  //send default parameters

  if(s)
  {
    s->attr(0, "g_Vertex");
    s->attr(1, "g_Normal");
    s->attr(2, "g_Texcoord");

    s->start();
    s->set("g_fTime",     kgmTime::getTime());
    s->set("g_fRandom",   (float)rand()/(float)RAND_MAX);
    s->set("g_mProj",     g_mtx_proj);
    s->set("g_mView",     g_mtx_view);
    s->set("g_mTran",     g_mtx_world);
    s->set("g_vAmbient",  g_vec_ambient);
    s->set("g_vLight",    g_vec_light);
    s->set("g_vEye",      m_camera.camera.mPos);
    s->set("g_vEyeDir",   m_camera.camera.mDir);

    if(tcolor)
      s->set("g_txColor", 0);

    if(tnormal)
      s->set("g_txNormal", 1);

    if(tspecular)
      s->set("g_txSpecular", 2);

    if(g_mtx_joints)
    {
      for(int i = 0; i < g_mtx_joints_count; i++)
      {
        char buf[16];

        sprintf(buf, "g_mJoints[%i]\0", i);
        s->set(buf, g_mtx_joints[i]);
      }
    }

    g_shd_active = s;
  }
  else
  {
    gc->gcSetShader(null);
    g_shd_active = null;
  }
}

void kgmGameGraphics::render(kgmMesh *m){
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

void kgmGameGraphics::render(kgmGui* gui){
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

    if(gui->m_useStyle)
    {
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
    }
    else
    {
      gcDrawRect(rect, gui->m_color, gui->m_image);
    }

    if(text.length() > 0)
      gcDrawText(gui_style->gui_font, fwidth, fheight, 0xFFFFFFFF, tClip, text);
  }
  else if(gui->isClass(kgmGuiScroll::Class))
  {
  }
  else if(gui->isClass(kgmGuiList::Class))
  {
    u32 fontHeight = ((kgmGuiList*)gui)->m_itemHeight - 2;
    u32 item_cnt = ((kgmGuiList*)gui)->m_items.size();
    u32 item_view = ((kgmGuiList*)gui)->m_itemHeight;
    kgmGui::Rect frect;

    //Draw Main Rect
    gcDrawRect(rect, gui_style->slist.bg_color, gui_style->slist.image);

    //Draw Focused Rect
    if((((kgmGuiList*)gui)->m_itemSel >= 0) &&
       (((kgmGuiList*)gui)->m_itemSel < ((kgmGuiList*)gui)->m_items.size()))
      gcDrawRect(kgmGui::Rect(rect.x, rect.y + ((kgmGuiList*)gui)->m_itemSel * ((kgmGuiList*)gui)->m_itemHeight,
                              rect.w, ((kgmGuiList*)gui)->m_itemHeight), gui_style->sbutton.fg_color, gui_style->slist.image);
    //Draw Items Rects
    for(int i = ((kgmGuiList*)gui)->m_position; i < (((kgmGuiList*)gui)->m_position + item_view); i++){
      if(i >= item_cnt) continue;
      kgmString item; item = ((kgmGuiList*)gui)->m_items[i];
      u32 a = (i - ((kgmGuiList*)gui)->m_position);
      kgmGui::Rect clip(rect.x, rect.y + ((kgmGuiList*)gui)->m_itemHeight * a,
                        rect.w, ((kgmGuiList*)gui)->m_itemHeight);

      clip.h ++;
      gcDrawText(gui_style->gui_font, clip.height() / 2, clip.height(), 0xFFFFFFFF, clip, item);
    }
  }
  else if(gui->isClass(kgmGuiTab::Class))
  {
    render((kgmGuiTab*)gui);
  }
  else if(gui->isClass(kgmGuiProgress::Class))
  {
    render((kgmGuiProgress*)gui);
  }
  else if(gui->isType(kgmGui::Class))
  {
    if(gui->m_hasMouse )
    {
      if(gui->m_useStyle)
      {
        gcDrawRect(rect, gui_style->sgui.fg_color, gui_style->sgui.image);
      }
      else
      {
        gcDrawRect(rect, gui->m_color, gui->m_image);
      }
    }
    else
    {
      if(gui->m_useStyle)
      {
        gcDrawRect(rect, gui_style->sgui.bg_color, gui_style->sgui.image);
      }
      else
      {
        gcDrawRect(rect, gui->m_color, gui->m_image);
      }
    }
  }

  if(gui->m_hasAlpha)
    gc->gcBlend(false, gcblend_zero, gcblend_zero);

  for(int i = 0; i < gui->m_childs.length(); i++){
    if(gui->m_childs[i]->m_view)
      render(gui->m_childs[i]);
  }
}

s32 kgmGameGraphics::getShaderId(kgmString s)
{
  return -1;
}

//*************** DRAWING ***************
void kgmGameGraphics::gcDrawRect(kgmGui::Rect rc, u32 col, kgmTexture* tex){
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;
  V v[4];

//  if(tex && tex->m_texture)
//    col = 0x00FFFFFF;

  v[0].pos = vec3(rc.x,        rc.y,        0); v[0].col = col; v[0].uv = vec2(0, 1);
  v[1].pos = vec3(rc.x,        rc.y + rc.h, 0); v[1].col = col; v[1].uv = vec2(0, 0);
  v[2].pos = vec3(rc.x + rc.w, rc.y,        0); v[2].col = col; v[2].uv = vec2(1, 1);
  v[3].pos = vec3(rc.x + rc.w, rc.y + rc.h, 0); v[3].col = col; v[3].uv = vec2(1, 0);

  if(tex && tex->m_texture)
    gc->gcSetTexture(0, tex->m_texture);

  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);

  if(tex)
    gc->gcSetTexture(0, 0);
}

void kgmGameGraphics::gcDrawText(kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor, kgmGui::Rect clip, kgmString& text){
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

  gc->gcBlend(true, gcblend_one, gcblend_one);
  gc->gcSetTexture(0, font->m_texture);

  for(u32 i = 0; i < tlen; i++){
    char ch = text[i];

    if(ch == '\n'){ cx = (float)clip.x, cy += fheight; continue; }
    if((ch == ' ') || (ch == '\t')){  tx = 0.0f, ty = 0.0f;	}

    tx = (float)(tdx * (ch % font->m_rows));
    ty = 1.0f - (float)(tdy * (ch / font->m_rows));

    if(clip.inside(cx + fwidth, cy + fheight))
    {
      v[0].pos = vec3(cx, cy, 0),    v[0].col = fcolor, v[0].uv = vec2(tx, ty);
      v[1].pos = vec3(cx, cy+fheight, 0),  v[1].col = fcolor, v[1].uv = vec2(tx, ty-tdy);
      v[2].pos = vec3(cx+fwidth, cy, 0),  v[2].col = fcolor, v[2].uv = vec2(tx+tdx-0.001, ty);
      v[3].pos = vec3(cx+fwidth, cy+fheight, 0),v[3].col = fcolor, v[3].uv = vec2(tx+tdx-0.001, ty-tdy);
      gc->gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
    }
    else
    {
#ifdef TEST
      kgm_log() << "\nText: " << (char*)text << " " << "Not clipped H,W " << (s32)fheight << " " << (s32)fwidth << " end!";
#endif
    }

    cx += fwidth;
  }

  gc->gcBlend(0, 0, 0);
  gc->gcSetTexture(0, 0);
}

void kgmGameGraphics::gcDrawBillboard(box b, u32 col){
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
  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
}

