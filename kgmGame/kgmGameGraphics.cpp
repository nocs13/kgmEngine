#include "kgmGameGraphics.h"
#include "kgmGameApp.h"
#include "kgmIGame.h"
#include "kgmUnit.h"

#ifndef EDITOR
#define EDITOR
#endif

kgmGameGraphics::kgmGameGraphics(kgmIGC *g, kgmIResources* r)
  :kgmGraphics(g, r)
{
  m_game = ((kgmGameApp*)kgmGameApp::application())->game();
}

kgmGameGraphics::~kgmGameGraphics()
{
}

void kgmGameGraphics::render_3d()
{
#ifdef EDITOR

  gc3DMode();

  mtx4 mi;

  mi.identity();
  setWorldMatrix(mi);

  kgmGraphics::render(m_def_material);
  kgmGraphics::render(m_shaders[kgmShader::TypeBase]);

  if (m_gridline)
    kgmGraphics::render((kgmMesh*) m_gridline);

  if (m_pivot)
  {
    mtx4 mt = m_pivot->getTransform();
    setWorldMatrix(mt);
    gc->gcDepth(false, false, gccmp_lequal);
    kgmGraphics::render(m_shaders[kgmShader::TypeBase]);
    kgmGraphics::render((kgmMesh*) m_pivot);
    gc->gcDepth(true, true, gccmp_lequal);
  }

  if (!m_game)
  {
    m_game = ((kgmGameApp*)kgmGameApp::application())->game();
  }

  if (m_game)
  {
    kgmIGame::Iterator* i = m_game->gObjects();

    while(kgmUnit* un = i->next())
    {
      if (un->getType() == kgmUnit::Light)
      {
        drawLight(un);
      }
    }
  }

  kgmGraphics::render((kgmShader*) null);

#endif
}

void kgmGameGraphics::render_2d()
{
#ifdef EDITOR
#endif
}

#ifdef  EDITOR

void kgmGameGraphics::drawLight(kgmUnit* u)
{
  typedef struct { vec3 pos; u32 col; } V;

  V v[6];

  v[0].pos.x = - .2, v[0].pos.y = 0, v[0].pos.z = 0, v[0].col = 0xffffffff;
  v[1].pos.x =   .2, v[1].pos.y = 0, v[1].pos.z = 0, v[1].col = 0xffffffff;
  v[2].pos.x =    0, v[2].pos.y = - .2, v[2].pos.z = 0, v[2].col = 0xffffffff;
  v[3].pos.x =    0, v[3].pos.y =   .2, v[3].pos.z = 0, v[3].col = 0xffffffff;
  v[4].pos.x =    0, v[4].pos.y = 0, v[4].pos.z = - .2, v[4].col = 0xffffffff;
  v[5].pos.x =    0, v[5].pos.y = 0, v[5].pos.z =   .2, v[5].col = 0xffffffff;

  mtx4 mt = u->transform();
  setWorldMatrix(mt);
  kgmGraphics::render(m_shaders[kgmShader::TypeBase]);
  gc->gcDraw(gcpmt_lines, gcv_xyz | gcv_col, sizeof(V), 6, v, 0, 0, 0);
}

#endif
