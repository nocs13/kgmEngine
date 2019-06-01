// kgmTerrain.cpp: implementation of the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmTerrain.h"
#include "kgmPicture.h"
#include "kgmMesh.h"
#include "kgmMaterial.h"
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

  m_width  = 1000;
  m_length = 1000;
  m_height = 500;
  m_chunk  = 100.0f;
}

kgmTerrain::~kgmTerrain()
{
  m_heightmap.map.clear();
  
  delete m_mesh;
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
  }

  kgmSort<Chunk> sort(chunks, cnt_chunks, (kgmSort<Chunk>::Compare) Chunk::compare);

  m_mesh->reset();

  for (u32 i = 0; i < cnt_chunks; i++)
  {
    generate(chunks[i].rect, chunks[i].details);
  }
}

void kgmTerrain::generate(box2 rect, u32 level)
{
  if (level < 1)
    level = 1;

    vec2 dv = rect.max - rect.min;

  vec2 cv = rect.min;

  f32 len = 10.0f * level;

  if (level < 2) {
    len = 0.025 * m_chunk;
  } else if (level < 3) {
    len = 0.05 * m_chunk;
  } else if (level < 4) {
    len = 0.1 * m_chunk;
  } else if (level < 5) {
    len = 0.2 * m_chunk;
  }else if (level < 6) {
    len = 0.333 * m_chunk;
  }else {
    len = 0.5 * m_chunk;
  }

  while(cv.y < rect.max.y)
  {
    while(cv.x < rect.max.x)
    {
      if (m_mesh->fcount() > 200000)
        return;

      triangle tr;
      vec2     v;

      v.x = cv.x, v.y = cv.y;       tr.pt[0].x = v.x, tr.pt[0].y = v.y, tr.pt[0].z = get_height(v);
      v.x = cv.x + len, v.y = cv.y; tr.pt[1].x = v.x, tr.pt[1].y = v.y, tr.pt[1].z = get_height(v);
      v.x = cv.x, v.y = cv.y + len; tr.pt[2].x = v.x, tr.pt[2].y = v.y, tr.pt[2].z = get_height(v);

      m_mesh->add(tr);

      v.x = cv.x + len, v.y = cv.y + len; tr.pt[0].x = v.x, tr.pt[0].y = v.y, tr.pt[0].z = get_height(v);
      v.x = cv.x, v.y = cv.y + len;       tr.pt[1].x = v.x, tr.pt[1].y = v.y, tr.pt[1].z = get_height(v);
      v.x = cv.x + len, v.y = cv.y;       tr.pt[2].x = v.x, tr.pt[2].y = v.y, tr.pt[2].z = get_height(v);

      m_mesh->add(tr);

      cv.x += len;
    }

    cv.y += len;
    cv.x = rect.min.x;
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
