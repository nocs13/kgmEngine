#ifndef KGMOBJECTS_H
#define KGMOBJECTS_H

#include "../kgmIGame.h"
#include "../kgmUnit.h"
#include "../kgmActor.h"
#include "../kgmEffect.h"
#include "../kgmGameResources.h"
#include "../../kgmGraphics/kgmMaterial.h"

class kgmResult: public kgmUnit
{
  KGM_UNIT(kgmResult);
public:

  kgmText* m_text = null;

  kgmString text;
  s32       result;

public:
  kgmResult(kgmIGame* g, u32 time = 0, s32 res = 0, kgmString txt = "");
  ~kgmResult();

  s32 getResult();

  void update(u32 t);
  void init();
  void exit();
};

class kgmParticlesObject: public kgmEffect
{
  KGM_UNIT(kgmParticlesObject);

protected:
  kgmParticles* particles = null;
  kgmMaterial*  material  = null;

  //For editor only

  kgmString     idTex;

public:
  kgmParticlesObject(kgmIGame* g,
               vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
               float speed = 0.0f, float div_speed = 0.0,
               float life = 1000,  float div_life = 0.5f,
               float size_start = .1f, float size_end = 1.0f,
               u32 count = 1,
               kgmString tid = "",
               bool loop = false);
  ~kgmParticlesObject();

  void setTexture(kgmTexture* t);
  void setSlideFrames(u32 rows, u32 cols);
  void init();
  void update(u32 ms);

#ifdef EDITOR
  void eupdate();
#endif
};

class kgmFlame: public kgmParticlesObject
{
  KGM_UNIT(kgmFlame);

public:
  kgmFlame(kgmIGame* g,
               vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
               float speed = 0.5f, float div_speed = 0.3f,
               float life = 2000,  float div_life = 0.5f,
               float size_start = .1f, float size_end = 1.0f,
               u32 count = 10,
               kgmString tid = "flame_a.tga",
               bool loop = true);
};

class kgmSmoke: public kgmParticlesObject
{
  KGM_UNIT(kgmSmoke);

public:
  kgmSmoke(kgmIGame* g,
           vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
           float speed = 0.1f, float div_speed = 0.5,
           float life = 5000,  float div_life = 0.5f,
           float size_start = .1f, float size_end = 1.0f,
           u32 count = 10,
           kgmString tid = "smoke_a.tga",
           bool loop = false);
};

class kgmExplode: public kgmParticlesObject
{
  KGM_UNIT(kgmExplode);

public:
  kgmExplode(kgmIGame* g,
             vec3 pos = vec3(0, 0, 0), vec3 vol = vec3(1, 1, 1), vec3 dir = vec3(0, 0, 0),
             float speed = 1.0f, float div_speed = 0.1f, float life = 1000, float div_life = 0.2f,
             float size_start = .1f, float size_end = 2.0f,
             u32 count = 10,
             bool loop = false,
             kgmString tid="fire_a.tga");
};

class kgmLaser: public kgmEffect
{
  KGM_UNIT(kgmLaser);

  struct Vertex
  {
    vec3 v;
    u32  c;
    vec2 uv;
  };

  kgmMesh*      mesh     = null;
  kgmMaterial*  material = null;

  u32  power;

public:
  kgmLaser(kgmIGame* g = null);
  kgmLaser(kgmIGame* g, u32 time, vec3 pos, vec3 rot, float speed,
           float hlen=0.5, float hwid=0.1);
  ~kgmLaser();
};

class kgmIonNozzle: public kgmEffect
{
  KGM_UNIT(kgmIonNozzle);

  struct Vertex
  {
    vec3 v;
    u32  c;
    vec2 uv;
  };

  kgmIGame*      game;
  kgmMaterial*   mtl;
  kgmParticles*  ptl;

  kgmVisual*     visual;
  kgmMesh*       mesh;

public:
  kgmIonNozzle(kgmIGame* g)
    :kgmEffect(g)
  {
    game     = g;
    m_visual = new kgmVisual();

    mtl = kgm_ptr<kgmMaterial>(new kgmMaterial());
    mtl->cull(false);
    mtl->depth(false);
    mtl->blend(true);
    mtl->m_srcblend = gcblend_srcalpha;
    mtl->m_dstblend = gcblend_one;
    mtl->setTexColor(g->getResources()->getTexture((char*)"point_d.tga"));
    mtl->m_type = "simple";
    mtl->setShader(g->getResources()->getShader("blend.glsl"));

    ptl = kgm_ptr<kgmParticles>(new kgmParticles());
    ptl->m_typerender  = kgmParticles::RTypePoint;
    ptl->m_count       = 25;
    ptl->m_speed       = 2.0f;
    ptl->m_life        = 200;
    ptl->direction     = vec3(-1,0,0);
    ptl->st_size       = .08f;
    ptl->en_size       = .02f;
    ptl->divlife(0.95);
    ptl->divspeed(0.95);
    ptl->build();

    visual = new kgmVisual();
    mesh = new kgmMesh();
    Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

    v[0]   = { {0, -0.1, -0.1}, 0xffffffff, {0, 1} };
    v[1]   = { {0, -0.1, 0.1},  0xffffffff, {0, 0} };
    v[2]   = { {0, 0.1, 0.1},   0xffffffff, {1, 0} };
    v[3]   = { {0, 0.1, 0.1},   0xffffffff, {1, 0} };
    v[4]   = { {0, 0.1, -0.1},  0xffffffff, {1, 1} };
    v[5]   = { {0, -0.1, -0.1}, 0xffffffff, {0, 1} };

    v[6]   = { {0, -0.1, 0},  0xffffffff, {0.5, 1} };
    v[7]   = { {0,  0.1, 0},  0xffffffff, {0.5, 0} };
    v[8]   = { {-0.5,  0.1, 0}, 0xffffffff, {1, 0} };
    v[9]   = { {-0.5, 0.1, 0},  0xffffffff, {1, 0} };
    v[10]  = { {-0.5, -0.1, 0}, 0xffffffff, {1, 1} };
    v[11]  = { {0, -0.1, 0},  0xffffffff, {0.5, 1} };

    v[12]  = { {0, 0, -0.1},  0xffffffff, {0.5, 1} };
    v[13]  = { {0, 0, 0.1},   0xffffffff, {0.5, 0} };
    v[14]  = { {-0.5, 0, 0.1},  0xffffffff, {1, 0} };
    v[15]  = { {-0.5, 0, 0.1},  0xffffffff, {1, 0} };
    v[16]  = { {-0.5, 0, -0.1}, 0xffffffff, {1, 1} };
    v[17]  = { {0, 0, -0.1},  0xffffffff, {0.5, 1} };

    m_visual->set(mesh);
    m_visual->set(mtl);
    visual->set(ptl);
    visual->set(mtl);
  }

  ~kgmIonNozzle()
  {
  }

  void update(u32 mls)
  {
    if(!valid())
      return;

    visual->getTransform() = m_visual->getTransform();
    visual->update();
  }

  void event(kgmObject* o, kgmString arg)
  {
    if(arg == "die")
    {
      remove();
    }
  }

  void setSpeed(float speed)
  {
    ptl->m_speed = 2.0 + 25 * speed;
  }
};

#endif // KGMOBJECTS_H
