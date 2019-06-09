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

  void Terrain::Thread::run()
  {
    if (!tr)
      return;

    valid = true;

    while(valid)
    {
      kgmTerrain*  ter = (kgmTerrain*) tr->gr->m_terrain->getNodeObject();

      if (ter)
      {
        lock();
        ter->prepare(tr->gr->m_camera);
        unlock();
      }

      kgmThread::sleep(0);
    }

    int k = 0;
  }

  Terrain::Terrain(kgmGraphics* gr)
    :BaseRender(gr)
  {
    m_thread = new Thread(this);
    // TODO Auto-generated constructor stub
    //m_shader = gr->rc->getShader("base.glsl");
    m_shader = gr->rc->getShader("phong.glsl");
  }

  Terrain::~Terrain()
  {
    clear();

    delete m_thread;
  }

  void Terrain::clear()
  {
    m_thread->stop();
    m_thread->wait();
  }
  
  void Terrain::render()
  {
    if(!m_thread->active())
      m_thread->start();

    mtx4 identity;
    identity.identity();

    kgmShader* sh = m_shader;

    gr->wired(false);

    kgmIGraphics::INode* nod = gr->m_terrain;

    kgmTerrain*  ter = (kgmTerrain*) gr->m_terrain->getNodeObject();

    //if (!ter)
    //  return;

    //kgm_log() << "terrain time before " << kgmTime::getTicks() << "\n";
    //ter->prepare(gr->m_camera);
    //kgm_log() << "terrain time after  " << kgmTime::getTicks() << "\n";

    m_thread->lock();

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

      m_thread->unlock();

      return;
    }

    kgmMesh::Vertex* v = msh->vertices();

    u32 vcount = msh->vcount();

    //gr->gc->gcCull(gc_null);
    gr->render(msh);
    //gr->gc->gcCull(gccull_back);

    {
      kgmMesh* lines = null;//ter->lines();

      if (lines)
      {
        gc->gcDraw(gcpmt_lines, lines->fvf(), lines->vsize(),
                   lines->vcount(), lines->vertices(),
                   0, 0, 0);
      }
    }

    m_thread->unlock();

    material(null);
    shader(null, null, null, null);
  }

} /* namespace Render */
