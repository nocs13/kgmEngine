// kgmTerrain.cpp: implementation of the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmTerrain.h"
#include "kgmPicture.h"
#include "kgmMesh.h"
#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "kgmCamera.h"

#include "../kgmMath/kgmBase.h"
#include "../kgmMath/kgmBound.h"
#include "../kgmMath/kgmVector2d.h"
#include "../kgmMath/kgmSphere2d.h"

#include "../kgmBase/kgmLog.h"
#include "../kgmBase/kgmSort.h"

kgmTerrain::kgmTerrain()
{
  m_mesh = new Mesh();
  m_fill = new Mesh();
  m_lines = new MLines();

  memset(m_tex_color, 0, sizeof(m_tex_color));

  m_tex_blend = null;

  m_width  = 1000;
  m_length = 1000;
  m_height = 500;
  m_chunk  = 100.0f;

  m_uv_scale[0] = m_uv_scale[1] = 1.0f;
}

kgmTerrain::~kgmTerrain()
{
  m_heightmap.map.clear();
  
  delete m_lines;
  delete m_mesh;

  if (m_tex_blend)
    m_tex_blend->release();

  for (s32 i = 0; i < 4; i++)
    if (m_tex_color[i])
      m_tex_color[i]->release();
}

bool kgmTerrain::heightmap(kgmPicture* map)
{
  if (!map || m_heightmap.map.data())
    return false;

  map->increment();

  u32 count = map->height * map->width;

  u32 bpp = map->bpp / 8;
  
  m_heightmap.width  = map->width;
  m_heightmap.height = map->height;
  m_heightmap.map.alloc(count);

  for (u32 j = 0; j < map->height; j++)
  {
    for (u32 i = 0; i < map->width; i++)
    {
      u8 r, g, b;
      u8 *p = (u8*) (map->pdata + bpp * map->width * j + bpp * i);

      u16 h = *((u16*)p);

      if(h)
      {
        int k = 0;
      }

      m_heightmap.map[map->width * j + i] = h;
    }
  }

  map->release();

  build();

  return true;
}

bool kgmTerrain::texColor(u32 id, kgmTexture* tex)
{
  if (!tex || !tex->texture())
    return false;

  if (id > 3)
    return false;

  if (m_tex_color[id])
    m_tex_color[id]->release();

  m_tex_color[id] = tex;

  tex->increment();
}

bool kgmTerrain::texBlend(kgmTexture* tex)
{
  if (!tex || !tex->texture())
    return false;

  if (m_tex_blend)
    m_tex_blend->release();

  m_tex_blend = tex;

  tex->increment();
}

void kgmTerrain::uvScale(f32 u, f32 v)
{
  m_uv_scale[0] = u;
  m_uv_scale[1] = v;
}

kgmTexture* kgmTerrain::texBlend()
{
  return m_tex_blend;
}

kgmTexture* kgmTerrain::texColor(u32 id)
{
  if (id > 3)
    return null;

  return m_tex_color[id];
}

void kgmTerrain::build()
{
  f32 w_pp = m_heightmap.width / m_width;
  f32 h_pp = m_heightmap.width / m_height;
}

void kgmTerrain::prepare(kgmCamera* camera)
{
  if (!camera)
    return;

  m_chunk = m_width / 20.0f;

  update(camera);
}

kgmMesh* kgmTerrain::mesh()
{
  if (m_mesh->vcount() > 0)
    return m_mesh;

  return null;
}

kgmMesh* kgmTerrain::fill()
{
  if (m_fill->vcount() > 0)
    return m_fill;

  return null;
}

kgmMesh* kgmTerrain::lines()
{
  if (m_lines->vcount() > 0)
    return m_lines;

  return null;
}

void kgmTerrain::update(kgmCamera* cam)
{
  vec2 cpos(cam->mPos.x, cam->mPos.y);

  vec2 box[2];

  box[0] = vec2(-0.5 * m_width, -0.5 * m_length);
  box[1] = vec2( 0.5 * m_width,  0.5 * m_length);

  vec2 cur = box[0];

  Chunk chunks[512];

  u32 cnt_chunks = 0;
  s32 ic[2] = {-1, -1};

  for (cur.y = box[0].y; cur.y < box[1].y; cur.y += m_chunk)
  {
    ic[1]++;

    for (cur.x = box[0].x; cur.x < box[1].x; cur.x += m_chunk)
    {
      ic[0]++;

      vec2 rect[2] = { vec2(cur.x, cur.y), vec2(cur.x + m_chunk, cur.y + m_chunk) };
      circle cr(rect, 2);
      vec3 center = vec3(cr.center.x, cr.center.y, cam->mPos.z);
      sphere sp(center, cr.radius);

      if (!cam->isSphereCross(sp.center, sp.radius)){
        continue;
      }

      f32 ds = cr.distance(cpos);

      float detalization = ds / m_chunk;

      if (detalization < 1.0)
        detalization = 1.0f;
      else if (detalization > 10.0)
        continue;

      Chunk current;

      current.rect = box2(rect[0], rect[1]);
      current.details = (u32) ceil(detalization);
      current.id[0] = ic[0], current.id[1] = ic[1];

      chunks[cnt_chunks] = current;
      cnt_chunks++;
    }

    ic[0] = -1;
  }

  kgmSort<Chunk> sort(chunks, cnt_chunks, (kgmSort<Chunk>::Compare) Chunk::compare);

  m_mesh->reset();
  m_fill->reset();
  m_lines->reset();

  for (u32 i = 0; i < cnt_chunks; i++)
  {
    Chunk* c = &chunks[i];

    generate(chunks[i].rect, chunks[i].details);


    Chunk* neibours[2] = {null};

    for (u32 j = 0; j < cnt_chunks; j++) {
      if (c == &chunks[j])
        continue;

      if ((chunks[j].id[1] == c->id[1]) && (chunks[j].id[0] == c->id[0] + 1))
        neibours[0] = &chunks[j];

      if ((chunks[j].id[0] == c->id[0]) && (chunks[j].id[1] == c->id[1] + 1))
        neibours[1] = &chunks[j];

      c->sis[0] = neibours[0];
      c->sis[1] = neibours[1];
    }

    if (neibours[0] && (c->details != neibours[0]->details)) {
      fillx(c, neibours[0]);
    }

    if (neibours[1] && (c->details != neibours[1]->details)) {
      filly(c, neibours[1]);
    }
  }
}

void kgmTerrain::generate(box2 rect, u32 level)
{
  if (level < 1)
    level = 1;

  vec2 dv = rect.max - rect.min;

  vec2 cv = rect.min;

  f32 len = 10.0f * level;

  vec2 vstart;

  vstart.x = -0.5 * m_width;
  vstart.y = -0.5 * m_length;

  vec2 uvs = vec2(1.0f / m_width, 1.0f / m_length);
  vec2 uvss = vec2(m_uv_scale[0] * uvs.x, m_uv_scale[1] * uvs.y);


  len = len_from_details(level);

  while(cv.y < rect.max.y)
  {
    while(cv.x < rect.max.x)
    {
      triangle tr, uv1, uv2;
      vec2     v;

      v.x = cv.x, v.y = cv.y;       tr.pt[0].x = v.x, tr.pt[0].y = v.y, tr.pt[0].z = get_height(v);
      v.x = cv.x + len, v.y = cv.y; tr.pt[1].x = v.x, tr.pt[1].y = v.y, tr.pt[1].z = get_height(v);
      v.x = cv.x, v.y = cv.y + len; tr.pt[2].x = v.x, tr.pt[2].y = v.y, tr.pt[2].z = get_height(v);

      uv1.pt[0] = vec3(uvs.x * (tr.pt[0].x - vstart.x), uvs.y * (tr.pt[1].y - vstart.y), 0);
      uv1.pt[1] = vec3(uvs.x * (tr.pt[1].x - vstart.x), uvs.y * (tr.pt[1].y - vstart.y), 0);
      uv1.pt[2] = vec3(uvs.x * (tr.pt[2].x - vstart.x), uvs.y * (tr.pt[2].y - vstart.y), 0);

      uv2.pt[0] = vec3(uvss.x * (tr.pt[0].x - vstart.x), uvss.y * (tr.pt[1].y - vstart.y), 0);
      uv2.pt[1] = vec3(uvss.x * (tr.pt[1].x - vstart.x), uvss.y * (tr.pt[1].y - vstart.y), 0);
      uv2.pt[2] = vec3(uvss.x * (tr.pt[2].x - vstart.x), uvss.y * (tr.pt[2].y - vstart.y), 0);

      m_mesh->add(tr, uv1, uv2);

      v.x = cv.x + len, v.y = cv.y + len; tr.pt[0].x = v.x, tr.pt[0].y = v.y, tr.pt[0].z = get_height(v);
      v.x = cv.x, v.y = cv.y + len;       tr.pt[1].x = v.x, tr.pt[1].y = v.y, tr.pt[1].z = get_height(v);
      v.x = cv.x + len, v.y = cv.y;       tr.pt[2].x = v.x, tr.pt[2].y = v.y, tr.pt[2].z = get_height(v);

      uv1.pt[0] = vec3(uvs.x * (tr.pt[0].x - vstart.x), uvs.y * (tr.pt[1].y - vstart.y), 0);
      uv1.pt[1] = vec3(uvs.x * (tr.pt[1].x - vstart.x), uvs.y * (tr.pt[1].y - vstart.y), 0);
      uv1.pt[2] = vec3(uvs.x * (tr.pt[2].x - vstart.x), uvs.y * (tr.pt[2].y - vstart.y), 0);

      uv2.pt[0] = vec3(uvss.x * (tr.pt[0].x - vstart.x), uvss.y * (tr.pt[1].y - vstart.y), 0);
      uv2.pt[1] = vec3(uvss.x * (tr.pt[1].x - vstart.x), uvss.y * (tr.pt[1].y - vstart.y), 0);
      uv2.pt[2] = vec3(uvss.x * (tr.pt[2].x - vstart.x), uvss.y * (tr.pt[2].y - vstart.y), 0);

      m_mesh->add(tr, uv1, uv2);

      cv.x += len;
    }

    cv.y += len;
    cv.x = rect.min.x;

    if (m_mesh->atLimit())
      break;
  }
}

void kgmTerrain::fillx(Chunk *c, Chunk *n)
{
  f32 len[2];

  len[0] = len_from_details(c->details);
  len[1] = len_from_details(n->details);

  f32 lbig, lsmall, laspect;

  if (len[0] > len[1]) {
    lbig = len[0];
    lsmall = len[1];
  } else {
    lbig = len[1];
    lsmall = len[0];
  }

  laspect = lbig / lsmall;

  laspect = ceil(laspect);

   vec2 vstart;

   vstart.x = -0.5 * m_width  + m_chunk * n->id[0];
   vstart.y = -0.5 * m_length + m_chunk * n->id[1];

   f32 max_y = vstart.y + m_chunk;

  vec2 cbig   = vstart;
  vec2 csmall = vstart;

  while (cbig.y < max_y)
  {
    vec3 vl[2];

    vl[0] = vec3(cbig.x, cbig.y, get_height(cbig));

    cbig.y += lbig;

    vl[1] = vec3(cbig.x, cbig.y, get_height(cbig));

    m_lines->add(vl[0], vl[1], 0xff0000ff);

    triangle tr, uv1, uv2;

    while (csmall.y < cbig.y)
    {
      tr.pt[0] = vec3(cbig.x, cbig.y, get_height(cbig));
      tr.pt[2] = vec3(csmall.x, csmall.y + lsmall, get_height(vec2(csmall.x, csmall.y + lsmall)));
      tr.pt[1] = vec3(csmall.x, csmall.y, get_height(csmall));

      //m_mesh->add(tr, 0xff00ff00);
      m_fill->add(tr, uv1, uv2);

      vl[0] = vec3(csmall.x, csmall.y, get_height(csmall));

      csmall.y += lsmall;

      vl[1] = vec3(csmall.x, csmall.y, get_height(csmall));

      m_lines->add(vl[0], vl[1], 0xffff0000);
    }

    tr.pt[0] = vec3(cbig.x, cbig.y, get_height(cbig));
    tr.pt[1] = vec3(cbig.x, cbig.y + lbig, get_height(vec2(cbig.x, cbig.y + lbig)));
    tr.pt[2] = vec3(csmall.x, csmall.y, get_height(csmall));

    m_fill->add(tr, uv1, uv2, 0xff00ff00);
  }
}

void kgmTerrain::filly(Chunk *c, Chunk *n)
{
  f32 len[2];

  len[0] = len_from_details(c->details);
  len[1] = len_from_details(n->details);

  f32 lbig, lsmall, laspect;

  if (len[0] > len[1]) {
    lbig = len[0];
    lsmall = len[1];
  } else {
    lbig = len[1];
    lsmall = len[0];
  }

  laspect = lbig / lsmall;

  laspect = ceil(laspect);

   vec2 vstart;

   vstart.x = -0.5 * m_width  + m_chunk * n->id[0];
   vstart.y = -0.5 * m_length + m_chunk * n->id[1];

   f32 max_x = vstart.x + m_chunk;

  vec2 cbig   = vstart;
  vec2 csmall = vstart;

  while (cbig.x < max_x)
  {
    vec3 vl[2];

    vl[0] = vec3(cbig.x, cbig.y, get_height(cbig));

    cbig.x += lbig;

    vl[1] = vec3(cbig.x, cbig.y, get_height(cbig));

    m_lines->add(vl[0], vl[1], 0xff0000ff);

    triangle tr, uv1, uv2;

    while (csmall.x < cbig.x)
    {
      tr.pt[0] = vec3(cbig.x, cbig.y, get_height(cbig));
      tr.pt[1] = vec3(csmall.x + lsmall, csmall.y, get_height(vec2(csmall.x + lsmall, csmall.y)));
      tr.pt[2] = vec3(csmall.x, csmall.y, get_height(csmall));

      //m_mesh->add(tr, 0xff00ff00);
      m_fill->add(tr, uv1, uv2);

      vl[0] = vec3(csmall.x, csmall.y, get_height(csmall));

      csmall.x += lsmall;

      vl[1] = vec3(csmall.x, csmall.y, get_height(csmall));

      m_lines->add(vl[0], vl[1], 0xffff0000);
    }

    tr.pt[0] = vec3(cbig.x, cbig.y, get_height(cbig));
    tr.pt[1] = vec3(cbig.x + lbig, cbig.y, get_height(vec2(cbig.x + lbig, cbig.y)));
    tr.pt[2] = vec3(csmall.x, csmall.y, get_height(csmall));

    m_fill->add(tr, uv1, uv2, 0xff00ff00);
  }
}

kgmTerrain::float2 kgmTerrain::from_uint2(kgmTerrain::uint2 v)
{
  f32 w_pp = m_heightmap.width / m_width;
  f32 h_pp = m_heightmap.height / m_length;

  if (v.x >= m_heightmap.width)
    v.x = m_heightmap.width;

  if (v.y >= m_heightmap.height)
    v.y = m_heightmap.height;

  return float2(v.x * w_pp, v.y * h_pp);
}

kgmTerrain::uint2  kgmTerrain::from_float2(kgmTerrain::float2 v)
{
  f32 w_pp = m_heightmap.width / m_width;
  f32 h_pp = m_heightmap.height / m_length;

  return uint2((v.x + 0.5 * m_width) * w_pp, (v.y + 0.5 * m_length) * h_pp);
}

f32 kgmTerrain::get_height(uint2 v)
{
  u32 offset = v.y * m_heightmap.width + v.x;

  if (offset >= (m_heightmap.width * m_heightmap.height))
    return 0.0f;

  u16 data = m_heightmap.map[offset];

  f32 height = (f32) data * ((f32)m_height / (f32)0xffff);
  
  return height;
}

f32 kgmTerrain::get_height(float2 v)
{
  uint2 uv = from_float2(v);
  
  return get_height(uv);
}

f32 kgmTerrain::len_from_details(u32 details)
{
  f32 len;

  if (details < 2) {
    len = 0.025 * m_chunk;
  } else if (details < 3) {
    len = 0.05 * m_chunk;
  } else if (details < 4) {
    len = 0.1 * m_chunk;
  } else if (details < 5) {
    len = 0.2 * m_chunk;
  } else if (details < 6) {
    len = 0.333 * m_chunk;
  } else {
    len = 0.5 * m_chunk;
  }

  return len;
}
