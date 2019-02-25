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
  m_chunks[0] = m_chunks[1] = null;
  
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

  if (!m_chunks[0] || !m_chunks[1])
    return;

  m_mesh->reset();

  updateMesh(camera,  m_chunks[0]);
  updateMesh(camera,  m_chunks[1]);
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
  
  tr.set(vec3(-0.5f * m_width, 0.5f * m_length, get_height(uint2(0, 0))),
         vec3(0.5f * m_width, 0.5f * m_length, get_height(uint2(m_heightmap.width, 0))),
         vec3(-0.5f * m_width, -0.5f * m_length, get_height(uint2(0, m_heightmap.height))));
  
  m_chunks[0]->chunk = tr;

  tr.set(vec3( 0.5f * m_width, -0.5f * m_length, get_height(uint2(m_heightmap.width, m_heightmap.height))),
         vec3(-0.5f * m_width, -0.5f * m_length, get_height(uint2(0, m_heightmap.height))),
         vec3( 0.5f * m_width,  0.5f * m_length, get_height(uint2(m_heightmap.width, 0))));

  m_chunks[1]->chunk = tr;

  buildROAM(m_chunks[0]);
  buildROAM(m_chunks[1]);
}

void kgmTerrain::buildROAM(kgmTerrain::Chunk* c)
{
  kgmBound3d<f32> bound(c->chunk.pt, 3);

  sphere3 sbound = bound.sphere();

  if (sbound.radius < 3.0)
    return;

  kgm_log() << "Warning: Splitting chunk.\n";

  vec3 normal = c->chunk.normal();

  vec3 split = (c->chunk.pt[1] + c->chunk.pt[2]) * 0.5;

  uint2 split2 = from_float2(float2(split.x, split.y));

  split.z = get_height(split2);

  triangle3 tr1(split, c->chunk.pt[1], c->chunk.pt[0]);
  triangle3 tr2(split, c->chunk.pt[0], c->chunk.pt[2]);

  vec3 nr1 = tr1.normal();
  vec3 nr2 = tr2.normal();

  if ((sbound.radius < 100.0) && (nr1.angle(nr2) < (PI / 20.0)))
	  return;

  //if (normal.angle(nr1) > 0.1)
  {
    c->right = new Chunk();
    c->right->chunk = tr1;

    buildROAM(c->right);
  }

  //if (normal.angle(nr2) > 0.1)
  {
    c->left = new Chunk();
    c->left->chunk = tr2;

    buildROAM(c->left);
  }
}

void kgmTerrain::updateMesh(kgmCamera* cam, kgmTerrain::Chunk* c)
{
  kgmBound3d<f32> bound(c->chunk.pt, 3);

  sphere3 sbound = bound.sphere();

  if (!cam->isSphereCross(sbound.center, sbound.radius))
    return;

  if (!c->left && !c->right)
  {
    m_mesh->add(c->chunk);

    return;
  }

  f32 distance = sbound.center.distance(cam->mPos);

  if ((sbound.radius / (distance + 1.0)) < 0.05)
  {
    m_mesh->add(c->chunk);

    return;
  }

  if (c->left)
    updateMesh(cam,  c->left);

  if (c->right)
    updateMesh(cam,  c->right);
}

void kgmTerrain::updateMesh(kgmCamera* cam, box2* rc)
{
  if (!cam || !rc)
    return;

  box3 bb = box3(vec3(rc->min.x, rc->min.y, get_height(rc->min)), vec3(rc->max.x, rc->max.y, get_height(rc->max)));
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
