// kgmTerrain.cpp: implementation of the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmTerrain.h"
#include "kgmPicture.h"
#include "kgmMesh.h"
#include "kgmMaterial.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
kgmTerrain::kgmTerrain()
{
  m_mesh = new kgmMesh();
}

kgmTerrain::~kgmTerrain()
{
  m_heightmap.map.clear();

  delete m_mesh;
}

bool kgmTerrain::heightmap(kgmPicture* map)
{
  if (!map || map->bpp != 2 || m_heightmap.map.data())
    return false;

  map->increment();

  u32 count = map->height * map->width;

  u32 bpp = map->bpp;

  m_heightmap.width = map->width;
  m_heightmap.height = map->height;
  m_heightmap.map.alloc(count);

  for (u32 j = 0; j < map->height; j++)
  {
    for (u32 i = 0; i < map->width; i++)
    {
      u8 r, g, b;
      u8 *p = (u8*) (map->pdata + bpp * map->width * j + bpp * i);

      u16 h = *((u16*)p);

      m_heightmap.map[map->width * j + i] = h;
    }
  }

  map->release();

  return true;
}

void kgmTerrain::prepare(kgmCamera* camera)
{
  if (!camera)
    return;

  m_camera = camera;
}

kgmTerrain::MeshIt kgmTerrain::meshes()
{
  MeshIt it;

  return it;
}

kgmMesh* kgmTerrain::mesh()
{
  if (m_mesh->vcount() > 0)
    m_mesh;

  return null;
}

void kgmTerrain::buildROAM(triangle3& tr)
{
  if (!m_camera)
    return;
}

kgmTerrain::float3 kgmTerrain::from_uint2(kgmTerrain::uint2 v)
{
  f32 w_pp = m_heightmap.width / m_width;
  f32 h_pp = m_heightmap.width / m_height;

  if (v.x >= m_heightmap.width)
    v.x = m_heightmap.width;

  if (v.y >= m_heightmap.height)
    v.y = m_heightmap.height;

  u16 z = v.y * m_heightmap.width + v.x;

  f32 zh = (f32) z * ((f32)m_height / (f32)0xffff);

  float3 result = float3(v.x * w_pp, v.y * h_pp, zh);

  return result;
}

kgmTerrain::uint2  kgmTerrain::from_float3(kgmTerrain::float3 v)
{
  f32 w_pp = m_heightmap.width / m_width;
  f32 h_pp = m_heightmap.width / m_height;

  return uint2(v.x / w_pp, v.y / h_pp);
}

