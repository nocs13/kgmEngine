// kgmTerrain.cpp: implementation of the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmTerrain.h"
#include "kgmPicture.h"
#include "kgmMesh.h"
#include "kgmMaterial.h"
#include "kgmCamera.h"

#include "../kgmMath/kgmBound.h"

#include "../kgmBase/kgmLog.h"
#include "../kgmMath/kgmBase.h"

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

  m_camera = camera;

  m_mesh->reset();

  vec3 vlook = camera->mDir;
  vec3 veye = camera->mPos;
  vec3 vup(0, 0, 1);

  vec3 vright = vlook.cross(vup);

  vright.normalize();

  f32 range = camera->mFar;

  f32 rchunk = range / 5.0f;

  for (u32 ir = 0; ir < 5; ir++) 
  {
    vec3 trapezoid[4];

    trapezoid[0] = veye - vright * (rchunk * ir) * tan(camera->mFov);
    trapezoid[1] = veye + vright * (rchunk * ir) * tan(camera->mFov);
    trapezoid[2] = veye - vright * (rchunk * (ir + 1)) * tan(camera->mFov);
    trapezoid[3] = veye + vright * (rchunk * (ir + 1)) * tan(camera->mFov);

    generate(trapezoid, ir + 1);
  }
}

void kgmTerrain::generate(vec3 points[4], u32 level)
{
  vec3 d_lr = points[1] - points[0];
  vec3 d_lu = points[2] - points[0];
  vec3 d_ru = points[3] - points[1];

  f32 length = d_lu.length();

  d_lr.normalize();
  d_lu.normalize();
  d_ru.normalize();

  u32 n = 500 / level;

  //for (int i = 0; i < n; i++)
  //{
    
  //}

  triangle3 chunks[2];

  triangle3 tr;
  
  tr.set(vec3(-0.5f * m_width, 0.5f * m_length, get_height(uint2(0, 0))),
         vec3(0.5f * m_width, 0.5f * m_length, get_height(uint2(m_heightmap.width, 0))),
         vec3(-0.5f * m_width, -0.5f * m_length, get_height(uint2(0, m_heightmap.height))));
  
  chunks[0] = tr;

  tr.set(vec3( 0.5f * m_width, -0.5f * m_length, get_height(uint2(m_heightmap.width, m_heightmap.height))),
         vec3(-0.5f * m_width, -0.5f * m_length, get_height(uint2(0, m_heightmap.height))),
         vec3( 0.5f * m_width,  0.5f * m_length, get_height(uint2(m_heightmap.width, 0))));

  chunks[1] = tr;

  gen_by_roam(&chunks[0]);
  gen_by_roam(&chunks[1]);
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
  vec3 line[2];

  vec3 dir = cam->mDir;

  dir.normalize();

  line[0] = cam->mPos;
  line[1] = line[0] + dir * cam->mFar;

  vec3 cen = (line[0] + line[1]) * 0.5;

  sphere3 sbound;

  sbound = sphere3(cen, cam->mFar * 0.5);

  vec2 box[2];

  box[0] = vec2(cen.x, cen.y) + vec2(-sbound.radius, -sbound.radius);
  box[1] = vec2(cen.x, cen.y) + vec2( sbound.radius,  sbound.radius);

  
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

void kgmTerrain::gen_by_roam(triangle3* tr)
{
 kgmBound3d<f32> bound(tr->pt, 3);

  sphere3 sbound = bound.sphere();

  if (sbound.radius < 10.0)
    return;

  kgm_log() << "Warning: Splitting chunk.\n";

  vec3 normal = tr->normal();

  vec3 split = (tr->pt[1] + tr->pt[2]) * 0.5;

  uint2 split2 = from_float2(float2(split.x, split.y));

  split.z = get_height(split2);

  triangle3 tr1(split, tr->pt[1], tr->pt[0]);
  triangle3 tr2(split, tr->pt[0], tr->pt[2]);

  vec3 nr1 = tr1.normal();
  vec3 nr2 = tr2.normal();

  //if (normal.angle(nr1) > 0.1)
  {
    gen_by_roam(&tr1);
  }

  //if (normal.angle(nr2) > 0.1)
  {
    gen_by_roam(&tr2);
  }
}
