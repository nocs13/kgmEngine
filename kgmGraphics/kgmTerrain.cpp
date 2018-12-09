// kgmTerrain.cpp: implementation of the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmTerrain.h"
#include "kgmPicture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
kgmTerrain::kgmTerrain()
{
  m_heighmap = null;
}

kgmTerrain::~kgmTerrain()
{
  if (m_heighmap)
    m_heighmap->release();
}

bool kgmTerrain::heightmap(kgmPicture* map)
{
  if (!map || m_heighmap)
    return false;

  m_heighmap = map;
  map->increment();

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

