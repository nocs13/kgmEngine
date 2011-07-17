#pragma once 
#include "../kgmMain/kgmIGraphics.h"

//*************** DRAWING ***************
inline void gcDrawRect(kgmIGraphics* gc, int x, int y, int w, int h, uint col){
  typedef struct{  vec3 pos;  uint col;  vec2 uv; } V;
  V v[4];

  v[0].pos = vec3(x,     y,     0); v[0].col = col; v[0].uv = vec2(0, 0);
  v[1].pos = vec3(x,     y + h, 0); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(x + w, y,     0); v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(x + w, y + h, 0); v[3].col = col; v[3].uv = vec2(1, 1);

  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
}

inline void gcDrawPlane(kgmIGraphics* gc, vec3 pt, float w, float h, uint col){
  typedef struct{  vec3 pos;  uint col;  vec2 uv; } V;
  V v[4];

  v[0].pos = vec3(pt.x - 0.5 * w, pt.y - 0.5 * h, pt.z); v[0].col = col; v[0].uv = vec2(0, 0);
  v[1].pos = vec3(pt.x - 0.5 * w, pt.y + 0.5 * h, pt.z); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(pt.x + 0.5 * w, pt.y - 0.5 * h, pt.z); v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(pt.x + 0.5 * w, pt.y + 0.5 * h, pt.z); v[3].col = col; v[3].uv = vec2(1, 1);

  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
}

inline void gcDrawGridlines(kgmIGraphics* gc, float len, uint cnt, uint col){
  typedef struct{  vec3 pos;  uint col; } V;
  V v[2];

  float n = -0.5 * len;
  float dn = len / ((cnt > 0)?(cnt):(1));
  for(int i = 0; i < cnt; i++){
    v[0].pos = vec3(n,     -0.5 * len,     0); v[0].col = col;
    v[1].pos = vec3(n,      0.5 * len,     0); v[1].col = col;
    n += dn;
    gc->gcDraw(gcpmt_lines, gcv_xyz | gcv_col, sizeof(V), 2, v, 0, 0, 0);
  }

  n = -0.5 * len;
  for(int i = 0; i < cnt; i++){
    v[0].pos = vec3(-0.5 * len,	    n,     0); v[0].col = col;
    v[1].pos = vec3( 0.5 * len,	    n,     0); v[1].col = col;
    n += dn;
    gc->gcDraw(gcpmt_lines, gcv_xyz | gcv_col, sizeof(V), 2, v, 0, 0, 0);
  }
}

inline void gcDrawText(kgmIGraphics* gc, kgmFont* font, int fw, int fh, int x, int y, int w, int h, uint col, kgmString& text){
  typedef struct{ vec3 pos; uint col; vec2 uv; } V;
  V v[4];
  kgmRect<int> clip(x, y, x + w, y + h);

  if(text.length() < 1)
   return;

  if(!font || !font->m_texture)
   return;

  uint tlen = text.length();
  float tx = 0.0f, ty = 0.0f;
  float tdx = (float)(1.f / font->m_cols),
	tdy = (float)(1.f / font->m_rows);

  float cx = (float)x, cy = (float)y;

  gc->gc2DMode();
  gc->gcAlpha(true, gccmp_great, 0.5f);
  gc->gcSetTexture(0, font->m_texture);


  for(uint i = 0; i < tlen; i++){
   char ch = text[i];

   if(ch == '\n'){ 
	cx = (float)x; 
	cy += fh; 
	continue; 
   }
   if((ch == ' ') || ((ch == '\t')))  
	   tx = 0.0f, ty = 0.0f;

   tx = (float)(tdx * (ch % font->m_rows));
   ty = 1.0f - (float)(tdy * (ch / font->m_rows));

   if(clip.inside(cx + fw, cy + fh))
   {
    v[0].pos = vec3(cx, cy, 0),    v[0].col = col, v[0].uv = vec2(tx, ty);
    v[1].pos = vec3(cx, cy+fh, 0),  v[1].col = col, v[1].uv = vec2(tx, ty-tdy);
    v[2].pos = vec3(cx+fw, cy, 0),  v[2].col = col, v[2].uv = vec2(tx+tdx, ty);
    v[3].pos = vec3(cx+fw, cy+fh, 0),v[3].col = col, v[3].uv = vec2(tx+tdx, ty-tdy);
    gc->gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
   }

   cx += fw;
  }

  gc->gcAlpha(false, 0, 0.0);
  gc->gcSetTexture(0, 0);
  gc->gc3DMode();
}

