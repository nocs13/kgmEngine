/*
 * Terrain.h
 *
 *  Created on: Feb 10, 2019
 *      Author: Goga
 */

#ifndef KGMGRAPHICS_RENDER_TERRAIN_H_
#define KGMGRAPHICS_RENDER_TERRAIN_H_

#include "BaseRender.h"
#include "../../kgmSystem/kgmThread.h"

class kgmGraphics;

class kgmShader;

namespace Render
{

class Terrain: public BaseRender
{
  friend class Thread;

  class Thread: public kgmThread
  {
    Terrain*         tr = null;
    kgmThread::Mutex mx = null;

    bool valid = false;

  public:
    Thread(Terrain* o)
    {
      tr = o;
      mx = kgmThread::mutex_create();
    }

    ~Thread()
    {
      kgmThread::mutex_free(mx);
    }

    void run();

    void wait()
    {
      join();
    }

    void stop()
    {
      valid = false;
    }

    void lock()
    {
      kgmThread::mutex_lock(mx);
    }

    void unlock()
    {
      kgmThread::mutex_unlock(mx);
    }

    bool canlock()
    {
      return kgmThread::mutex_lockable(mx);
    }
  };

  kgmShader* m_sh_cmap;
  kgmShader* m_sh_lmap;

  Thread*    m_thread = null;
  
public:
  Terrain(kgmGraphics* gr);
  ~Terrain();

  void render();
  void lightmap();
  void clear();
};

} /* namespace Render */

#endif /* KGMGRAPHICS_RENDER_TERRAIN_H_ */
