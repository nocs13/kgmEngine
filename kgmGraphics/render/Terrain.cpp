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

    m_sh_cmap = gr->rc->getShader("terrain_cmap.glsl");
    m_sh_lmap = gr->rc->getShader("terrain_lmap.glsl");
  }

  Terrain::~Terrain()
  {
    clear();

    if (m_sh_cmap)
      m_sh_cmap->release();

    if (m_sh_lmap)
      m_sh_lmap->release();

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

    kgmShader* sh = m_sh_cmap;

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
    shader(sh, gr->m_camera, mtl, nod);

    if (!msh)
    {
      material(null);
      shader(null, null, null, null);

      m_thread->unlock();

      return;
    }

    if (ter->texBlend())
    {
      gr->gc->gcSetTexture(0, ter->texBlend()->texture());
    }

    vec2 uvscale = ter->uvScale();

    if (!sh)
      return;

    sh->set("g_vUVScale", uvscale);
    sh->set("g_txColor", 0);
    sh->set("g_txNormal", 1);
    sh->set("g_txSpecular", 2);
    sh->set("g_txColor1", 3);
    sh->set("g_txColor2", 4);

    for (u32 i = 0; i < 4; i++)
    {
      if (ter->texColor(i))
        gr->gc->gcSetTexture(1 + i, ter->texColor(i)->texture());
      else
        gr->gc->gcSetTexture(1 + i, gr->m_tex_black->texture());
    }

    kgmMesh::Vertex* v = msh->vertices();

    u32 vcount = msh->vcount();

    gr->render(msh);

    msh = ter->fill();

    if (msh)
    {
      gr->gc->gcCull(gc_null);

      gr->render(msh);

      gr->gc->gcCull(gccull_back);
    }

#ifdef DEBUG
    {
      kgmMesh* lines = ter->lines();

      if (lines)
      {
        gc->gcDraw(gcpmt_lines, lines->fvf(), lines->vsize(),
                   lines->vcount(), lines->vertices(),
                   0, 0, 0);
      }
    }
#endif

    m_thread->unlock();

    material(null);
    shader(null, null, null, null);
  }

  void Terrain::lightmap(bool blend)
  {
    mtx4 identity;
    identity.identity();

    kgmShader* sh = m_sh_lmap;

    gr->wired(false);

    kgmIGraphics::INode* nod = gr->m_terrain;

    kgmTerrain*  ter = (kgmTerrain*) gr->m_terrain->getNodeObject();

    if (!ter)
      return;

    ter->prepare(gr->m_camera);

    kgmMesh*     msh = (kgmMesh*) ter->mesh();

    if (!msh)
      return;

    kgmCamera* cam = &gr->camera();

    mtx4 m = nod->getNodeTransform();
    gr->setWorldMatrix(m);

    if (ter->texBlend())
    {
      gr->gc->gcSetTexture(0, ter->texBlend()->texture());
    }

    vec2 uvscale = ter->uvScale();

    if (!sh)
      return;

    sh->start();
    sh->set("g_mTran",           m);
    sh->set("g_mProj",           cam->mProj);
    sh->set("g_mView",           cam->mView);
    sh->set("g_vEye",            cam->mPos);
    sh->set("g_vLook",           cam->mDir);

    sh->set("g_vUVScale", uvscale);
    sh->set("g_txColor",    0);
    sh->set("g_txNormal",   1);
    sh->set("g_txSpecular", 2);
    sh->set("g_txColor1",   3);
    sh->set("g_txColor2",   4);

    for (u32 i = 0; i < 4; i++)
    {
      if (ter->texColor(i))
        gr->gc->gcSetTexture(1 + i, ter->texColor(i)->texture());
      else
        gr->gc->gcSetTexture(1 + i, gr->m_tex_black->texture());
    }

    kgmMesh::Vertex* v = msh->vertices();

    if (blend)
    {
      gr->gc->gcBlend(true, 0, gcblend_dstcol, gcblend_zero);
    }

    gr->render(msh);

    msh = ter->fill();

    if (msh)
    {
      gr->gc->gcCull(gc_null);

      gr->render(msh);

      gr->gc->gcCull(gccull_back);
    }

    if (blend)
    {
      gr->gc->gcBlend(false, 0, 0, 0);
    }

    material(null);
    shader(null, null, null, null);
  }

} /* namespace Render */
