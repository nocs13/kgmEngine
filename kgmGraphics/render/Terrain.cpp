/*
 * Terrain.cpp
 *
 *  Created on: Feb 10, 2019
 *      Author: Goga
 */

#include "Terrain.h"
#include "../kgmGraphics.h"
#include "../kgmTerrain.h"

#include "../kgmBase/kgmLog.h"

namespace Render
{

  Terrain::Terrain(kgmGraphics* gr)
    :BaseRender(gr)
  {
    // TODO Auto-generated constructor stub
    m_shader = gr->rc->getShader("phong.glsl");
  }

  Terrain::~Terrain()
  {
    // TODO Auto-generated destructor stub
  }
  
  void Terrain::render()
  {
    mtx4 identity;
    identity.identity();

    gr->setWorldMatrix(identity);

    kgmShader* sh = m_shader;

    gr->wired(false);

    kgmIGraphics::INode* nod = gr->m_terrain;

    kgmTerrain*  ter = (kgmTerrain*) gr->m_terrain->getNodeObject();
    kgm_log() << "terrain time before " << kgmTime::getTicks() << "\n";
    ter->prepare(gr->m_camera);
    kgm_log() << "terrain time after  " << kgmTime::getTicks() << "\n";

    kgmMesh*     msh = (kgmMesh*) ter->mesh();
    
    kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);

    sphere3 sbound(gr->m_camera->mPos, 1000);

    mtx4 m = nod->getNodeTransform();
    gr->setWorldMatrix(m);

    material(mtl);
    shader(m_shader, gr->m_camera, mtl, nod);

    if (!msh)
    {
      material(null);
      shader(null, null, null, null);

      return;
    }

    kgmMesh::Vertex* v = msh->vertices();

    u32 vcount = msh->vcount();

    gr->render(msh);

    material(null);
    shader(null, null, null, null);
  }

} /* namespace Render */
