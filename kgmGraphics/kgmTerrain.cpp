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

  m_uv_scale = vec2(1.0f, 1.0f);
}

kgmTerrain::~kgmTerrain()
{
  m_heightmap.map.clear();
  
  m_lines->release();
  m_mesh->release();

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

  for (u32 i = 0; i < map->width; i++)
  {
    for (u32 j = 0; j < map->height; j++)
    {
      u8 r, g, b;
      u8 *p = (u8*) (map->pdata + bpp * map->width * i + bpp * j);

      u16 h = *((u16*)p);

      if(h)
      {
        int k = 0;
      }

      m_heightmap.map[map->width * i + j] = h;
    }
  }

  map->release();

  build();

  return true;
}

bool kgmTerrain::normalmap(kgmPicture* map)
{
  if (!map || m_heightmap.nor.data())
    return false;

  map->increment();

  u32 count = map->height * map->width;

  u32 bpp = map->bpp / 8;

  m_heightmap.nor.alloc(count);

  for (u32 i = 0; i < map->width; i++)
  {
    for (u32 j = 0; j < map->height; j++)
    {
      u8 r, g, b;
      u8 *p = (u8*) (map->pdata + bpp * map->width * i + bpp * j);

      r = (u8) p[0];
      g = (u8) p[1];
      b = (u8) p[2];

      vec3 v;

      v.x = (f32)r / 255.0;
      v.y = (f32)g / 255.0;
      v.z = (f32)b / 255.0;

      //v.normalize();

      m_heightmap.nor[map->width * i + j] = v;
    }
  }

  map->release();

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

  return true;
}

bool kgmTerrain::texBlend(kgmTexture* tex)
{
  if (!tex || !tex->texture())
    return false;

  if (m_tex_blend)
    m_tex_blend->release();

  m_tex_blend = tex;

  tex->increment();

  return true;
}

void kgmTerrain::uvScale(f32 u, f32 v)
{
  m_uv_scale = vec2(u, v);
}

vec2 kgmTerrain::uvScale()
{
  return m_uv_scale;
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
  f32 w_pp = m_heightmap.width  / m_width;
  f32 h_pp = m_heightmap.height / m_length;

  normals();
}

void kgmTerrain::normals()
{
  return;

  if (m_heightmap.map.length() < 1)
    return;

  for (u32 i = 1; i < (m_heightmap.width - 1); i++)
  {
    for (u32 j = 1; j < (m_heightmap.height - 1); j++)
    {
      uint2 u[4][3];
      u[0][0] = uint2(i, j), u[0][1] = uint2(i - 1, j), u[0][2] = uint2(i, j - 1);
      u[1][0] = uint2(i, j), u[1][1] = uint2(i, j - 1), u[1][2] = uint2(i + 1, j);
      u[2][0] = uint2(i, j), u[2][1] = uint2(i + 1, j), u[2][2] = uint2(i, j + 1);
      u[3][0] = uint2(i, j), u[3][1] = uint2(i, j + 1), u[3][2] = uint2(i - 1, j);
      float2 f[4][3];
      f[0][0] = from_uint2(u[0][0]), f[0][1] = from_uint2(u[0][1]), f[0][2] = from_uint2(u[0][2]);
      f[1][0] = from_uint2(u[1][0]), f[1][1] = from_uint2(u[1][1]), f[1][2] = from_uint2(u[1][2]);
      f[2][0] = from_uint2(u[2][0]), f[2][1] = from_uint2(u[2][1]), f[2][2] = from_uint2(u[2][2]);
      f[3][0] = from_uint2(u[3][0]), f[3][1] = from_uint2(u[3][1]), f[3][2] = from_uint2(u[3][2]);
      vec3 v[4][3];
      v[0][0] = vec3(f[0][0].x, f[0][0].y, get_height(u[0][0]));
        v[0][1] = vec3(f[0][1].x, f[0][1].y, get_height(u[0][1]));
        v[0][2] = vec3(f[0][2].x, f[0][2].y, get_height(u[0][2]));
      v[1][0] = vec3(f[1][0].x, f[1][0].y, get_height(u[1][0]));
        v[1][1] = vec3(f[1][1].x, f[1][1].y, get_height(u[1][1]));
        v[1][2] = vec3(f[1][2].x, f[1][2].y, get_height(u[1][2]));
      v[2][0] = vec3(f[2][0].x, f[2][0].y, get_height(u[2][0]));
        v[2][1] = vec3(f[2][1].x, f[2][1].y, get_height(u[2][1]));
        v[2][2] = vec3(f[2][2].x, f[2][2].y, get_height(u[2][2]));
      v[3][0] = vec3(f[1][0].x, f[3][0].y, get_height(u[3][0]));
        v[3][1] = vec3(f[3][1].x, f[3][1].y, get_height(u[3][1]));
        v[3][2] = vec3(f[3][2].x, f[3][2].y, get_height(u[3][2]));

      triangle3 tr[4];
      tr[0].set(v[0][0], v[0][1], v[0][2]);
      tr[1].set(v[1][0], v[1][1], v[1][2]);
      tr[2].set(v[2][0], v[2][1], v[2][2]);
      tr[3].set(v[3][0], v[3][1], v[3][2]);

      //uint2  u1(i, j), u2(i + 1, j), u3(i, j + 1);
      //float2 f1 = from_uint2(u1);
      //float2 f2 = from_uint2(u2);
      //float2 f3 = from_uint2(u3);

      //vec3  v1 = vec3(f1.x, f1.y, get_height(u1));
      //vec3  v2 = vec3(f2.x, f2.y, get_height(u2));
      //vec3  v3 = vec3(f3.x, f3.y, get_height(u3));

      //triangle3 tr1;

      //tr1.set(v1, v2, v3);

      //vec3 n = tr1.normal();

      //n.normalize();

      vec3 n = vec3(0, 0, 0);

      for (u32 k = 0; k < 4; k++)
      {
        vec3 nn = tr[k].normal();
        n += nn;
      }

      //n.normalize();

      m_heightmap.nor[m_heightmap.width * i + j]       = n;
    }
  }

  for (u32 i = 0; i < (m_heightmap.width - 1); i++)
  {
    for (u32 j = 0; j < (m_heightmap.height - 1); j++)
    {
      vec3 n;

      vec3 nn[2];

      n = m_heightmap.nor[m_heightmap.width * i + j];
      nn[0] = m_heightmap.nor[m_heightmap.width * i + (j + 1)];
      nn[1] = m_heightmap.nor[m_heightmap.width * (i + 1) + j];

      n += nn[0];
      n += nn[1];
      n.normalize();

      m_heightmap.nor[m_heightmap.width * i + j] = n;
    }
  }
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

  len = len_from_details(level);

  while(cv.y < rect.max.y)
  {
    while(cv.x < rect.max.x)
    {
      triangle tr[2], uv[2], tn[2];
      vec2     v;
      vec3     n;

      v.x = cv.x, v.y = cv.y;       tr[0].pt[0].x = v.x, tr[0].pt[0].y = v.y, tr[0].pt[0].z = get_height(v);
      n = get_normal(v); n.normalize();
      v.x = cv.x + len, v.y = cv.y; tr[0].pt[1].x = v.x, tr[0].pt[1].y = v.y, tr[0].pt[1].z = get_height(v);
      v.x = cv.x, v.y = cv.y + len; tr[0].pt[2].x = v.x, tr[0].pt[2].y = v.y, tr[0].pt[2].z = get_height(v);

      uv[0].pt[0] = vec3(uvs.x * (tr[0].pt[0].x - vstart.x), uvs.y * (tr[0].pt[1].y - vstart.y), 0);
      uv[0].pt[1] = vec3(uvs.x * (tr[0].pt[1].x - vstart.x), uvs.y * (tr[0].pt[1].y - vstart.y), 0);
      uv[0].pt[2] = vec3(uvs.x * (tr[0].pt[2].x - vstart.x), uvs.y * (tr[0].pt[2].y - vstart.y), 0);

      //tn[0] = triangle(tr[0].pt[0], tr[0].pt[1], tr[0].pt[2]);
      //n = tn[0].normal();


      v.x = cv.x + len, v.y = cv.y + len; tr[1].pt[0].x = v.x, tr[1].pt[0].y = v.y, tr[1].pt[0].z = get_height(v);
      v.x = cv.x, v.y = cv.y + len;       tr[1].pt[1].x = v.x, tr[1].pt[1].y = v.y, tr[1].pt[1].z = get_height(v);
      v.x = cv.x + len, v.y = cv.y;       tr[1].pt[2].x = v.x, tr[1].pt[2].y = v.y, tr[1].pt[2].z = get_height(v);

      uv[1].pt[0] = vec3(uvs.x * (tr[1].pt[0].x - vstart.x), uvs.y * (tr[1].pt[1].y - vstart.y), 0);
      uv[1].pt[1] = vec3(uvs.x * (tr[1].pt[1].x - vstart.x), uvs.y * (tr[1].pt[1].y - vstart.y), 0);
      uv[1].pt[2] = vec3(uvs.x * (tr[1].pt[2].x - vstart.x), uvs.y * (tr[1].pt[2].y - vstart.y), 0);

      //tn[1] = triangle(tr[1].pt[0], tr[1].pt[1], tr[1].pt[2]);
      //n += tn[1].normal();

      m_mesh->add(tr[0], uv[0], 0xffffffff, n);
      m_mesh->add(tr[1], uv[1], 0xffffffff, n);

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

  vec2 uvs = vec2(1.0f / m_width, 1.0f / m_length);

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

    triangle tr, uv;
    vec3     n;

    n = get_normal(vec2(cbig.x, cbig.y));

    while (csmall.y < cbig.y)
    {
      tr.pt[0] = vec3(cbig.x, cbig.y, get_height(cbig));
      tr.pt[2] = vec3(csmall.x, csmall.y + lsmall, get_height(vec2(csmall.x, csmall.y + lsmall)));
      tr.pt[1] = vec3(csmall.x, csmall.y, get_height(csmall));

      uv.pt[0] = vec3(uvs.x * (tr.pt[0].x - vstart.x), uvs.y * (tr.pt[1].y - vstart.y), 0);
      uv.pt[1] = vec3(uvs.x * (tr.pt[1].x - vstart.x), uvs.y * (tr.pt[1].y - vstart.y), 0);
      uv.pt[2] = vec3(uvs.x * (tr.pt[2].x - vstart.x), uvs.y * (tr.pt[2].y - vstart.y), 0);

      //m_mesh->add(tr, 0xff00ff00);
      m_fill->add(tr, uv, 0xffffffff, n);

      vl[0] = vec3(csmall.x, csmall.y, get_height(csmall));

      csmall.y += lsmall;

      vl[1] = vec3(csmall.x, csmall.y, get_height(csmall));

      m_lines->add(vl[0], vl[1], 0xffff0000);
    }

    tr.pt[0] = vec3(cbig.x, cbig.y, get_height(cbig));
    tr.pt[1] = vec3(cbig.x, cbig.y + lbig, get_height(vec2(cbig.x, cbig.y + lbig)));
    tr.pt[2] = vec3(csmall.x, csmall.y, get_height(csmall));

    //m_fill->add(tr, uv, 0xff00ff00);
    m_fill->add(tr, uv, 0xffffffff, n);
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

  vec2 uvs = vec2(1.0f / m_width, 1.0f / m_length);

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

    triangle tr, uv;

    vec3 n;

    n = get_normal(vec2(cbig.x, cbig.y));

    while (csmall.x < cbig.x)
    {
      tr.pt[0] = vec3(cbig.x, cbig.y, get_height(cbig));
      tr.pt[1] = vec3(csmall.x + lsmall, csmall.y, get_height(vec2(csmall.x + lsmall, csmall.y)));
      tr.pt[2] = vec3(csmall.x, csmall.y, get_height(csmall));

      uv.pt[0] = vec3(uvs.x * (tr.pt[0].x - vstart.x), uvs.y * (tr.pt[1].y - vstart.y), 0);
      uv.pt[1] = vec3(uvs.x * (tr.pt[1].x - vstart.x), uvs.y * (tr.pt[1].y - vstart.y), 0);
      uv.pt[2] = vec3(uvs.x * (tr.pt[2].x - vstart.x), uvs.y * (tr.pt[2].y - vstart.y), 0);

      //m_mesh->add(tr, 0xff00ff00);
      m_fill->add(tr, uv, 0xffffffff, n);

      vl[0] = vec3(csmall.x, csmall.y, get_height(csmall));

      csmall.x += lsmall;

      vl[1] = vec3(csmall.x, csmall.y, get_height(csmall));

      m_lines->add(vl[0], vl[1], 0xffff0000);
    }

    tr.pt[0] = vec3(cbig.x, cbig.y, get_height(cbig));
    tr.pt[1] = vec3(cbig.x + lbig, cbig.y, get_height(vec2(cbig.x + lbig, cbig.y)));
    tr.pt[2] = vec3(csmall.x, csmall.y, get_height(csmall));

    //m_fill->add(tr, uv, 0xff00ff00);
    m_fill->add(tr, uv, 0xffffffff, n);
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
  u32 offset = v.x * m_heightmap.width + v.y;

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

vec3 kgmTerrain::get_normal(uint2 v)
{
  vec3 n, nn;

  if (m_heightmap.nor.length() < 1 || v.x >= m_heightmap.width || v.y >= m_heightmap.height)
    return vec3(0, 0, 1);

  n = m_heightmap.nor[m_heightmap.width * v.x + v.y];

  n.normalize();

  return n;
}

vec3 kgmTerrain::get_normal(float2 v)
{
  uint2 u = from_float2(v);

  vec3 n = get_normal(u);

  return n;
}
