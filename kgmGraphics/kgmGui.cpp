#include "kgmGui.h"
#include "kgmFont.h"
#include "../kgmMain/kgmIGraphics.h"

kgmGui::kgmGui(){
  m_id = 0;
  m_parent = 0;
  m_view = false;
  m_focus = this;
  m_font = 0;

  m_colors[0] = 0xff777777;
  m_colors[1] = 0xff999999;
  m_colors[2] = 0xffaaaaaa;
}

kgmGui::kgmGui(kgmGui *par, int x, int y, int w, int h){
  m_id = 0;
  m_parent = par;
  m_rect = Rect(x, y, w, h);
  m_view = true;
  m_font = 0;
  m_focus = (par)?(0):(this);
  if(par){
   par->addChild(this);
   m_parent = par;
   m_font = par->m_font;
  }

  m_colors[0] = 0xff777777;
  m_colors[1] = 0xff999999;
  m_colors[2] = 0xffaaaaaa;
}

kgmGui::~kgmGui(){
  m_childs.clear();
  if(m_parent) 
   m_parent->delChild(this);
}


//public:
void kgmGui::show(){
  m_view = true;
}

void kgmGui::hide(){
  m_view = false;
}

void kgmGui::scale(float sw, float sh){
  m_rect.x *= sw;
  m_rect.y *= sh;
  m_rect.w *= sw;
  m_rect.h *= sh;
  for(int i = 0; i < m_childs.size(); i++)
    m_childs[i]->scale(sw, sh);
}

void kgmGui::resize(int w, int h){
 m_rect.w = w;
 m_rect.h = h;
}
 
void kgmGui::repaint(kgmIGraphics* gc){
 onPaint(gc);
}

void kgmGui::repaint(){
 onPaint(getGC());
}

void kgmGui::setGC(kgmIGraphics* gc){
 if(gc)
  m_gc = gc;
}

void kgmGui::setParent(kgmGui* pr){
 if(!pr)
  return;
 if(m_parent)
  m_parent->delChild(this);
 pr->addChild(this);
 m_parent = pr;
}

void kgmGui::setRect(kgmGui::Rect& rc){
 m_rect = rc;
}
void kgmGui::setRect(uint x, uint y, uint w, uint h){
 m_rect = Rect(x, y, w, h);
}

void kgmGui::getRect(kgmGui::Rect& rc){
 rc = m_rect;
}

void kgmGui::getRect(uint& x, uint& y, uint& w, uint& h){
  x = m_rect.x;
  y = m_rect.y;
  w = m_rect.w;
  h = m_rect.h;
}

void kgmGui::setFont(kgmFont* font){
 if(font)
  m_font = font;
}

void kgmGui::setText(kgmString& s){
 m_text = s;
}

void kgmGui::setColor(uint type, uint col){
 if(type >= 0 && type < EXTRA)
  m_colors[type] = col;
}

void kgmGui::setTexture(uint type, kgmTexture* t){
 if(type >= 0 && type < EXTRA)
  m_texture[type] = t;
}

void kgmGui::setMaterial(uint type, kgmMaterial* m){
 if(type >= 0 && type < EXTRA)
  m_material[type] = m;
}

// MESSAGE MANAGER
void kgmGui::onEvent(kgmEvent::Event* e){
  if(!e)
   return;
  kgmEvent::onEvent(e);
  switch(e->event){
  case evtPaint:
//	  onPaint((kgmIGraphics*)e->m_gc);
	  break;
  }
}

void kgmGui::onAction(kgmGui* from, uint type){
  if(m_parent)
   m_parent->onAction(from, type);
}

void kgmGui::onMsWheel(int k, int x, int y, int z){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view)
    m_childs[i]->onMsWheel(k, x, y, z);
}

void kgmGui::onMsMove(int k, int x, int y){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view)
    m_childs[i]->onMsMove(k, x, y);
}

void kgmGui::onMsLeftDown(int k, int x, int y){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view && m_childs[i]->m_rect.inside(x, y))
    m_childs[i]->onMsLeftDown(k, x, y);
}

void kgmGui::onMsLeftUp(int k, int x, int y){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view && m_childs[i]->m_rect.inside(x, y))
    m_childs[i]->onMsLeftUp(k, x, y);
}

void kgmGui::onMsRightDown(int k, int x, int y){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view && m_childs[i]->m_rect.inside(x, y))
    m_childs[i]->onMsRightDown(k, x, y);
}

void kgmGui::onMsRightUp(int k, int x, int y){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view && m_childs[i]->m_rect.inside(x, y))
    m_childs[i]->onMsRightUp(k, x, y);
}

void kgmGui::onKeyDown(int k){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view)
    m_childs[i]->onKeyDown(k);
}

void kgmGui::onKeyUp(int k){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view)
    m_childs[i]->onKeyUp(k);
}

void kgmGui::onPaint(kgmIGraphics* gc){
  if(!m_view || !gc)
   return;

  gcDrawRect(gc, m_rect, m_colors[BACKGROUND], 0);

  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i]->m_view)
      m_childs[i]->onPaint(gc);
}

void kgmGui::onIdle(){
}

//protected:
long2 kgmGui::toScreen(long2 pt){
  kgmGui *par = m_parent;
  long2 ret = pt;
  while(par){
   ret.x += par->m_rect.x;
   ret.y += par->m_rect.y;
   par = par->m_parent;
  }
  return ret;
}

void kgmGui::addChild(kgmGui *e){
  if(!e)
   return;
  if(!isChild(e))
   m_childs.push_back(e);
}

void kgmGui::delChild(kgmGui *e){
 for(Guis::iterator i = m_childs.begin(); i != m_childs.end(); ++i){
  if(e == *i)
   i = m_childs.erase(i);
 }
}

bool kgmGui::isChild(kgmGui *e){
  for(int i = 0; i < m_childs.size(); i++)
   if(m_childs[i] == e)
    return true;
  return false;
}

kgmGui* kgmGui::getFocus(long2 pos){
  for(int i = 0; i < m_childs.size(); i++){
   if(m_childs[i]->m_view && m_childs[i]->m_rect.inside(pos.x, pos.y)){
     return m_childs[i];
   }
  }
  return 0;
}

kgmIGraphics* kgmGui::getGC(){
 kgmGui* g = this;
 while(g){
  if(g->m_gc)
   return g->m_gc;
  else
    g = g->m_parent;
 }
 return 0;
}

kgmFont* kgmGui::getFont(){
 kgmGui* g = this;
 while(g){
  if(g->m_font)
   return g->m_font;
  else
    g = g->m_parent;
 }
 return 0;
}

//*************** DRAWING ***************
void kgmGui::gcDrawRect(kgmIGraphics* gc, Rect rc, uint col, void* tex){
  typedef struct{  vec3 pos;  uint col;  vec2 uv; } V;
  V v[4];

  v[0].pos = vec3(rc.x,        rc.y,        0); v[0].col = col; v[0].uv = vec2(0, 0);
  v[1].pos = vec3(rc.x,        rc.y + rc.h, 0); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(rc.x + rc.w, rc.y,        0); v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(rc.x + rc.w, rc.y + rc.h, 0); v[3].col = col; v[3].uv = vec2(1, 1);

  if(tex)
   gc->gcSetTexture(0, tex);
//  gc->gc2DMode();
  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
//  gc->gc3DMode();
  if(tex)
   gc->gcSetTexture(0, 0);
}

void kgmGui::gcDrawText(kgmIGraphics* gc, kgmFont* font, uint fwidth, uint fheight, uint fcolor, Rect clip, kgmString& text){
  typedef struct{ vec3 pos; uint col; vec2 uv; } V;
  V v[4];
  uint tlen = text.length();

  if(tlen < 1)
   return;

  if(!font || !font->m_texture)
   return;

  float tx = 0.0f, ty = 0.0f;
  float tdx = (float)(1.f / font->m_cols),
		tdy = (float)(1.f / font->m_rows);

  float cx = (float)clip.x, cy = (float)clip.y;

//  gc->gc2DMode();
  gc->gcBlend(true, gcblend_one, gcblend_one);
  gc->gcSetTexture(0, font->m_texture);


  for(uint i = 0; i < tlen; i++){
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
//  gc->gc3DMode();
}
