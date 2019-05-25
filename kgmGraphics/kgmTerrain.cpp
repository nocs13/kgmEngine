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

void kgmTerrain::prepare(kgmCamera* camera)
{
  if (!camera)
    return;

  update(camera);
}

void kgmTerrain::generate(vec3 points[4], u32 level)
{
}

kgmTerrain::MeshIt kgmTerrain::meshes()
{
  MeshIt it;

  return it;
}

kgmMesh* kgmTerrain::mesh()
{
  if (m_mesh->vcount() > 0)
    return m_mesh;

  return null;
}

void kgmTerrain::build()
{
  f32 w_pp = m_heightmap.width / m_width;
  f32 h_pp = m_heightmap.width / m_height;
}

void kgmTerrain::update(kgmCamera* cam)
{
  struct Chunk {
    box2 rect;
    u32  details;

    static bool compare(Chunk& a, Chunk& b)
    {
      return (a.details < b.details);
    }

    bool operator<(Chunk& a)
    {
      return (details < a.details);
    }
  };

  vec3 line[2];

  vec3 dir = cam->mDir;

  vec2 cpos(cam->mPos.x, cam->mPos.y);

  dir.normalize();

  line[0] = cam->mPos;
  line[1] = line[0] + dir * cam->mFar;

  vec3 cen = (line[0] + line[1]) * 0.5;

  sphere3 sbound;

  sbound = sphere3(cen, cam->mFar * 0.5);

  vec2 box[2];

  box[0] = vec2(cen.x, cen.y) + vec2(-sbound.radius, -sbound.radius);
  box[1] = vec2(cen.x, cen.y) + vec2( sbound.radius,  sbound.radius);

  vec2 cur = box[0];

  f32 chunk = 100.0f;

  Chunk chunks[256];

  u32 cnt_chunks = 0;

  for (cur.y = box[0].y; cur.y < box[1].y; cur.y += chunk)
  {
    for (cur.x = box[0].x; cur.x < box[1].x; cur.x += chunk)
    {
      vec2 rect[2] = { vec2(cur.x, cur.y), vec2(cur.x + chunk, cur.y + chunk) };
      circle cr(rect, 2);
      vec3 center = vec3(cr.center.x, cr.center.y, 0);
      sphere sp(center, cr.radius);

      if (!cam->isSphereCross(sp.center, sp.radius)){
        continue;
      }

      f32 ds = cr.distance(cpos);

      float detalization = ds / chunk;

      if (detalization < 1.0)
        detalization = 1.0f;

      Chunk current;

      current.rect = box2(rect[0], rect[1]);
      current.details = (u32) ceil(detalization);

      chunks[cnt_chunks] = current;
      cnt_chunks++;
    }
  }

  kgmSort<Chunk> sort(chunks, cnt_chunks, (kgmSort<Chunk>::Compare) Chunk::compare);

  for (u32 i = 0; i < cnt_chunks; i++)
  {
    generate(chunks[i].rect, chunks[i].details);
  }
}

void kgmTerrain::generate(box2 rect, u32 level)
{
  if (level > 5)
    return;

  if (level < 1)
    level = 1;

  vec2 dv = rect.max - rect.min;

  vec2 cv = rect.min;

  f32 len = 10.0f * level;

  while(cv.y < rect.max.y)
  {
    while(cv.x < rect.max.x)
    {
      triangle tr;
      vec2     v;

      v.x = cv.x, v.y = cv.y;       tr.pt[0].x = v.x, tr.pt[0].y = v.y, tr.pt[0].z = get_height(v);
      v.x = cv.x + len, v.y = cv.y; tr.pt[1].x = v.x, tr.pt[1].y = v.y, tr.pt[1].z = get_height(v);
      v.x = cv.x, v.y = cv.y + len; tr.pt[2].x = v.x, tr.pt[2].y = v.y, tr.pt[2].z = get_height(v);

      m_mesh->add(tr);

      v.x = cv.x + len, v.y = cv.y + level; tr.pt[0].x = v.x, tr.pt[0].y = v.y, tr.pt[0].z = get_height(v);
      v.x = cv.x, v.y = cv.y + level;       tr.pt[1].x = v.x, tr.pt[1].y = v.y, tr.pt[1].z = get_height(v);
      v.x = cv.x + len, v.y = cv.y;         tr.pt[2].x = v.x, tr.pt[2].y = v.y, tr.pt[2].z = get_height(v);

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
  u16 data = m_heightmap.map[v.y * m_heightmap.width + v.x];

  f32 height = (f32) data * ((f32)m_height / (f32)0xffff);
  
  return height;
}

f32 kgmTerrain::get_height(float2 v)
{
  uint2 uv = from_float2(v);
  
  return get_height(uv);
}
