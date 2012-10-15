#include "kgmGraphics.h"
#include "kgmGui.h"
#include "kgmMesh.h"
#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "kgmShader.h"
#include "kgmAnimation.h"
#include "kgmSkeleton.h"
#include "kgmFrustum.h"
#include "kgmCamera.h"

#include "../kgmMath/kgmBase.h"
#include "../kgmBase/kgmTime.h"

#include <stdlib.h>

////////////////////
KGMOBJECT_IMPLEMENT(kgmFrustum,		kgmObject);
KGMOBJECT_IMPLEMENT(kgmCamera,		kgmFrustum);

const u32  m_mapdim = 512;
const float m_lfov = DEGTORAD(190.0);//default 120

uchar     g_azero[] = {0x00, 0x00, 0x00, 0x00};
uchar     g_afull[] = {0xFF, 0xFF, 0xFF, 0xFF};

void*  fbo_depthmapcamera = 0;			//depth map in camera space
void*  fbo_lightmapcamera = 0;			//light map in camera space
void*  fbo_colormapcamera = 0;			//color map in camera space
void*  fbo_normalmapcamera = 0;			//color map in camera space
void*  fbo_depthmaplight = 0;			//shadow map in light space


kgmGraphics::kgmGraphics(kgmWindow* w, kgmIResources* r)
{
  gc = w->getGC();
  wnd = w;
  rc = r;
  font = 0;
  
  node = new kgmNode();

  fwidth = 800;
  fheight = 600;
  zfar = 100000.0;
  znear = 1.0;

  camera = 0;

  //create nessessary textures
  char txd[16] = {0};
  //generic black texture
  memset(txd, 0x00, sizeof(txd));
  textures.add((u16)0, gc->gcGenTexture(txd, 2, 2, 32, 0) );
  //generic white texture
  memset(txd, 0xffffffff, sizeof(txd));
  textures.add((u16)1, gc->gcGenTexture(txd, 2, 2, 32, 0) );

  //create nessessary shaders
  registration(kgmMaterial::ShaderNone, rc->getShader((char*)"light.glsl"));

  //fbo_lightmapcamera  = gc->gcGenTexture(0, m_mapdim, m_mapdim, gctex_fmt24, gctype_textarget);//gc->gcGenRenderTarget(m_mapdim, m_mapdim, 0x01);
  //fbo_colormapcamera  = gc->gcGenRenderTarget(m_mapdim, m_mapdim, 0x01);
  //fbo_normalmapcamera = gc->gcGenRenderTarget(m_mapdim, m_mapdim, 0x01);
  //fbo_depthmapcamera  = gc->gcGenRenderTarget(m_mapdim, m_mapdim, 0x02);
  //fbo_depthmaplight   = gc->gcGenRenderTarget(m_mapdim, m_mapdim, 0x02);

  fbo_lightmapcamera  = gc->gcGenTexture(0, m_mapdim, m_mapdim, gctex_fmt24, gctype_textarget);
  fbo_colormapcamera  = gc->gcGenTexture(0, m_mapdim, m_mapdim, gctex_fmt24, gctype_textarget);
  fbo_depthmapcamera  = gc->gcGenTexture(0, m_mapdim, m_mapdim, gctex_fmtdepth, gctype_textarget);

  //gui
  gui_style = new kgmGuiStyle();

  //mainCamera
  
}

kgmGraphics::~kgmGraphics()
{
  if(node){
    delete node;
  }
  
  for(int i = 0; i < textures.length(); i++){
    u16 k;
    void* v;
    textures.get(i, k, v);
    gc->gcFreeTexture(v);
  }

  for(int i = 0; i < shaders.length(); i++){
    u16 k;
    kgmShader* v;
    shaders.get(i, k, v);
    rc->remove(v);
  }

  if(gui_style){
    gui_style->release();
  }

  gc->gcFreeTexture(fbo_depthmapcamera);
  gc->gcFreeTexture(fbo_lightmapcamera);
  gc->gcFreeTexture(fbo_colormapcamera);
  gc->gcFreeTexture(fbo_normalmapcamera);
  gc->gcFreeTexture(fbo_depthmaplight);

  guis.clear();
  shaders.clear();
  textures.clear();
}

void kgmGraphics::build()
{

}

void kgmGraphics::setActiveCamera(kgmCamera* c){
  camera = c;
}

void kgmGraphics::setDefaultFont(kgmFont* f){
  font = f;  
  gui_style->gui_font = f; //r->getFont((char*)"arial.tga", 16, 16);
}

kgmLight*          c_light = 0;   //drop shadows
kgmCamera*         c_camera = 0;  //active camera
kgmList<kgmLight*> c_lights;  //usual lights
kgmList<kgmMesh*>  c_meshes;  //statuc meshes

inline void collect(kgmNode* n){
  if(!n)
    return;
  for(int i = 0; i < n->count(); i++){
    kgmNode* node = n->node(i);
      if(node->type == kgmNode::NodeCamera && !c_camera){
          c_camera = node->camera;
      }else if(node->type == kgmNode::NodeLight){

      }else if(node->type == kgmNode::NodeMesh){

      }
  }
}

void kgmGraphics::render()
{
  static float alpha = 0.0;
  static float m_time[4];
  s32 rect[4];
  vec3 v[2];
  mtx4 m;
  vec4 myvar;
  int i = 0;

  mtx4 mvw, mpr;
  int  rc[4];

  kgmMaterial mbase;
  
  wnd->getRect(rect[0], rect[1], rect[2], rect[3]);
  gc->gcSetViewport(0, 0, rect[2], rect[3], 1.0, 1000.0);
  gc->gcClear(gcflag_color | gcflag_depth, 0xFF555555, 1.0, 0);
  gc->gcCull(0);
  
  /*if(camera){
     //gc->gcSetMatrix(gcmtx_proj, camera->mVwPj.m);
     gc->gcSetMatrix(gcmtx_proj, camera->mProj.m);
     gc->gcSetMatrix(gcmtx_view, camera->mView.m);
  }else{
     mtx4 mvw, mpr;
     mvw.identity();
     mpr.identity();
     //mvw.lookat(vec3(0, 0, 100 * sin(k)), 
     //           vec3(0, 0, -100 * sin(k)), 
     //           vec3(0, 0, 1));
     gc->gcSetMatrix(gcmtx_proj, mpr.m);
     gc->gcSetMatrix(gcmtx_view, mvw.m);
     }*/

  gc->gcSetMatrix(gcmtx_proj, mcamera.mProj.m);
  gc->gcSetMatrix(gcmtx_view, mcamera.mView.m);

  gc->gcBegin();

  //render 3D
  render(node);
  
  //For last step draw gui
  gc->gc2DMode();
  for(int i = 0; i < guis.size(); i++)
  {
    if(guis[i]->m_view)
    {
      render (guis[i]);
    }
  }

  gc->gc3DMode();
  gc->gcEnd();
  gc->gcRender();

  //clear&reset
  gc->gcSetShader(0);
  gc->gcSetTexture(0, 0);
  gc->gcSetTexture(1, 0);
  gc->gcSetTexture(2, 0);
  gc->gcSetTexture(3, 0);
}

void kgmGraphics::render(kgmNode* n)
{
  if(!n)
    return;

  switch(n->type){
  case kgmNode::NodeCamera:
    if(n->camera){
      mtx4 mvw;
      mvw.identity();
      //gc->gcSetMatrix(gcmtx_proj, ((NodeCamera*)n)->camera->mVwPj.m);
      gc->gcSetMatrix(gcmtx_proj, n->camera->mProj.m);
      gc->gcSetMatrix(gcmtx_view, n->camera->mView.m);
      //gc->gcSetMatrix(gcmtx_view, mvw.m);
      camera = n->camera;
    }else{
      mtx4 mvw, mpr;
      mvw.identity();
      mpr.identity();
      gc->gcSetMatrix(gcmtx_proj, mpr.m);
      gc->gcSetMatrix(gcmtx_view, mvw.m);
    }
    break;
  case kgmNode::NodeMaterial:
    set(n->material);
    break;
  case kgmNode::NodeMesh:
    gc->gcDraw(gcpmt_triangles, n->mesh->fvf(), n->mesh->vsize(),
               n->mesh->vcount(), n->mesh->vertices(),
               2, 3 * n->mesh->fcount(),  n->mesh->faces());
    printf("Draw mesh: \n");
    break;
  default:
    break;
  }

  for(int i = 0; i < n->count(); i++)
    render(n->node(i));
}

void kgmGraphics::render(kgmGui* gui){
  kgmGui::Rect rect;
  kgmString text;
  
  if(!gui)
    return;

  gui->getRect(rect, true);
  gui->getText(text);
  
  if(gui->isClass(kgmGuiButton::Class)){
    render((kgmGuiButton*)gui);
  }else if(gui->isClass(kgmGuiScroll::Class)){
    render((kgmGuiScroll*)gui);
  }else if(gui->isClass(kgmGuiList::Class)){
    render((kgmGuiList*)gui);
  }else if(gui->isClass(kgmGuiTab::Class)){
    render((kgmGuiTab*)gui);
  }else if(gui->isClass(kgmGuiProgress::Class)){
    render((kgmGuiProgress*)gui);
  }else if(gui->isClass(kgmGui::Class)){
    //gcDrawRect(rect, gui_style->sgui.bg_color, gui_style->gui_font->m_texture);
    if(gui->m_hasMouse ){
      gcDrawRect(rect, gui_style->sgui.fg_color, 0);
    }else{
      gcDrawRect(rect, gui_style->sgui.bg_color, 0);
    }  
  }
  
  for(int i = 0; i < gui->m_childs.length(); i++){
      if(gui->m_childs[i]->m_view)
        render(gui->m_childs[i]);
  }
}

void kgmGraphics::render(kgmGuiButton* gui){
  kgmGui::Rect rect;
  kgmString text;

  gui->getRect(rect, true);
  gui->getText(text);

  u32 fwidth = (u32)((float)rect.w / (float)(text.length() + 1));
  u32 fheight = (u32)((float)rect.h * (float)0.75f);
  u32 tlen = text.length();
  u32 fw = (tlen) * fwidth;
  u32 fh = fheight;
  kgmGui::Rect tClip = rect;

  tClip.x = rect.x + rect.w / 2 - fw / 2;
  tClip.y = rect.y + rect.h/ 2 - fh / 2;
  tClip.w = fw;
  tClip.h = fh;

  switch(gui->getState()){
    case kgmGuiButton::StateFocus:
    gcDrawRect(rect, gui_style->sbutton.ac_color, 0);
    break;
  case kgmGuiButton::StateClick:
    gcDrawRect(rect, gui_style->sbutton.fg_color, 0);
    break;
  case kgmGuiButton::StateNone:
  default:
    gcDrawRect(rect, gui_style->sbutton.bg_color, 0);
  }
  gcDrawText(gui_style->gui_font, fwidth, fheight, 0xFFFFFFFF, tClip, text);
}

void kgmGraphics::render(kgmGuiList* gui){
  kgmGui::Rect rect;
  kgmString text;

  gui->getRect(rect, true);
  gui->getText(text);

  u32 fontHeight = gui->m_itemHeight - 2;
  u32 item_cnt = gui->m_items.size();
  u32 item_view = gui->m_itemHeight;
  kgmGui::Rect frect;

  //Draw Main Rect
  gcDrawRect(rect, gui_style->slist.bg_color, 0);

  //Draw Focused Rect
  if((gui->m_itemSel >= 0) && (gui->m_itemSel < gui->m_items.size()))
   gcDrawRect(kgmGui::Rect(rect.x, rect.y + gui->m_itemSel * gui->m_itemHeight,
                       rect.w, gui->m_itemHeight), gui_style->sbutton.fg_color, 0);
  //Draw Items Rects
  for(int i = gui->m_position; i < (gui->m_position + item_view); i++){
   if(i >= item_cnt) continue;
   kgmString item; item = gui->m_items[i];
   u32 a = (i - gui->m_position);
   kgmGui::Rect clip(rect.x, rect.y + gui->m_itemHeight * a,
         rect.w, gui->m_itemHeight);

   clip.h ++;
   //gcDrawText(gc, getFont(), 10, m_fontHeight, m_colors[kgmGui::TEXT], clip, item);
   gcDrawText(gui_style->gui_font, 10, clip.height(), 0xFFFFFFFF, clip, item);
  }
}

void kgmGraphics::render(kgmGuiTab* gui){
  kgmGui::Rect rect;
  kgmString text;

  gui->getRect(rect);
  gui->getText(text);
}

void kgmGraphics::render(kgmGuiScroll* gui){
  kgmGui::Rect rect;
  kgmString text;

  gui->getRect(rect);
  gui->getText(text);
}

void kgmGraphics::render(kgmGuiProgress* gui){
  kgmGui::Rect rect;
  kgmString text;

  gui->getRect(rect);
  gui->getText(text);
}
void kgmGraphics::resize(int w, int h)
{
  mtx4 mvw, mpr;

  fwidth = w;
  fheight = h;
  mpr.perspective(0.25 * PI, (float)w/(float)h, 0.1, 10000000.0);
  mvw.lookat(vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1));

  gc->gcSetViewport(0, 0, w, h, znear, zfar);
  gc->gcSetMatrix(gcmtx_view, mvw.m);
  gc->gcSetMatrix(gcmtx_proj, mpr.m);
}


void kgmGraphics::add(kgmNode* n)
{
 if(!n)
   return;
 node->add(n);
}

void kgmGraphics::erase(kgmNode* n)
{
 node->erase(n);
}

void kgmGraphics::add(kgmGui* g)
{
  guis.add(g);
}

void kgmGraphics::erase(kgmGui* g)
{
  for(int i = (guis.size() - 1); i >= 0; i--)
    if(guis[i] == g)
      guis.erase(i);
}

void kgmGraphics::set(kgmMaterial* m){
  if(!m){
    gc->gcSetShader(0);
    gc->gcSetTexture(0, 0);
    gc->gcSetTexture(1, 0);
    gc->gcSetTexture(2, 0);
    gc->gcSetTexture(3, 0);
    return;
  }

  if(m->m_tex_diffuse){
    gc->gcSetTexture(0, m->m_tex_diffuse->m_texture);
    tdiffuse = m->m_tex_diffuse->m_texture;
  }

  if(m->m_tex_normal){
    gc->gcSetTexture(1, m->m_tex_normal->m_texture);
    tnormal = m->m_tex_normal->m_texture;
  }

  if(m->m_tex_specular){
    gc->gcSetTexture(1, m->m_tex_specular->m_texture);
    tspecular = m->m_tex_specular->m_texture;
  }

  kgmShader* s = shaders[(u16)m->m_shader];
  if(!s)
    s = shaders[kgmMaterial::ShaderNone];
  set(s, m->m_shader);
}

void kgmGraphics::set(kgmShader* s, u32 t){
  s->start();
  switch(t){
    default:
      s->set("g_fTime", kgmTime::getTime());
      s->set("g_fRandom", (float)rand()/(float)RAND_MAX);
      if(camera){
        s->set("g_mView", camera->mView);
        s->set("g_mProj", camera->mProj);
      }
  };
}

//*************** DRAWING ***************
void kgmGraphics::gcDrawRect(kgmGui::Rect rc, u32 col, void* tex){
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;
  V v[4];

  v[0].pos = vec3(rc.x,        rc.y,        0); v[0].col = col; v[0].uv = vec2(0, 0);
  v[1].pos = vec3(rc.x,        rc.y + rc.h, 0); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(rc.x + rc.w, rc.y,        0); v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(rc.x + rc.w, rc.y + rc.h, 0); v[3].col = col; v[3].uv = vec2(1, 1);

  if(tex)
   gc->gcSetTexture(0, tex);
  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
  if(tex)
   gc->gcSetTexture(0, 0);
}

void kgmGraphics::gcDrawText(kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor, kgmGui::Rect clip, kgmString& text){
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
   }else{
    fprintf(stderr, "\nText: %s Not clipped fheight: %i fwidth: %i", (char*)text, fheight, fwidth); 
   }

   cx += fwidth;
  }

  gc->gcBlend(0, 0, 0);
  gc->gcSetTexture(0, 0);
}

