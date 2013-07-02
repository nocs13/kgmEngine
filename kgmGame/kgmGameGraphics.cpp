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
  //m_has_shaders = false;
  if(m_has_shaders)
  {
    kgmVisual::AnimateVertices = false;

    if(rc != null){
      shaders.add(kgmMaterial::ShaderNone, rc->getShader("none.glsl"));
      shaders.add(kgmMaterial::ShaderBase, rc->getShader("base.glsl"));
      shaders.add(kgmMaterial::ShaderSkin, rc->getShader("skin.glsl"));
    }
  }
}

kgmGameGraphics::~kgmGameGraphics(){
  if(g_tex_black)
    gc->gcFreeTexture(g_tex_black);

  if(g_tex_white)
    gc->gcFreeTexture(g_tex_white);

  gui_style->release();
  m_guis.clear();
  clear();
}

void kgmGameGraphics::clear(){
  for(int i = 0; i < m_meshes.size(); i++)
    delete m_meshes[i];
  m_meshes.clear();

  for(int i = 0; i < m_materials.size(); i++){
    m_materials[i]->release();
  }
  m_materials.clear();

  for(int i = 0; i < m_visuals.size(); i++){
    m_visuals[i]->release();
  }
  m_visuals.clear();

  for(int i = 0; i < m_vis_blend.size(); i++){
    m_vis_blend[i]->release();
  }
  m_vis_blend.clear();

  for(int i = 0; i < m_lights.size(); i++){
    m_lights[i].light->release();
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

void kgmGameGraphics::resize(float width, float height){
  gc->gcSetViewport(0, 0, width, height, 1.0, 10000.0);
  m_camera.camera.set(PI / 6, width / height, .1f, 10000.0,
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

  kgmMaterial           mbase;
  kgmList<kgmMesh*>     meshes;
  kgmList<kgmVisual*>   visuals_norm,
                        visuals_blend;

  // parse visible objects
  for(kgmList<kgmVisual*>::iterator i = m_visuals.begin(); i != m_visuals.end(); i++)
  {
    if((*i)->removed())
    {
      (*i)->release();
      m_visuals.erase(i);

      continue;
    }
    else if(!(*i)->valid())
    {
      continue;
    }
    else if((*i)->m_visuals.length() > 0)
    {
      visuals_norm.add((*i));
    }
  }

  for(kgmList<kgmVisual*>::iterator i = m_vis_blend.begin(); i != m_vis_blend.end(); i++)
  {
    if((*i)->removed())
    {
      (*i)->release();
      m_vis_blend.erase(i);

      continue;
    }
    else if(!(*i)->valid())
    {
      continue;
    }
    else if((*i)->m_visuals.length() > 0)
    {
      visuals_blend.add((*i));
    }
  }

  for(kgmList<kgmVisual*>::iterator i = m_vis_text.begin(); i != m_vis_text.end(); i++)
  {
    if((*i)->removed())
    {
      (*i)->release();
      m_vis_text.erase(i);
    }
  }
  //---

  gc->gcCull(1);

  gc->gcSetMatrix(gcmtx_proj, m_camera.camera.mProj.m);
  gc->gcSetMatrix(gcmtx_view, m_camera.camera.mView.m);
  g_mtx_world.identity();

  gc->gcBegin();
  gc->gcDepth(true, 1, gccmp_lequal);
  gc->gcClear(gcflag_color | gcflag_depth, 0xFF777777, 1, 0);

  //Grid
  Vert lines[] = {{{0, 0, 0}, 0xff0000ff},   {{1000, 0, 0}, 0xff0000ff},
                  {{0, 0, 0}, 0xff00ff00},   {{0, 1000, 0}, 0xff00ff00},
                  {{0, 0, 0}, 0xffff0000},   {{0, 0, 1000}, 0xffff0000}};
  gc->gcDraw(gcpmt_lines, gcv_xyz | gcv_col, sizeof(Vert), 6, lines, 0, 0, null);


  if(this->m_lights.size() > 0)
  {
    gc->gcSetParameter(gcpar_lighting, (void*)true);
    lighting = true;

    for(int i = 0; i < m_lights.size(); i++)
    {
      Light l = m_lights[i];

      if(l.light)
      {
        gc->gcSetLight(i, (float*)&l.light->position, l.light->intensity);
        g_vec_light = vec4(l.light->position.x, l.light->position.y,
                           l.light->position.z, l.light->intensity);
      }
    }
  }

  //render 3D

  for(int i = 0; i < m_meshes.size(); i++){
    render(m_meshes[i]);
  }

  for(int i = visuals_norm.size(); i > 0;  i--){
    render(visuals_norm[i - 1]);
  }

  for(int i = visuals_blend.size(); i > 0;  i--){
    render(visuals_blend[i - 1]);
  }

#ifdef TEST
  gc->gcSetMatrix(gcmtx_view, m_camera.camera.mView.m);

  for(int i = m_bodies.size(); i > 0;  i--)
  {
    kgmBody* body = m_bodies[i - 1];

    if(!body->valid() || body->removed())
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

    body->m_bound.points(vec_points);
    body->transform(mtx_tr);

    for(int i = 0; i < 8; i++)
      vec_points[i] = mtx_tr * vec_points[i];

    gc->gcDraw(gcpmt_lines, gcv_xyz, sizeof(vec3), 8, vec_points, 2, 24, lines);
  }
#endif

  //For last step draw gui
  gc->gcSetShader(null);
  gc->gcSetLight(-1, null, 0.0);
  gc->gcSetLight(-2, null, 0.0);
  gc->gcSetLight(-3, null, 0.0);
  gc->gcSetLight(-4, null, 0.0);
  gc->gcSetLight(-5, null, 0.0);
  gc->gcSetLight(-6, null, 0.0);
  gc->gcSetLight(-7, null, 0.0);
  gc->gcSetLight(-8, null, 0.0);
  gc->gcSetLight(-9, null, 0.0);
  gc->gcDepth(false, 0, 0);
  gc->gc2DMode();

  if(lighting)
  {
    gc->gcSetParameter(gcpar_lighting, null);
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

  char info[4096] = {0};
  sprintf(info, "camera direction: %f %f %f \ncamera position: %f %f %f \nobject count: %i\n",
          m_camera.camera.mDir.x, m_camera.camera.mDir.y, m_camera.camera.mDir.z,
          m_camera.camera.mPos.x, m_camera.camera.mPos.y, m_camera.camera.mPos.z,
          kgmObject::objectCount());
  kgmString text(info);
  gcDrawText(font, 10, 15, 0xffffffff, kgmGui::Rect(1, 1, 600, 200), text);

  gc->gc3DMode();

  gc->gcEnd();

  gc->gcRender();

  //clear&reset
  gc->gcSetTexture(0, 0);
  gc->gcSetTexture(1, 0);
  gc->gcSetTexture(2, 0);
  gc->gcSetTexture(3, 0);

  visuals_blend.clear();
  visuals_norm.clear();

  for(kgmList<kgmVisual*>::iterator i = m_visuals.begin(); i != m_visuals.end(); ++i)
  {
    if((*i)->removed())
    {
      (*i)->release();
      m_visuals.erase(i);
    }
  }
}

void kgmGameGraphics::render(Mesh *m){
  if(!m)
    return;

  if(m->m_mtrl)
  {
    render(m->m_mtrl);
    render(shaders[1]);
  }

  if(m->m_mesh){
    gc->gcDraw(gcpmt_triangles, m->m_mesh->fvf(), m->m_mesh->vsize(),
               m->m_mesh->vcount(), m->getVertices(),
               2, 3 * m->m_mesh->fcount(), m->m_mesh->faces());
  }

  if(m->m_mtrl){
    render((kgmShader*)null);
    render((kgmMaterial*)null);
  }
}

void kgmGameGraphics::render(kgmVisual* visual){
  if(!visual)
    return;

  mtx4 tr = visual->m_transform * m_camera.camera.mView;
  gc->gcSetMatrix(gcmtx_view, tr.m);
  g_mtx_world      = visual->m_transform;

  if(visual->m_tm_joints)
  {
    g_mtx_joints       = visual->m_tm_joints;
    g_mtx_joints_count = visual->m_skeleton->m_joints.size();
  }

  //gc->gc2DMode();
  //gcDrawText(gui_style->gui_font, 10, 20, 0xFFFFFFFF, kgmGui::Rect(1, 10, 400, 500), visual->m_info);
  //gc->gc3DMode();

  for(int i = 0; i < visual->m_visuals.size(); i++)
  {
    kgmVisual::Visual* v = visual->m_visuals[i];

    kgmMaterial* mtl = v->getMaterial();
    render(mtl);

    if(visual->m_tm_joints)
      render((kgmShader*)this->shaders[2]);
    else if(mtl && mtl->m_shader == kgmMaterial::ShaderNone)
      render((kgmShader*)this->shaders[0]);
    else
      render((kgmShader*)this->shaders[1]);

    // /*
    gc->gcDraw(gcpmt_triangles, v->getFvf(),
               v->getVsize(), v->getVcount(), v->getVertices(),
               2, 3 * v->getFcount(), v->getFaces());
    // */
    render((kgmShader*)null);
    render((kgmMaterial*)null);
  }

  g_mtx_joints       = null;
  g_mtx_joints_count = 0;
  visual->update();

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
}

void kgmGameGraphics::render(kgmParticles* p)
{
  if(!p)
    return;
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
      //gc->gcDepth(true, true, gccmp_less);
      gc->gcAlpha(false, 0, 0);
      gc->gcBlend(false, gcblend_srcalpha, gcblend_one);
      m_alpha = false;
    }

    return;
  }

  if(m->m_transparency > 0)
  {
    gc->gcDepth(true, false, gccmp_less);
    //gc->gcBlend(true, gcblend_srcalpha, gcblend_one);
    gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);
    //gc->gcAlpha(true, gccmp_great, 1.0f - m->m_transparency);
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
    s->set("g_mView",     m_camera.camera.mView);
    s->set("g_mProj",     m_camera.camera.mProj);
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

  if(m){
    gc->gcDraw(gcpmt_triangles, m->fvf(), m->vsize(),
               m->vcount(), m->vertices(),
               2, 3 * m->fcount(), m->faces());
  }
}

/*
uniform vec4 g_vAmbient;
uniform vec4 g_vLight;
uniform vec3 g_vEye;
uniform vec3 g_vEyeLook;
*/
void kgmGameGraphics::render(kgmGui* gui){
  kgmGui::Rect rect;
  kgmString    text;

  if(!gui)
    return;

  gui->getRect(rect, true);
  text = gui->getText();

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
  else if(gui->isClass(kgmGui::Class))
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

  for(int i = 0; i < gui->m_childs.length(); i++){
    if(gui->m_childs[i]->m_view)
      render(gui->m_childs[i]);
  }
}

//*************** DRAWING ***************
void kgmGameGraphics::gcDrawRect(kgmGui::Rect rc, u32 col, kgmTexture* tex){
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;
  V v[4];

  v[0].pos = vec3(rc.x,        rc.y,        0); v[0].col = col; v[0].uv = vec2(0, 0);
  v[1].pos = vec3(rc.x,        rc.y + rc.h, 0); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(rc.x + rc.w, rc.y,        0); v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(rc.x + rc.w, rc.y + rc.h, 0); v[3].col = col; v[3].uv = vec2(1, 1);

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
      v[2].pos = vec3(cx+fwidth, cy, 0),  v[2].col = fcolor, v[2].uv = vec2(tx+tdx, ty);
      v[3].pos = vec3(cx+fwidth, cy+fheight, 0),v[3].col = fcolor, v[3].uv = vec2(tx+tdx, ty-tdy);
      gc->gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
    }
    else
    {
      kgm_log() << "\nText: " << (char*)text << " " << "Not clipped H,W " << (s32)fheight << " " << (s32)fwidth << " end!";
    }

    cx += fwidth;
  }

  gc->gcBlend(0, 0, 0);
  gc->gcSetTexture(0, 0);
}
