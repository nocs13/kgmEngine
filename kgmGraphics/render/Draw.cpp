#include "Draw.h"

#include "../../kgmBase/kgmIGC.h"

#include "../kgmGui.h"
#include "../kgmIcon.h"
#include "../kgmFont.h"
#include "../kgmMesh.h"
#include "../kgmSprite.h"
#include "../kgmTexture.h"
#include "../kgmParticles.h"

void gcDrawRect(kgmIGC* gc, kgmGui::Rect rc, u32 col, kgmTexture* tex)
{
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;

  V v[4];

  v[0].pos = vec3(rc.x,        rc.y,        -1); v[0].col = col; v[0].uv = vec2(0, 1);
  v[1].pos = vec3(rc.x,        rc.y + rc.h, -1); v[1].col = col; v[1].uv = vec2(0, 0);
  v[2].pos = vec3(rc.x + rc.w, rc.y,        -1); v[2].col = col; v[2].uv = vec2(1, 1);
  v[3].pos = vec3(rc.x + rc.w, rc.y + rc.h, -1); v[3].col = col; v[3].uv = vec2(1, 0);

  if(tex && tex->texture())
    gc->gcSetTexture(0, tex->texture());

  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);

  if(tex)
    gc->gcSetTexture(0, 0);
}

void gcDrawBorder(kgmIGC* gc, kgmGui::Rect rc, u32 col, kgmTexture* tex)
{
  typedef struct{  vec3 pos;  u32 col;} V;

  V v[5];

  v[0].pos = vec3(rc.x,        rc.y,        -1); v[0].col = col;
  v[1].pos = vec3(rc.x + rc.w, rc.y,        -1); v[1].col = col;
  v[2].pos = vec3(rc.x + rc.w, rc.y + rc.h, -1); v[2].col = col;
  v[3].pos = vec3(rc.x,        rc.y + rc.h, -1); v[3].col = col;
  v[4].pos = vec3(rc.x,        rc.y,        -1); v[4].col = col;

  if(tex && tex->texture())
    gc->gcSetTexture(0, tex->texture());

  gc->gcDraw(gcpmt_linestrip, gcv_xyz | gcv_col, sizeof(V), 5, v, 0, 0, 0);

  if(tex)
    gc->gcSetTexture(0, 0);
}

void gcDrawText(kgmIGC* gc, kgmFont* font, u32 fwidth, u32 fheight, u32 fcolor, kgmGui::Rect clip, kgmString& text)
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

  float cx = (float)clip.x,
        cy = (float)clip.y;

  gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);
  gc->gcSetTexture(0, font->texture());

  for(u32 i = 0; i < tlen; i++)
  {
    char ch = text[i];

    if (ch == '\0')
      break;

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

void gcDrawBillboard(kgmIGC* gc, box b, u32 col)
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

  gc->gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
}

//void gcDraw(kgmGui*){}

void gcDraw(kgmIGC* gc, kgmMesh* m)
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

void gcDraw(kgmIGC* gc, kgmVisual* visual)
{}

void gcDraw(kgmIGC* gc, kgmSprite* sprite)
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

void gcDraw(kgmIGC* gc, kgmParticles* particles)
{
  if(!particles || !particles->getMesh())
    return;

  gcDraw(gc, particles->getMesh());
}

void gcDraw(kgmIGC* gc, kgmIcon* icon)
{
  mtx4 mtr;
  vec3 dir;
  vec3 rv, uv;

  gc->gcGetMatrix(gcmtx_view, mtr.m);

  dir = vec3(mtr.m[2], mtr.m[6], mtr.m[10]);

  dir.normalize();

  dir.invert();

  rv = vec3(mtr.m[0], mtr.m[2], mtr.m[1]);
  rv.normalize();
  uv = rv.cross(dir);
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
