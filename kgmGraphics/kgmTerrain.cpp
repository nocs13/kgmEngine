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

void kgmTerrain::prepare(kgmCamera* cam)
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
    m_mesh;

  return null;
}
