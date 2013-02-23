/*
 * kgmGameRenderer.cpp
 *
 *  Created on: May 27, 2012
 *      Author: goga
 */

#include "kgmGameGraphics.h"
#include "../kgmGraphics/kgmGuiProgress.h"
#include "../kgmGraphics/kgmGuiButton.h"
#include "../kgmGraphics/kgmGuiScroll.h"
#include "../kgmGraphics/kgmGuiList.h"
#include "../kgmGraphics/kgmGuiTab.h"

kgmGameGraphics::kgmGameGraphics(kgmIGraphics *g, kgmIResources* r){
  gc = g;
  rc = r;
  gui_style = new kgmGuiStyle();
}

kgmGameGraphics::~kgmGameGraphics(){
  gui_style->release();
  m_guis.clear();
  clean();
}

void kgmGameGraphics::clean(){
  for(int i = 0; i < m_meshes.size(); i++){
    //m_meshes[i].mesh->release();
    //m_meshes[i].material->release();
  }

  m_meshes.clear();
}

kgmShader* s_def = null;
void kgmGameGraphics::build(){
  if(rc != null){
    s_def = rc->getShader("base.glsl");
  }
}

void kgmGameGraphics::setDefaultFont(kgmFont* f){
  font = f;
  gui_style->gui_font = f;
}

void kgmGameGraphics::resize(float width, float height){
  //wnd->getRect(rect[0], rect[1], rect[2], rect[3]);
  gc->gcSetViewport(0, 0, width, height, 1.0, 1000.0);
}

void kgmGameGraphics::render(){
    static float alpha = 0.0;
    static float m_time[4];
    s32 rect[4];
    vec3 v[2];
    mtx4 m;
    vec4 myvar;
    int i = 0;

    mtx4 mvw, mpr;

    kgmMaterial mbase;

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

    gc->gcSetMatrix(gcmtx_proj, m_camera.mProj.m);
    gc->gcSetMatrix(gcmtx_view, m_camera.mView.m);

    gc->gcBegin();

    //render 3D
    for(int i = 0; i < m_meshes.size(); i++)
    {
      if(s_def){
        render(s_def, 0);
      }

      render(&m_meshes[i]);

      if(s_def){
        render(s_def, 1);
      }
    }

    for(int i = 0; i < m_actors.size(); i++)
    {
      render(m_actors[i]);
    }

    //For last step draw gui
    gc->gc2DMode();
    for(int i = 0; i < m_guis.size(); i++)
    {
      if(m_guis[i]->m_view)
      {
        render (m_guis[i]);
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

void kgmGameGraphics::render(Mesh *m){
  if(!m)
    return;

  if(m->material){
    render(m->material);
  }

  if(m->mesh){
    gc->gcDraw(gcpmt_triangles, m->mesh->fvf(), m->mesh->vsize(),
               m->mesh->vcount(), m->mesh->vertices(),
               2, 3 * m->mesh->fcount(), m->mesh->faces());
  }
}

void kgmGameGraphics::render(kgmActor* actor){
  if(!actor)
    return;

  for(int i = 0; i < actor->m_sections.size(); i++){
    render(actor->m_sections[i].material);
    render(actor->m_sections[i].mesh);
    render((kgmMaterial*)0);
  }
}

void kgmGameGraphics::render(kgmMaterial* m){
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
  render(s, m->m_shader);
}

void kgmGameGraphics::render(kgmShader* s, u32 t){
  vec4 vAmbient(1, 1, 1, 1);
  vec4 vLight(1, 1, 1, 1);

  switch(t){
    case 1:
      s->stop();
      break;
    default:
      s->start();
      s->set("g_fTime", kgmTime::getTime());
      s->set("g_fRandom", (float)rand()/(float)RAND_MAX);

      s->set("g_mView", m_camera.mView);
      s->set("g_mProj", m_camera.mProj);
      s->set("g_mViewProj", m_camera.mVwPj);
      s->set("g_vAmbient", vAmbient);
      s->set("g_vLight", vLight);
      s->set("g_vEye", m_camera.mPos);
      s->set("g_vEyeLook", m_camera.mDir);
  };
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
    kgmString text;

    if(!gui)
      return;

    gui->getRect(rect, true);
    gui->getText(text);

    if(gui->isClass(kgmGuiButton::Class)){
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

      switch(((kgmGuiButton*)gui)->getState()){
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
    }else if(gui->isClass(kgmGuiScroll::Class)){
    }else if(gui->isClass(kgmGuiList::Class)){
      u32 fontHeight = ((kgmGuiList*)gui)->m_itemHeight - 2;
      u32 item_cnt = ((kgmGuiList*)gui)->m_items.size();
      u32 item_view = ((kgmGuiList*)gui)->m_itemHeight;
      kgmGui::Rect frect;

      //Draw Main Rect
      gcDrawRect(rect, gui_style->slist.bg_color, 0);

      //Draw Focused Rect
      if((((kgmGuiList*)gui)->m_itemSel >= 0) &&
         (((kgmGuiList*)gui)->m_itemSel < ((kgmGuiList*)gui)->m_items.size()))
        gcDrawRect(kgmGui::Rect(rect.x, rect.y + ((kgmGuiList*)gui)->m_itemSel * ((kgmGuiList*)gui)->m_itemHeight,
                   rect.w, ((kgmGuiList*)gui)->m_itemHeight), gui_style->sbutton.fg_color, 0);
      //Draw Items Rects
      for(int i = ((kgmGuiList*)gui)->m_position; i < (((kgmGuiList*)gui)->m_position + item_view); i++){
        if(i >= item_cnt) continue;
        kgmString item; item = ((kgmGuiList*)gui)->m_items[i];
        u32 a = (i - ((kgmGuiList*)gui)->m_position);
        kgmGui::Rect clip(rect.x, rect.y + ((kgmGuiList*)gui)->m_itemHeight * a,
                          rect.w, ((kgmGuiList*)gui)->m_itemHeight);

        clip.h ++;
        //gcDrawText(gc, getFont(), 10, m_fontHeight, m_colors[kgmGui::TEXT], clip, item);
        gcDrawText(gui_style->gui_font, 10, clip.height(), 0xFFFFFFFF, clip, item);
      }
    }else if(gui->isClass(kgmGuiTab::Class)){
      render((kgmGuiTab*)gui);
    }else if(gui->isClass(kgmGuiProgress::Class)){
      render((kgmGuiProgress*)gui);
    }else if(gui->isClass(kgmGui::Class)){
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

//*************** DRAWING ***************
void kgmGameGraphics::gcDrawRect(kgmGui::Rect rc, u32 col, void* tex){
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
   }else{
    fprintf(stderr, "\nText: %s Not clipped fheight: %i fwidth: %i", (char*)text, fheight, fwidth);
   }

   cx += fwidth;
  }

  gc->gcBlend(0, 0, 0);
  gc->gcSetTexture(0, 0);
}

