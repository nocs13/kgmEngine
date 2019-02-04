// kgmTerrain.cpp: implementation of the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmTerrain.h"
#include "kgmPicture.h"
#include "kgmMesh.h"
#include "kgmMaterial.h"

#include "../kgmMath/kgmBound.h"

kgmTerrain::kgmTerrain()
{
  m_chunks[0] = m_chunks[1] = null;
  
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

  m_chunks[0] = new Chunk();
  m_chunks[1] = new Chunk();

  triangle3 tr;
  
  tr.pt[0] = vec3(-0.5f * m_width,  0.5f * m_height, get_height(uint2(0, 0)));
  tr.pt[1] = vec3( 0.5f * m_width,  0.5f * m_height, get_height(uint2(m_heightmap.width, 0)));
  tr.pt[2] = vec3(-0.5f * m_width, -0.5f * m_height, get_height(uint2(0, m_heightmap.height)));
  
  m_chunks[0]->chunk = tr;

  tr.pt[0] = vec3( 0.5f * m_width, -0.5f * m_height, get_height(uint2(m_heightmap.width, m_heightmap.height)));
  tr.pt[1] = vec3(-0.5f * m_width, -0.5f * m_height, get_height(uint2(0, m_heightmap.height)));
  tr.pt[2] = vec3( 0.5f * m_width,  0.5f * m_height, get_height(uint2(m_heightmap.width, 0)));

  m_chunks[1]->chunk = tr;

  buildROAM(m_chunks[0]);
  buildROAM(m_chunks[1]);
}

void kgmTerrain::buildROAM(kgmTerrain::Chunk* c)
{
  kgmBound3d<f32> bound(c->chunk.pt, 3);

  sphere3 sbound = bound.sphere();

  if (sbound.radius < 0.6)
    return;
}

kgmTerrain::float2 kgmTerrain::from_uint2(kgmTerrain::uint2 v)
{
  f32 w_pp = m_heightmap.width / m_width;
  f32 h_pp = m_heightmap.width / m_height;

  if (v.x >= m_heightmap.width)
    v.x = m_heightmap.width;

  if (v.y >= m_heightmap.height)
    v.y = m_heightmap.height;

  return float2(v.x * w_pp, v.y * h_pp);
}

kgmTerrain::uint2  kgmTerrain::from_float2(kgmTerrain::float2 v)
{
  f32 w_pp = m_heightmap.width / m_width;
  f32 h_pp = m_heightmap.width / m_height;

  return uint2(v.x / w_pp, v.y / h_pp);
}

f32 kgmTerrain::get_height(uint2 v)
{
  u16 data = m_heightmap.map[v.y * m_heightmap.width + v.x];

  f32 height = (f32) data * ((f32)m_height / (f32)0xffff);
  
  return height;
}
